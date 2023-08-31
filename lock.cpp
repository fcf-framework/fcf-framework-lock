#include <iostream>
#include <node.h>
#include <uv.h>
#include <thread>
#ifndef WIN32
  #include <sys/file.h>
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
  v8::Persistent<v8::Function>  callback;
  v8::Isolate*                  isolate;
  std::string                   error;
  bool                          unavailable;
  SLock(ELock a_type, THandle a_file, v8::Local<v8::Function> a_callback, v8::Isolate* a_isolate)
    : type(a_type), file(a_file), callback(a_isolate, a_callback), isolate(a_isolate), unavailable(false) {
  }
};

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

void LockComplete(uv_async_t* a_uvasync) {
  SLock* lockInfo = (SLock*)a_uvasync->data;
  {
    v8::HandleScope scope(lockInfo->isolate);
    if (lockInfo->error.empty()) {
      v8::Local<v8::Function>::New(lockInfo->isolate, lockInfo->callback)->Call(
        lockInfo->isolate->GetCurrentContext(),
        v8::Null(lockInfo->isolate),
        0,
        (v8::Local<v8::Value>*)0
      );
    } else {
      v8::Local<v8::Value> args[2] = {
                                        v8::Exception::Error(v8::String::NewFromUtf8(lockInfo->isolate, lockInfo->error.c_str()).ToLocalChecked()),
                                        lockInfo->unavailable ? v8::True(lockInfo->isolate) : v8::False(lockInfo->isolate)
                                     };
      v8::Local<v8::Function>::New(lockInfo->isolate, lockInfo->callback)->Call(
        lockInfo->isolate->GetCurrentContext(),
        v8::Null(lockInfo->isolate),
        2,
        args
      );
    }
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

  THandle handle = (THandle)uv_get_osfhandle(v8::Local<v8::Int32>::Cast(a_args[0])->Value());
  SLock* lockInfo = new SLock(LOCK,
                              handle,
                              v8::Local<v8::Function>::Cast(a_args[1]),
                              isolate
                              );
  uv_async_t* uvasync = new uv_async_t();
  uv_async_init(uv_default_loop(), uvasync, LockComplete);
  uvasync->data = lockInfo;

  std::thread(LockHandler, uvasync).detach();

  a_args.GetReturnValue().SetUndefined();
}


void Init(v8::Local<v8::Object> a_exports, v8::Local<v8::Value> a_module, void* a_priv) {
  NODE_SET_METHOD(a_exports, "lock", Lock<L_LOCK>);
  NODE_SET_METHOD(a_exports, "unlock", Lock<L_UNLOCK>);
  NODE_SET_METHOD(a_exports, "trylock", Lock<L_TRYLOCK>);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);

