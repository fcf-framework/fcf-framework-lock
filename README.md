# fcf-framework-lock

Packet provides functions for locking on a file and named mutexes.
The package is compatible with UNIX systems and Windows.

Package description page: https://fcf-framework.tech/packages/fcf-framework-lock

## lockFile(string a_filePath, function a_cb)
## lockFile(number a_fileHandle, function a_cb)
Performs a file lock
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - If the function executed with an error, then the argument contains an error object
        - number a_lock - Contains a lock handle to be passed to the unlockFile function to unlock the file.

## tryLockFile(string a_filePath, function a_cb)
## tryLockFile(number a_fileHandle, function a_cb)
## tryLockFile(string a_filePath, boolean a_quiet, function a_cb)
## tryLockFile(number a_fileHandle, boolean a_quiet, function a_cb)
Performs a lock on the file, if the file already has a lock, the function fails (the "unavailable" property of the error object is set to true)
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- boolean a_quiet = false - If true, then if a lock is held on a file that is already locked, the function exits without error, and the value of the a_lock argument in the callback function is undefined. If the value is not set or is false, then if the file is already locked, the function will fail with an error object containing an "unavailable" field equal to true.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - Error object. If the function was locking on an already locked file, then the error object will contain the "unavailable" property set to true.
        - number a_lock - If successful, contains a lock handle that must be passed to the unlockFile function to unlock the file.

## unlockFile(number a_lock, function a_cb)
Performs file unlocking
- number a_lock - The lock handle.
- function a_cb - The function of processing the result of the function execution.
    - Function signature a_cb: a_cb(Error|undefined a_error)
        - Error|undefined a_error - Error object.

## isLockFile(string a_filePath, function a_cb)
## isLockFile(number a_fileHandle, function a_cb)
Checks if the file is locked.
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, boolean a_isLocked)
        - Error|undefined a_error - Error object.
        - boolean a_isLocked - True if the file has a lock.

## lockNamedMutex(string a_name, function a_cb)
Locks a named mutex
- string a_name - The mutex name
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - If the function executed with an error, then the argument contains an error object
        - number a_lock - Contains a lock handle to be passed to the unlockNamedMutex function to unlock the mutex.

## tryLockNamedMutex(string a_name, function a_cb)
## tryLockNamedMutex(string a_name, boolean a_quiet, function a_cb)
Tries to lock a named mutex without waiting. If the mutex already has a lock, the function fails (the "unavailable" property of the error object is set to true)
- string a_name - The mutex name
- boolean a_quiet = false - If true, then if a lock is held that is already locked, the function exits without error, and the value of the a_lock argument in the callback function is undefined. If the value is not set or is false, then if the mutex is already locked, the function will fail with an error object containing an "unavailable" field equal to true.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - Error object. If the function was locking on an already locked mutex, then the error object will contain the "unavailable" property set to true.
        - number a_lock - If successful, contains a lock handle that must be passed to the unlockNamedMutex function to unlock the mutex.

## unlockNamedMutex(number a_lock, function a_cb)
Performs a mutex unlock
- number a_lock - The lock ID obtained by the lockNamedMutex tryLockNamedMutex functions
- function a_cb - The function of processing the result of the function execution.
    - Function signature a_cb: a_cb(Error|undefined a_error)
        - Error|undefined a_error - Error object.

## isLockNamedMutex(string a_filePath, function a_cb)
Checks whether the named mutex is locked
- string a_name - The mutex name
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, boolean a_isLocked)
        - Error|undefined a_error - Error object.
        - boolean a_isLocked - True if the mutex has a lock.
