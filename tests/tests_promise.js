let libLock = require("../index.js");
let libFS   = require("fs");
let libOS   = require("os");

let diff;
let start;

exports.promise = new Promise((a_res)=>{
  a_res();
})
// lockFile (by path) && unlockFile
.then(()=>{
    start = Date.now();
    let promises = [];
    for(let i = 0; i < 10; ++i) {
      promises.push(new Promise((a_res, a_rej) => {
        libLock.lockFile("index.js", (a_error, a_lock)=>{
          if (a_error){
            invalidTest(a_error);
          }
          setTimeout(()=>{
            libLock.unlockFile(a_lock, (a_error)=>{
              if (a_error){
                invalidTest(a_error);
              }
              a_res();
            });
          }, 100);
        })
      }));
    }

    return Promise.all(promises);
})
.then(()=>{
  let diff = Date.now() - start;
  if (diff < 1000 || diff > 1200) {
    throw new Error(`(1) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
  }
})
// lockFile (by descriptor) && unlockFile
.then(()=>{
  start = Date.now();
  let promises = [];
  for(let i = 0; i < 10; ++i) {
    promises.push(new Promise((a_res, a_rej) => {
      libFS.open("index.js", "r", (a_error, a_fd) => {
        if (a_error){
          a_rej(a_error);
          return;
        }
        libLock.lockFile(a_fd, (a_error, a_lock)=>{
            if (a_error) {
              a_rej(a_error);
              return;
            }
           setTimeout(()=>{
            libLock.unlockFile(a_lock, (a_error)=>{
              if (a_error){
                a_rej(a_error);
                return;
              }
              libFS.close(a_fd, ()=>{});
              a_res();
            });
          }, 100);
        });
      });
    }));
  }

  return Promise.all(promises);
})
.then(()=>{
  diff = Date.now() - start;
  if (diff < 1000 || diff > 1200){
    invalidTest(`(2) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
  }
})
// tryLockFile (by descriptor) && unlockFile
.then(()=>{
  let lock;
  return new Promise((a_res, a_rej)=>{
    a_res();
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockFile("index.js", (a_error, a_lock)=>{
        if (a_error) {
          a_rej(a_error);
          return;
        }
        lock = a_lock;
        a_res();
      })
    })
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js", (a_error, a_lock)=>{
        if (a_error && a_error.unavailable){
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js", true, (a_error, a_lock)=>{
        if (!a_error && a_lock === undefined){
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js12312321313", (a_error, a_lock)=>{
        if (a_error && !a_error.unavailable){
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js12312321313", true, (a_error, a_lock)=>{
        if (a_error && !a_error.unavailable){
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockFile(lock, (a_error)=>{
        lock = undefined;
        if (!a_error){
          a_res();
        } else {
          a_rej(a_error);
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js", (a_error, a_lock)=>{
        if (!a_error && a_lock){
          lock = a_lock;
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockFile("index.js", true, (a_error, a_lock)=>{
        if (a_lock === undefined){
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockFile(lock, (a_error)=>{
        lock = undefined;
        if (!a_error){
          a_res();
        } else {
          a_rej(a_error);
        }
      });
    });
  })
})
// isLockFile (by descriptor) && unlockFile
.then(()=>{
  let lock;
  return new Promise((a_res)=>{ a_res() })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockFile("index.js", (a_error, a_lock)=>{
        if (a_lock){
          lock = a_lock;
          a_res(a_lock);
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockFile("index.js", (a_error, a_islock)=>{
        if (!a_error && a_islock){
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockFile(lock, (a_error)=>{
        lock = undefined;
        if (!a_error){
          a_res();
        } else {
          a_rej(a_error);
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockFile("index.js", (a_error, a_islock)=>{
        if (!a_error && !a_islock){
          a_res();
        } else if (a_error) {
          a_rej(a_error);
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
})
// lockNamedMutex && unlockNamedMutex
.then(()=>{
  let lock;
  return new Promise((a_res)=>{ a_res() })
  .then(()=>{
    start = Date.now();
    let promises = [];
    for(let i = 0; i < 10; ++i) {
      promises.push(new Promise((a_res, a_rej) => {
        libLock.lockNamedMutex("FCF_TEST_MUTEX", (a_error, a_lock)=>{
          if (a_error){
            a_rej(a_error);
            return;
          }
          setTimeout(()=>{
            libLock.unlockNamedMutex(a_lock, (a_error)=>{
              if (a_error){
                a_rej(a_error);
                return;
              }
              a_res();
            });
          }, 100);
        })
      }));
    }

    return Promise.all(promises);
  })
  .then(()=>{
    let diff = Date.now() - start;
    if (diff < 1000 || diff > 1200){
      throw new Error(`(3) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
    }
  })
})
// tryLockNamedMutex && unlockNamedMutex
.then(()=>{
  let mutexName = "FCF_TEST_MUTEX";
  let errorMutexName = "FCF_TEST_MUTEX/@#$%^&*()_111";
  for(let i = 0; i < 9999; ++i){
    errorMutexName += ".";
  }
  let lock;
  return new Promise((a_res)=>{a_res();})
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockNamedMutex(mutexName, (a_error, a_lock)=>{
        if (a_error){
          a_rej(a_error);
          return;
        }
        lock = a_lock;
        a_res();
      })
    })
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName,(a_error, a_lock)=>{
        if (!a_error || !a_error.unavailable){
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName, true,(a_error, a_lock)=>{
        if (!a_error && a_lock === undefined){
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    if (libOS.platform != "win32") {
      return new Promise((a_res, a_rej)=>{
        libLock.tryLockNamedMutex(errorMutexName, (a_error, a_lock)=>{
          if (a_error && !a_error.unavailable){
            a_res();
          } else {
            a_rej(new Error("Invalid logic"));
          }
        });
      });
    }
  })
  .then(()=>{
    if (libOS.platform != "win32") {
      return new Promise((a_res, a_rej)=>{
        libLock.tryLockNamedMutex(errorMutexName, true, (a_error, a_lock)=>{
          if (a_error && !a_error.unavailable){
            a_res();
          } else {
            a_rej(new Error("Invalid logic"));
          }
        });
      });
    }
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockNamedMutex(lock, (a_error)=>{
        if (a_error){
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName,(a_error, a_lock)=>{
        if (!a_error && a_lock){
          lock = a_lock
          a_res();
        } else {
          a_rej(a_error);
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName, true, (a_error, a_lock)=>{
        if (!a_error && a_lock === undefined){
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockNamedMutex(lock, (a_error, a_lock)=>{
        if (a_error){
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName, true, (a_error, a_lock)=>{
        if (!a_error && a_lock){
          lock = a_lock;
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.tryLockNamedMutex(mutexName, true, (a_error, a_lock)=>{
        if (!a_error && a_lock === undefined){
          a_res();
        } else {
          a_rej(new Error("Invalid logic"));
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockNamedMutex(lock, (a_error, a_lock)=>{
        if (a_error){
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });

  })
})
// isLockNamedMutex && unlockNamedMutex
.then(()=>{
  let mutexName = "FCF_TEST_MUTEX";
  let lock;
  return new Promise((a_res, a_rej)=>{
    a_res();
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockNamedMutex(mutexName, (a_error, a_islock)=>{
        if (a_error || a_islock){
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockNamedMutex(mutexName, (a_error, a_lock)=>{
        if (a_error){
          a_rej(a_error);
        } else if (!a_lock){
          a_rej(new Error("Invalid logic"));
        } else {
          lock = a_lock;
          a_res(a_lock);
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockNamedMutex(mutexName, (a_error, a_islock)=>{
        if (a_error || !a_islock){
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockNamedMutex(lock, (a_error)=>{
        if (a_error){
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockNamedMutex(mutexName, (a_error, a_islock)=>{
        if (a_error || a_islock){
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockNamedMutex("123/:\\123@#$%^&*()", (a_error, a_lock)=>{
        if (a_error){
          a_rej(a_error);
        } else if (!a_lock){
          a_rej(new Error("Invalid logic"));
        } else {
          lock = a_lock;
          a_res(a_lock);
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockNamedMutex(lock, (a_error)=>{
        if (a_error){
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
})
.then(()=>{
  console.log("Complete tests promise");
})
.catch((a_error)=>{
  console.log(a_error);
  process.exit(1);
})
;
