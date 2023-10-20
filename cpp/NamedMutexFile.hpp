#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <deque>
#include <exception>
#include <set>
#include <mutex>
#include <filesystem>
#include "NamedMutexCallback.hpp"
#include "NamedMutexHelpers.hpp"

class NamedMutexFile {
public:
  void lock(const char* a_name, bool a_try, const char* a_cacheDirectory, NamedMutexCallback a_cb) {
    std::string mutexName(getSafeNamedMutexName(a_name));
    std::string directoryStr(_safeDirectoryPath(a_cacheDirectory));
    bool        directoryPrepared;
    {
      std::lock_guard<std::mutex> lock(_m);
      directoryPrepared = !!_d.count(directoryStr);
    }
    if (!directoryPrepared) {
      this->_prepareDirectory(directoryStr);
    }

    std::filesystem::path lockFilePath(directoryStr);
    lockFilePath /= mutexName;
    int fd = open(lockFilePath.string().c_str(), O_CREAT, 0666);
    if (fd == -1) {
      throw std::runtime_error(std::string() + "Failed to open file for named mutex: " + strerror(errno));
    }
    bool busy = false;
    int res = flock(fd, a_try ?  LOCK_NB | LOCK_EX : LOCK_EX);
    if (res) {
      if (a_try && errno == EWOULDBLOCK) {
        close(fd);
        busy = true;
      } else {
        throw std::runtime_error(std::string() + "Failed to get lock for named mutex: " + strerror(errno));
      }
    }

    if (busy) {
      a_cb(0);
    } else {
      a_cb(!fd ? -1 : fd);
    }
  }

  void unlock(int a_lock) {
    if (!a_lock) {
      throw std::runtime_error("Failed to unlock named mutex. Invalid lock key for named mutex");
    }
    a_lock = a_lock == -1 ? 0 : a_lock;
    int res = flock(a_lock, LOCK_UN);
    if (res){
      close(a_lock);
      throw std::runtime_error(std::string() + "Invalid unlock named mutex: " + strerror(errno));
    }
    res = close(a_lock);
    if (res){
      throw std::runtime_error(std::string() + "Invalid unlock named mutex: " + strerror(errno));
    }
  }

private:
  std::string _safeDirectoryPath(const char* a_directory){
    std::string str(a_directory);
    std::string directoryStr;
    std::regex re("([\\\\\\/]+)");
    std::smatch m;
    std::string::const_iterator p = str.cbegin();
    std::string::const_iterator e = str.cend();
    std::ssub_match complete;
    bool            ismatch = false;
    while (std::regex_search(p, e, m, re)) {
      p = m.suffix().first;
      directoryStr += m.prefix();
      directoryStr += "/";
      complete = m.suffix();
      ismatch = true;
    }
    if (ismatch){
      directoryStr += complete;
    } else {
      directoryStr += str;
    }
    return directoryStr;
  }

  void _prepareDirectory(const std::string& a_directoryStr) {
    std::filesystem::path curDirectory(a_directoryStr);
    std::deque<std::filesystem::path> parts;
    while(!curDirectory.empty() && curDirectory != "/"){
      if (std::filesystem::is_directory(curDirectory)) {
        break;
      }
      parts.push_front(curDirectory.filename());
      curDirectory = curDirectory.parent_path();
    }
    for(const std::filesystem::path& item : parts){
      curDirectory /= item;
      std::filesystem::create_directory(curDirectory.string());
      try {
        std::system((std::string("chmod 777 ") + curDirectory.string()).c_str());
      } catch(...){
      }
    }
  }

  std::mutex            _m;
  std::set<std::string> _d;
};
