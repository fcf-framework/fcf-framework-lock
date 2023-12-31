#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
 #include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "NamedMutexCallback.hpp"
#include "NamedMutexHelpers.hpp"
#include "Storage.hpp"
#include "Fcf/Scope.hpp"

class NamedMutexShm {
private:
  struct ShmData {
    bool                      init;
    std::chrono::milliseconds time;
    pthread_mutex_t           mutex;
  };

  struct LockData {
    ShmData*                data;
    int                     handle;
    bool                    complete;
    bool                    result;
    std::mutex              mutex;
    std::condition_variable condition;
    std::mutex              resultMutex;
    std::condition_variable resultCondition;
  };

  typedef std::shared_ptr<LockData> PLockData;

public:
  void lock(const char* a_name, bool a_try, const char* a_cacheDirectory, NamedMutexCallback a_cb) {
    int res;
    int shm_id          = -1;
    ShmData* mutexData  = 0;
    bool ready          = false;
    bool needInitialize = false;
    bool busy           = false;
    int numberAttempts1 = 2;
    int numberAttempts2 = 20; // > 100ms

    FCF_CLOSING_SCOPE(mutexData, shm_id, {
      if (mutexData && mutexData != MAP_FAILED){
        munmap(mutexData, sizeof(ShmData));
        mutexData = 0;
      }
      if (shm_id != -1) {
        close(shm_id);
        shm_id = -1;
      }
    });

    std::string name = "/" + getSafeNamedMutexName(a_name);
    for(int attempt1 = 0; !ready && attempt1 < numberAttempts1; ++attempt1) {
      for(int attempt2 = 0; !ready && attempt2 < numberAttempts2; ++attempt2) {
        shm_id = shm_open(name.c_str(), O_CREAT | O_RDWR | O_EXCL, 0666);
        if (shm_id == -1) {
          if (errno == EEXIST) {
            shm_id = shm_open(name.c_str(), O_RDWR, 0666);
            if (shm_id == -1) {
              throw std::runtime_error(std::string() + "Failed to open shared memory for named mutex: " + strerror(errno));
            }
          } else {
            throw std::runtime_error(std::string() + "Failed to open shared memory for named mutex: " + strerror(errno));
          }
          needInitialize = false;
        } else {
          needInitialize = true;
        }

        res = ftruncate(shm_id, sizeof(ShmData));
        if (res){
          throw std::runtime_error(std::string() + "Failed to set shared memory size for named mutex: " + strerror(errno));
        }

        mutexData = (ShmData*)mmap(0, sizeof(ShmData),  PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
        if (mutexData == MAP_FAILED){
          throw std::runtime_error(std::string() + "Failed to maping shared memory size for named mutex: " + strerror(errno));
        }
        if (!mutexData->time.count()) {
          mutexData->time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        }

        if (!needInitialize && !mutexData->init) {
          std::chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
          close(shm_id);
          shm_id = -1;
          if ((currentTime > mutexData->time && (currentTime - mutexData->time).count() > 100) ||
              (mutexData->time > currentTime && (mutexData->time - currentTime).count() > 100)
             ) {
            shm_unlink(name.c_str());
          } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
          }
        } else {
          ready = true;
          break;
        }
      }
    }

    try {
      if (!ready) {
        throw std::runtime_error("Failed to get ready status on a mutex that aborted its initialization");
      }
      if (needInitialize) {
        pthread_mutexattr_t mattr;
        res = pthread_mutexattr_init(&mattr);
        if (res) {
          throw std::runtime_error(std::string() + "Failed to initialize mutex attributes structure: " + strerror(errno));
        }
        FCF_CLOSING_SCOPE(mattr, {
          pthread_mutexattr_destroy(&mattr);
        });
        res = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        if (res) {
          throw std::runtime_error(std::string() + "Failed to set PTHREAD_PROCESS_SHARED flag for mutex attributes structure: " + strerror(errno));
        }
        res = pthread_mutexattr_setrobust(&mattr, PTHREAD_MUTEX_ROBUST);
        if (res) {
          throw std::runtime_error(std::string() + "Failed to set PTHREAD_MUTEX_ROBUST flag for mutex attributes structure: " + strerror(errno));
        }
        res = pthread_mutex_init(&mutexData->mutex, &mattr);
        if (res) {
          throw std::runtime_error(std::string() + "Failed to initialize mutex structure: " + strerror(errno));
        }
        mutexData->init = true;
      }
      if (a_try) {
        res = pthread_mutex_trylock(&mutexData->mutex);
        if (res && res != EOWNERDEAD) {
          if (res == EBUSY) {
            busy = true;
          } else {
            throw std::runtime_error(strerror(errno));
          }
        } else {
          pthread_mutex_consistent(&mutexData->mutex);
        }
      } else {
        res = pthread_mutex_lock(&mutexData->mutex);
        if (res && res != EOWNERDEAD) {
          throw std::runtime_error(strerror(errno));
        }
        pthread_mutex_consistent(&mutexData->mutex);
      }
    } catch(const std::runtime_error& e){
      throw e;
    }

    if (busy) {
      a_cb(0);
    } else {
      PLockData pdata(new LockData());
      pdata->data     = mutexData;
      pdata->handle   = shm_id;
      pdata->complete = false;
      pdata->result   = false;
      int key(_s.set(pdata));
      {
        FCF_CLOSING_SCOPE(_s, key, mutexData, {
          pthread_mutex_unlock(&mutexData->mutex);
          _s.remove(key);
        });
        a_cb(key);
        {
          std::unique_lock<std::mutex> lock(pdata->mutex);
          if (!pdata->complete){
            pdata->condition.wait(lock);
          }
        }
      }
      {
        std::unique_lock<std::mutex> lock(pdata->resultMutex);
        pdata->result = true;
        pdata->resultCondition.notify_all();
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
    {
      std::unique_lock<std::mutex> lock(pdata->resultMutex);
      if (!pdata->result){
        pdata->resultCondition.wait(lock);
      }
    }
  }

  private:
    Storage<PLockData> _s;
};
