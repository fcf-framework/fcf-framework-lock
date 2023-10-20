#ifndef WIN32
  #pragma GCC diagnostic ignored "-Wunused-result"
#else
  #pragma warning( disable : 4996 )
#endif

#include <iostream>
#include <map>
#include <memory>
#include <node.h>
#include <uv.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifndef WIN32
  #include <unistd.h>
  #include <sys/file.h>
  #include <sys/mman.h>
  #include <sys/stat.h>
  #include <fcntl.h>
#else
  #include <windows.h>
  #include <codecvt>
#endif

#include "NamedMutex.hpp"


#ifndef WIN32
  typedef int THandle;
#else
  typedef HANDLE THandle;
#endif

enum ELock {
  L_LOCK,
  L_TRYLOCK,
  L_UNLOCK,
};

struct SLock {
  ELock                         type;
  THandle                       file;
  int                           fileNumber;
  v8::Persistent<v8::Function>  callback;
  v8::Isolate*                  isolate;
  std::string                   error;
  bool                          unavailable;
  SLock(ELock a_type, THandle a_file, int a_fileNumber, v8::Local<v8::Function> a_callback, v8::Isolate* a_isolate)
    : type(a_type), file(a_file), fileNumber(a_fileNumber), callback(a_isolate, a_callback), isolate(a_isolate), unavailable(false) {
  }
};

struct SLockNamedMutex {
  ELock                         type;
  std::string                   name;
  std::string                   cacheDirectory;
  int                           fileNumber;
  v8::Persistent<v8::Function>  callback;
  v8::Isolate*                  isolate;
  std::string                   error;
  bool                          unavailable;
  SLockNamedMutex(ELock a_type, const std::string& a_name, const std::string& a_cacheDirectory, int a_fileNumber, v8::Local<v8::Function> a_callback, v8::Isolate* a_isolate)
    : type(a_type), name(a_name), cacheDirectory(a_cacheDirectory), fileNumber(a_fileNumber), callback(a_isolate, a_callback), isolate(a_isolate), unavailable(false) {
  }
};

namespace {
  NamedMutex namedMutexLocker;
}


std::string getErrorMessage() {
  #ifndef WIN32
    return strerror(errno);
  #else
    DWORD error = GetLastError();
    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string result(conv.to_bytes(std::wstring(messageBuffer, size)));
    LocalFree(messageBuffer);
    return result;
  #endif
}

void removeAsyncHandle(uv_handle_t* a_handle) {
  delete (uv_async_t*)a_handle;
}

void LockHandler(uv_async_t* a_uvasync) {
  SLock* lockInfo = (SLock*)a_uvasync->data;
  #ifndef WIN32
    int mode = lockInfo->type == L_LOCK    ? LOCK_EX :
               lockInfo->type == L_TRYLOCK ? LOCK_NB|LOCK_EX:
  	                                         LOCK_UN;
    int res = flock(lockInfo->file, mode);
    if (res  && errno == EWOULDBLOCK && lockInfo->type == L_TRYLOCK) {
      lockInfo->unavailable = true;
      lockInfo->error = "Resource temporarily unavailable";
    } else if (res) {
      lockInfo->error = getErrorMessage();
    }
  #else
    if (lockInfo->type == L_LOCK || lockInfo->type == L_TRYLOCK) {
      int flags = lockInfo->type == L_LOCK    ? LOCKFILE_EXCLUSIVE_LOCK
                                              : LOCKFILE_EXCLUSIVE_LOCK|LOCKFILE_FAIL_IMMEDIATELY;
      OVERLAPPED overlapped;
      memset(&overlapped, 0, sizeof(overlapped));
      BOOL res = LockFileEx(lockInfo->file, flags, 0, 0xffffffff, 0xffffffff, &overlapped);
      DWORD error = GetLastError();
      if (lockInfo->type == L_TRYLOCK && error == ERROR_LOCK_VIOLATION) {
        lockInfo->unavailable = true;
        lockInfo->error = "Resource temporarily unavailable";
      } else if (!res) {
        lockInfo->error = getErrorMessage();
      }
    } else {
      OVERLAPPED overlapped;
      memset(&overlapped, 0, sizeof(overlapped));
      DWORD res = UnlockFileEx(lockInfo->file, 0, 0xffffffff, 0xffffffff, &overlapped);
      if (!res){
        lockInfo->error = getErrorMessage();
      }
    }
  #endif
  uv_async_send(a_uvasync);
}

template <typename TData>
void Complete(uv_async_t* a_uvasync) {
  TData* lockInfo = (TData*)a_uvasync->data;
  {
    v8::HandleScope scope(lockInfo->isolate);
    v8::Local<v8::Value> args[3] = {
                                      !lockInfo->error.empty() ? v8::Exception::Error(v8::String::NewFromUtf8(lockInfo->isolate, lockInfo->error.c_str()).ToLocalChecked())
                                                               : (v8::Local<v8::Value>)v8::Undefined(lockInfo->isolate),
                                      lockInfo->fileNumber     ? (v8::Local<v8::Value>)v8::Number::New(lockInfo->isolate, lockInfo->fileNumber)
                                                               : (v8::Local<v8::Value>)v8::Undefined(lockInfo->isolate),
                                      lockInfo->unavailable    ? v8::True(lockInfo->isolate)
                                                               : v8::False(lockInfo->isolate)
                                   };
    v8::Local<v8::Function>::New(lockInfo->isolate, lockInfo->callback)->Call(
      lockInfo->isolate->GetCurrentContext(),
      v8::Null(lockInfo->isolate),
      3,
      args
    );
  }
  lockInfo->callback.Reset();
  delete lockInfo;
  uv_close((uv_handle_t*)a_uvasync, removeAsyncHandle);
}

template <ELock LOCK>
void Lock(const v8::FunctionCallbackInfo<v8::Value>& a_args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  if (
        a_args.Length() < 2 ||
        !a_args[0]->IsNumber() ||
        !a_args[1]->IsFunction()
     ) {
    v8::Local<v8::String> msg(v8::String::NewFromUtf8Literal(isolate, "Wrong arguments"));
    isolate->ThrowException(v8::Exception::TypeError(msg));
    return;
  }

  int     fileNumber = v8::Local<v8::Int32>::Cast(a_args[0])->Value();
  THandle handle     = (THandle)uv_get_osfhandle(fileNumber);
  SLock* lockInfo = new SLock(LOCK,
                              handle,
                              fileNumber,
                              v8::Local<v8::Function>::Cast(a_args[1]),
                              isolate
                              );
  uv_async_t* uvasync = new uv_async_t();
  uv_async_init(uv_default_loop(), uvasync, Complete<SLock>);
  uvasync->data = lockInfo;

  std::thread(LockHandler, uvasync).detach();

  a_args.GetReturnValue().SetUndefined();
}

void LockNamedMutexHandler(uv_async_t* a_uvasync) {
  SLockNamedMutex* lockInfo = (SLockNamedMutex*)a_uvasync->data;

  if (lockInfo->type == L_LOCK || lockInfo->type == L_TRYLOCK) {
    try {
      namedMutexLocker.lock(
        lockInfo->name.c_str(),
        lockInfo->type == L_TRYLOCK,
        lockInfo->cacheDirectory.c_str(),
        [lockInfo, a_uvasync](int a_lock){
          if (a_lock){
            lockInfo->fileNumber = a_lock;
          } else {
            lockInfo->unavailable = true;
            lockInfo->error = "Resource temporarily unavailable";
          }
          uv_async_send(a_uvasync);
        }
      );
    } catch(const std::exception& e) {
      lockInfo->error = e.what();
      uv_async_send(a_uvasync);
    }
  } else {
    try {
      namedMutexLocker.unlock(lockInfo->fileNumber);
      uv_async_send(a_uvasync);
    } catch(const std::exception& e) {
      lockInfo->error = e.what();
      uv_async_send(a_uvasync);
    }
  }
}


template <ELock LOCK>
void LockNamedMutex(const v8::FunctionCallbackInfo<v8::Value>& a_args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  std::string              name;
  std::string              cacheDirectory;
  int                      fileNumber = 0;
  v8::Local<v8::Function>  callback;

  if (LOCK == L_LOCK || LOCK == L_TRYLOCK) {
    if (
          a_args.Length() < 3 ||
          !a_args[0]->IsString() ||
          !a_args[1]->IsString() ||
          !a_args[2]->IsFunction()
       ) {
      v8::Local<v8::String> msg(v8::String::NewFromUtf8Literal(isolate, "Wrong arguments"));
      isolate->ThrowException(v8::Exception::TypeError(msg));
      return;
    }
    name            = std::string(*v8::String::Utf8Value(isolate, a_args[0]));
    cacheDirectory  = std::string(*v8::String::Utf8Value(isolate, a_args[1]));
    callback        = v8::Local<v8::Function>::Cast(a_args[2]);
  } else {
    if (
          a_args.Length() < 2 ||
          !a_args[0]->IsNumber() ||
          !a_args[1]->IsFunction()
       ) {
      v8::Local<v8::String> msg(v8::String::NewFromUtf8Literal(isolate, "Wrong arguments"));
      isolate->ThrowException(v8::Exception::TypeError(msg));
      return;
    }
    fileNumber      = v8::Local<v8::Int32>::Cast(a_args[0])->Value();
    callback        = v8::Local<v8::Function>::Cast(a_args[1]);
  }
  SLockNamedMutex* lockInfo = new SLockNamedMutex(LOCK,
                                                  name,
                                                  cacheDirectory,
                                                  fileNumber,
                                                  callback,
                                                  isolate
                                                  );
  uv_async_t* uvasync = new uv_async_t();
  uv_async_init(uv_default_loop(), uvasync, Complete<SLockNamedMutex>);
  uvasync->data = lockInfo;

  std::thread(LockNamedMutexHandler, uvasync).detach();

  a_args.GetReturnValue().SetUndefined();
}


void Init(v8::Local<v8::Object> a_exports, v8::Local<v8::Value> a_module, void* a_priv) {
  NODE_SET_METHOD(a_exports, "lockFile", Lock<L_LOCK>);
  NODE_SET_METHOD(a_exports, "unlockFile", Lock<L_UNLOCK>);
  NODE_SET_METHOD(a_exports, "trylockFile", Lock<L_TRYLOCK>);
  NODE_SET_METHOD(a_exports, "lockNamedMutex", LockNamedMutex<L_LOCK>);
  NODE_SET_METHOD(a_exports, "unlockNamedMutex", LockNamedMutex<L_UNLOCK>);
  NODE_SET_METHOD(a_exports, "trylockNamedMutex", LockNamedMutex<L_TRYLOCK>);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
