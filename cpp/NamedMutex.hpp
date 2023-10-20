#if defined(__ANDROID__) || defined(__APPLE__)
  #include "NamedMutexFile.hpp"
  typedef NamedMutexFile NamedMutex;
#elif defined(WIN32)
  #include "NamedMutexWin.hpp"
  typedef NamedMutexWin NamedMutex;
#else
  #include "NamedMutexShm.hpp"
  typedef NamedMutexShm NamedMutex;
#endif
