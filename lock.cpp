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
  #pragma GCC diagnostic ignored "-Wunused-result"
#else
  #include <windows.h>
  #include <codecvt>
  #pragma warning( disable : 4996 )
#endif


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
  THandle                       file;
  int                           fileNumber;
  v8::Persistent<v8::Function>  callback;
  v8::Isolate*                  isolate;
  std::string                   error;
  bool                          unavailable;
  SLockNamedMutex(ELock a_type, const std::string& a_name, THandle a_file, int a_fileNumber, v8::Local<v8::Function> a_callback, v8::Isolate* a_isolate)
    : type(a_type), name(a_name), file(a_file), fileNumber(a_fileNumber), callback(a_isolate, a_callback), isolate(a_isolate), unavailable(false) {
  }
};


#ifdef WIN32

struct LockNamedMutexInfo{
  std::mutex              mutex;
  std::condition_variable condition;
  bool                    wait;
  THandle                 handle;
};

typedef std::shared_ptr<LockNamedMutexInfo> PLockNamedMutexInfo;

class LockNMStorage{
public:
  LockNMStorage()
    : _c(0) {
  }

  int set(PLockNamedMutexInfo a_info){
    std::lock_guard<std::mutex> guard(_m);
    while(true){
      ++_c;
      if (_c < 0){
        _c = 0;
        continue;
      }
      if (_s.find(_c) != _s.end()){
        continue;
      }
      break;
    }
    _s[_c] = a_info;
    return _c;
  }

  PLockNamedMutexInfo get(int a_key){
    std::lock_guard<std::mutex> guard(_m);
    std::map<int, PLockNamedMutexInfo>::iterator it = _s.find(a_key);
    return it != _s.end() ? it->second : 0;
  }

  void remove(int a_key) {
    std::lock_guard<std::mutex> guard(_m);
    _s.erase(a_key);
    if (a_key == _c) {
      while(_c > 0 && _s.find(_c) == _s.end()) {
        --_c;
      }
    }
  }

private:
  int                                _c;
  std::map<int, PLockNamedMutexInfo> _s;
  std::mutex                         _m;
};

LockNMStorage lockNMStorage;
#endif

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
  #ifndef WIN32
    if (lockInfo->type != L_UNLOCK) {
      #if defined(__ANDROID__) || defined(__APPLE__)
        int fd = open(lockInfo->name.c_str(), O_CREAT, 0666);
      #else
        int fd = shm_open(lockInfo->name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
      #endif
      if (fd != -1) {
        lockInfo->file       = fd;
        lockInfo->fileNumber = fd ? fd : -1;
      } else {
        lockInfo->error = getErrorMessage();
      }
    }

    if (lockInfo->error.empty()) {
      int lockMode = lockInfo->type == L_LOCK    ? LOCK_EX :
                     lockInfo->type == L_TRYLOCK ? LOCK_NB|LOCK_EX:
    	                                             LOCK_UN;
      int res = flock(lockInfo->file, lockMode);
      if (res  && errno == EWOULDBLOCK && lockInfo->type == L_TRYLOCK) {
        lockInfo->unavailable = true;
        lockInfo->error = "Resource temporarily unavailable";
      } else if (res){
        lockInfo->error = getErrorMessage();
      }
    }

    if (!lockInfo->error.empty() || lockInfo->type == L_UNLOCK) {
      close(lockInfo->file);
      lockInfo->file = -1;
      lockInfo->fileNumber = 0;
    }
    uv_async_send(a_uvasync);
  #else
    if (lockInfo->type == L_LOCK || lockInfo->type == L_TRYLOCK) {
      PLockNamedMutexInfo info;
      do {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wname(conv.from_bytes(lockInfo->name));
        lockInfo->file = CreateMutexW(NULL, FALSE, wname.c_str());
        if (!lockInfo->file){
          lockInfo->error = getErrorMessage();
          break;
        }
        DWORD res = WaitForSingleObject(lockInfo->file, lockInfo->type == L_TRYLOCK ? 0 : INFINITE);
        if (res != WAIT_OBJECT_0 && res != WAIT_ABANDONED) {
          if (res == WAIT_TIMEOUT) {
            lockInfo->unavailable = true;
            lockInfo->error = "Resource temporarily unavailable";
          } else {
            lockInfo->error = getErrorMessage();
          }
          CloseHandle(lockInfo->file);
          lockInfo->file = 0;
          break;
        }
        info.reset(new LockNamedMutexInfo());
        info->handle = lockInfo->file;
        info->wait   = true;
        lockInfo->fileNumber = lockNMStorage.set(info);
      } while(false);
      uv_async_send(a_uvasync);
      if (info.get()){
        std::unique_lock lock(info->mutex);
        if (info->wait){
          info->condition.wait(lock);
        }
        ReleaseMutex(info->handle);
        CloseHandle(info->handle);
      }
    } else {
      PLockNamedMutexInfo info = lockNMStorage.get(lockInfo->fileNumber);
      lockNMStorage.remove(lockInfo->fileNumber);
      lockInfo->file = 0;
      lockInfo->fileNumber = 0;
      if (info.get()){
        std::unique_lock lock(info->mutex);
        info->wait = false;
        info->condition.notify_all();
      }
      uv_async_send(a_uvasync);
    }
  #endif
}


template <ELock LOCK>
void LockNamedMutex(const v8::FunctionCallbackInfo<v8::Value>& a_args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  if (
        a_args.Length() < 2 ||
        ((LOCK == L_LOCK || LOCK == L_TRYLOCK) && !a_args[0]->IsString()) ||
        ((LOCK == L_UNLOCK) && !a_args[0]->IsNumber()) ||
        !a_args[1]->IsFunction()
     ) {
    v8::Local<v8::String> msg(v8::String::NewFromUtf8Literal(isolate, "Wrong arguments"));
    isolate->ThrowException(v8::Exception::TypeError(msg));
    return;
  }

  std::string name        = LOCK == L_LOCK || LOCK == L_TRYLOCK ? std::string(*v8::String::Utf8Value(isolate, a_args[0])) : std::string();
  int         fileNumber  = LOCK == L_UNLOCK ? v8::Local<v8::Int32>::Cast(a_args[0])->Value() : 0;
  #ifndef WIN32
    THandle file = fileNumber == -1 ? 0 : fileNumber;
  #else
    THandle file = 0;
  #endif

  SLockNamedMutex* lockInfo = new SLockNamedMutex(LOCK,
                              name,
                              file,
                              fileNumber,
                              v8::Local<v8::Function>::Cast(a_args[1]),
                              isolate
                              );
  uv_async_t* uvasync = new uv_async_t();
  uv_async_init(uv_default_loop(), uvasync, Complete<SLockNamedMutex>);
  uvasync->data = lockInfo;

  std::thread(LockNamedMutexHandler, uvasync).detach();

  a_args.GetReturnValue().SetUndefined();
}


void Init(v8::Local<v8::Object> a_exports, v8::Local<v8::Value> a_module, void* a_priv) {
  NODE_SET_METHOD(a_exports, "lock", Lock<L_LOCK>);
  NODE_SET_METHOD(a_exports, "unlock", Lock<L_UNLOCK>);
  NODE_SET_METHOD(a_exports, "trylock", Lock<L_TRYLOCK>);
  NODE_SET_METHOD(a_exports, "lockNamedMutex", LockNamedMutex<L_LOCK>);
  NODE_SET_METHOD(a_exports, "unlockNamedMutex", LockNamedMutex<L_UNLOCK>);
  NODE_SET_METHOD(a_exports, "trylockNamedMutex", LockNamedMutex<L_TRYLOCK>);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
