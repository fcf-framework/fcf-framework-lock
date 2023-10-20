let libFS   = require("fs");
let libOS   = require("os");
let libUtil = require("util");
let libPath = require("path");
let libLock = require("./build/Release/lock");

let fcf = typeof global !== 'undefined' && global.fcf ? global.fcf
                                                      : {};
if (typeof global !== 'undefined') {
  global.fcf = fcf;
}

if (!fcf.NLock) {
  fcf.NLock = {};
}

module.exports = fcf.NLock;

function getCahceDirectory() {
  return libPath.join(libOS.tmpdir(), "fcf-framework", "namedmutex");
}

const stAfterClose = {};

function restoreStack(a_error, a_stack) {
  if (a_error.stack.split("\n").length < 2){
    a_error.stack = a_error.stack + a_stack.substr(a_stack.indexOf("\n"));
  }
}

function call(a_methodName, a_fd, a_restoreStack, a_cb){
  libLock[a_methodName](a_fd, (a_error, a_result, a_unavailable)=>{
    if (a_unavailable){
      a_error.unavailable = a_unavailable;
    }
    if (a_error){
      restoreStack(a_error, a_restoreStack);
      a_cb(a_error);
    } else {
      a_cb(undefined, a_result);
    }
  });
}

function lockFile(a_file, a_try, a_cb) {
  let stack = (new Error()).stack;
  try {
    if (typeof a_file == "string") {
      libFS.open(a_file, "r", (a_error, a_fd)=>{
        if (a_error) {
          restoreStack(a_error, stack);
          a_cb(a_error);
        } else {
          call(a_try ? "trylockFile" : "lockFile", a_fd, stack, (a_error, a_lock)=>{
            if (a_error) {
              libFS.close(a_fd, ()=>{
                a_cb(a_error);
              });
            } else {
              stAfterClose[a_lock] = a_lock;
              a_cb(undefined, a_lock);
            }
          });
        }
      });
    } else if (typeof a_file == "number") {
      call(a_try ? "trylockFile" : "lockFile", a_file, stack, a_cb);
    } else {
      throw new Error("The argument is not a lock file handle or a string containing the file path");
    }
  } catch(e) {
    a_cb(e);
  }
};

function unlockFile(a_lock, a_cb) {
  let stack = (new Error()).stack;
  function completeLock(a_error) {
    if (a_lock in stAfterClose) {
      delete stAfterClose[a_lock];
      libFS.close(a_lock, ()=>{
        a_cb(a_error);
      });
    } else {
      a_cb(a_error);
    }
  }

  if (typeof a_lock !== "number") {
    a_cb(new Error("Argument is not a lock file descriptor"));
    return;
  }

  try {
    call("unlockFile", a_lock, stack, completeLock);
  } catch(e){
    completeLock(e);
  }
}

function islockFile(a_file, a_cb) {
  lockFile(a_file, true, (a_error, a_lock) => {
    if (!a_error) {
      unlockFile(a_lock, (a_error)=>{
        if (typeof a_cb === "function") {
          a_cb(a_error, false);
        }
      });
    } else if (a_error.unavailable){
      a_cb(undefined, true);
    } else {
      a_cb(a_error);
    }
  });
}


fcf.NLock.lockFile = (a_file, a_cb) => {
  libUtil.promisify(lockFile)(a_file, false)
  .then((a_res)=>{
    if (typeof a_cb === "function") {
      a_cb(undefined, a_res);
    }
    return a_res;
  })
  .catch((a_error)=>{
    if (typeof a_cb === "function") {
      a_cb(a_error);
    }
   });
};

fcf.NLock.tryLockFile = (a_file, a_quiet, a_cb) => {
  if (typeof a_quiet == "function") {
    a_cb = a_quiet;
    a_quiet = false;
  }
  libUtil.promisify(lockFile)(a_file, true)
  .then((a_res)=>{
    if (typeof a_cb === "function") {
      a_cb(undefined, a_res);
    }
    return a_res;
  })
  .catch((a_error)=>{
    if (typeof a_cb === "function") {
      if (a_quiet && a_error.unavailable){
        a_cb(undefined, undefined);
      } else {
        a_cb(a_error);
      }

    }
  });
};

fcf.NLock.unlockFile = (a_lock, a_cb) => {
  libUtil.promisify(unlockFile)(a_lock)
  .then((a_res)=>{
    if (typeof a_cb === "function") {
      a_cb(undefined, a_res);
    }
    return a_res;
  })
  .catch((a_error)=>{
    if (typeof a_cb === "function") {
      a_cb(a_error);
    }
  });

};

fcf.NLock.isLockFile = (a_file, a_cb) => {
  libUtil.promisify(islockFile)(a_file)
  .then((a_res)=>{
    if (typeof a_cb === "function") {
      a_cb(undefined, a_res);
    }
    return a_res;
  })
  .catch((a_error)=>{
    if (typeof a_cb === "function") {
      a_cb(a_error);
    }
  });
};


fcf.NLock.lockNamedMutex = (a_name, a_cb) => {
  let stack = (new Error()).stack;
  a_cb = typeof a_cb == "function" ? a_cb : ()=>{};
  if (typeof a_name != "string" || !a_name || a_name == "." || a_name == "..") {
    a_cb(new Error("The argument is not a string containing the name of mutex"));
    return;
  }
  new Promise((a_res, a_rej)=>{
    libLock.lockNamedMutex(a_name, getCahceDirectory(), (a_error, a_lock)=>{
      if (a_error) {
        a_rej(a_error);
      } else {
        a_res(a_lock);
      }
    });
  })
  .catch((a_error)=>{
    restoreStack(a_error, stack);
    a_cb(a_error);
  })
  .then((a_lock)=>{
    a_cb(undefined, a_lock);
  })
  .catch((a_error)=>{
  });
};

fcf.NLock.tryLockNamedMutex = (a_name, a_quiet, a_cb) => {
  if (typeof a_quiet == "function") {
    a_cb = a_quiet;
    a_quiet = false;
  }
  a_cb = typeof a_cb == "function" ? a_cb : ()=>{};
  if (typeof a_name != "string" || !a_name || a_name == "." || a_name == "..") {
    a_cb(new Error("The argument is not a string containing the name of mutex"));
    return;
  }
  let stack = (new Error()).stack;
  new Promise((a_res, a_rej)=>{
    libLock.trylockNamedMutex(a_name, getCahceDirectory(), (a_error, a_lock, a_unavailable)=>{
      if (a_unavailable) {
        a_error.unavailable = a_unavailable;
        if (a_quiet) {
          a_res(a_lock);
        } else {
          a_rej(a_error);
        }
      } else if (!a_error) {
        a_res(a_lock);
      } else {
        a_rej(a_error);
      }
    });
  })
  .catch((a_error)=>{
    restoreStack(a_error, stack);
    a_cb(a_error);
  })
  .then((a_lock)=>{
    a_cb(undefined, a_lock);
  })
  .catch((a_error)=>{
  });
};


fcf.NLock.unlockNamedMutex = (a_lock, a_cb) => {
  let stack = (new Error()).stack;
  a_cb = typeof a_cb == "function" ? a_cb : ()=>{};
  if (typeof a_lock != "number" || !a_lock) {
    a_cb(new Error("The argument is not a lock index containing the mutex name"));
    return;
  }
  new Promise((a_res, a_rej)=>{
    libLock.unlockNamedMutex(a_lock, (a_error, a_lock)=>{
      if (a_error) {
        a_rej(a_error);
      } else {
        a_res(a_lock);
      }
    });
  })
  .catch((a_error)=>{
    restoreStack(a_error, stack);
    a_cb(a_error);
  })
  .then((a_lock)=>{
    a_cb(undefined, a_lock);
  })
  .catch((a_error)=>{
  });
};


fcf.NLock.isLockNamedMutex = (a_name, a_cb) => {
  a_cb = typeof a_cb == "function" ? a_cb : ()=>{};
  fcf.NLock.tryLockNamedMutex(a_name, true, (a_error, a_lock) => {
    if (!a_error) {
      if (a_lock) {
        fcf.NLock.unlockNamedMutex(a_lock, (a_error)=>{
          a_cb(undefined, false);
        });
      } else {
        a_cb(undefined, true);
      }
    } else {
      a_cb(a_error);
    }
  });
};
