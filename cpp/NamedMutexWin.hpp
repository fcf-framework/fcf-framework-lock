
#include <windows.h>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <codecvt>
#include "NamedMutexCallback.hpp"
#include "NamedMutexHelpers.hpp"
#include "Storage.hpp"
#include "Fcf/Scope.hpp"


class NamedMutexWin {
private:
  struct LockData {
    bool                    complete;
    std::mutex              mutex;
    std::condition_variable condition;
  };

  typedef std::shared_ptr<LockData> PLockData;

public:

  void lock(const char* a_name, bool a_try, const char* a_cacheDirectory, NamedMutexCallback a_cb) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wname(conv.from_bytes(std::string("Global\\") + getSafeNamedMutexName(a_name)));
    HANDLE handle = CreateMutexW(NULL, FALSE, wname.c_str());
    if (!handle) {
      throw std::runtime_error(std::string() + "Failed to initialize named mutex: " + _getErrorMessage());
    }
    FCF_CLOSING_SCOPE(handle, {
      CloseHandle(handle);
    });

    DWORD res = WaitForSingleObject(handle, a_try ? 0 : INFINITE);
    if (res != WAIT_OBJECT_0 && res != WAIT_ABANDONED) {
      if (a_try && res == WAIT_TIMEOUT) {
        a_cb(0);
      } else {
        throw std::runtime_error(std::string() + "Failed to wait named mutex: " + _getErrorMessage());
      }
    } else {
      PLockData pdata(new LockData());
      pdata->complete = false;
      int key = _s.set(pdata);
      FCF_CLOSING_SCOPE(_s, key, handle, {
        _s.remove(key);
        ReleaseMutex(handle);
      });
      a_cb(key);
      {
        std::unique_lock<std::mutex> lock(pdata->mutex);
        if (!pdata->complete){
          pdata->condition.wait(lock);
        }
      }
    }
  }

  void unlock(int a_lock) {
    PLockData pdata(_s.get(a_lock));
    if (!pdata.get()) {
      throw std::runtime_error("Failed to unlock named mutex. Invalid lock key for named mutex");
    }
    {
      std::unique_lock<std::mutex> lock(pdata->mutex);
      pdata->complete = true;
      pdata->condition.notify_all();
    }
  }
private:

  std::string _getErrorMessage() {
    DWORD error = GetLastError();
    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string result(conv.to_bytes(std::wstring(messageBuffer, size)));
    LocalFree(messageBuffer);
    return result;
  }

  Storage<PLockData> _s;


};
