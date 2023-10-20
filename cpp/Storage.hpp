#pragma once
#include <map>
#include <memory>
#include <mutex>

template <typename Type>
class Storage {
public:
  Storage()
    : _c(0) {
  }

  int set(Type a_info){
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

  Type get(int a_key){
    std::lock_guard<std::mutex> guard(_m);
    typename std::map<int, Type>::iterator it = _s.find(a_key);
    return it != _s.end() ? it->second : Type();
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
  int                 _c;
  std::map<int, Type> _s;
  std::mutex          _m;
};
