let libFS   = require("fs");
let libUtil = require("util");
let libLock = require("./build/Release/lock");

module.exports = {};

const stAfterClose = {};

function restoreStack(a_error, a_stack) {
  if (a_error.stack.split("\n").length < 2){
    a_error.stack = a_error.stack + a_stack.substr(a_stack.indexOf("\n"));
  }
}

function call(a_methodName, a_fd, a_restoreStack, a_cb){
  libLock[a_methodName](a_fd, (a_error, a_unavailable)=>{
    if (a_unavailable){
      a_error.unavailable = a_unavailable;
    }
    if (a_error){
      restoreStack(a_error, a_restoreStack);
      a_cb(a_error);
    } else {
      a_cb(undefined, a_unavailable ? undefined : a_fd);
    }
  });
}

function lock(a_file, a_try, a_cb) {
  let stack = (new Error()).stack;
  try {
    if (typeof a_file == "string") {
      libFS.open(a_file, "r", (a_error, a_fd)=>{
        if (a_error) {
          restoreStack(a_error, stack);
          a_cb(a_error);
        } else {
          call(a_try ? "trylock" : "lock", a_fd, stack, (a_error, a_lock)=>{
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
      call(a_try ? "trylock" : "lock", a_file, stack, a_cb);
    } else {
      throw new Error("The argument is not a lock file handle or a string containing the file path");
    }
  } catch(e) {
    a_cb(e);
  }
};

function unlock(a_lock, a_cb) {
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
    call("unlock", a_lock, stack, completeLock);
  } catch(e){
    completeLock(e);
  }
}

function islock(a_file, a_cb) {
  lock(a_file, true, (a_error, a_lock) => {
    if (!a_error) {
      unlock(a_lock, (a_error)=>{
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

module.exports["lockFile"] = (a_file, a_cb) => {
  libUtil.promisify(lock)(a_file, false)
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

module.exports["tryLockFile"] = (a_file, a_cb) => {
  libUtil.promisify(lock)(a_file, true)
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

module.exports["unlockFile"] = (a_lock, a_cb) => {
  libUtil.promisify(unlock)(a_lock)
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

module.exports["isLockFile"] = (a_file, a_cb) => {
  libUtil.promisify(islock)(a_file)
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


let fcf = typeof global !== 'undefined' && global.fcf ? global.fcf
                                                      : {};
if (typeof global !== 'undefined') {
  global.fcf = fcf;
}

if (!fcf.NLock) {
  fcf.NLock = {};
}

fcf.NLock.lockFile    = module.exports.lockFile;
fcf.NLock.tryLockFile = module.exports.tryLockFile;
fcf.NLock.unlockFile  = module.exports.unlockFile;
fcf.NLock.isLockFile  = module.exports.isLockFile;
