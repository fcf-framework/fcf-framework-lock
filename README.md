# fcf-framework-lock

The package provides locking functions on a file using the flock or LockFileEx function.

# lockFile(string a_filePath, function a_cb)
# lockFile(number a_fileHandle, function a_cb)
Performs a file lock
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - If the function executed with an error, then the argument contains an error object
        - number a_lock - Contains a lock handle to be passed to the unlockFile function to unlock the file.

# tryLockFile(string a_filePath, function a_cb)
# tryLockFile(number a_fileHandle, function a_cb)
Performs a lock on the file, if the file already has a lock, the function fails (the "unavailable" property of the error object is set to true)
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, number a_lock)
        - Error|undefined a_error - Error object. If the function was locking on an already locked file, then the error object will contain the "unavailable" property set to true.
        - number a_lock - If successful, contains a lock handle that must be passed to the unlockFile function to unlock the file.

# unlockFile(number a_lock, function a_cb)
Performs file unlocking
- number a_lock - The lock handle.
- function a_cb - The function of processing the result of the function execution.
    - Сигнатура функции a_cb: a_cb(Error|undefined a_error)
        - Error|undefined a_error - Error object.

# isLockFile(string a_filePath, function a_cb)
# isLockFile(number a_fileHandle, function a_cb)
Checks if the file is locked.
- string a_filePath - The path to the file.
- number a_fileHandle - The open file handle returned by the FS.open() functions.
- function a_cb - The function of processing the result of the function execution.
    - Function signature: a_cb(Error|undefined a_error, boolean a_isLocked)
        - Error|undefined a_error - Error object.
        - boolean a_isLocked - True if the file has a lock.
