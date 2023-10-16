let libFS   = require("fs");
let libOS   = require("os");
let libUtil = require("util");
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
  return (libOS.tmpdir() + "/fcf-framework/namedmutex").replace(/\\/g, "/").replace(/[\/]+/g, "/");
}

async function prepareCahceDirectory() {
  if (libOS.platform() != "android") {
    return;
  }
  let pathArrOrig = getCahceDirectory().split("/");
  let pathArr     = [...pathArrOrig];
  while(pathArr.length){
    let path = pathArr.join("/");
    let state;
    try {
      state = await libUtil.promisify(libFS.stat)(path);
    } catch(e){
    }
    if (state && state.isDirectory()) {
      break;
    } else {
      pathArr.pop();
    }
  }
  while(pathArr.length != pathArrOrig.length) {
    pathArr.push(pathArrOrig[pathArr.length]);
    let path = pathArr.join("/");
    try {
      await libUtil.promisify(libFS.mkdir)(path);
    } catch(error){
      let state;
      try {
        state = await libUtil.promisify(libFS.stat)(path);
      } catch(stateError){
      }
      if (!state || !state.isDirectory()) {
        throw error;
      }
    }
  }
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

function lockNamedMutex(a_name, a_try, a_cb) {
  let stack = (new Error()).stack;
  try {
    if (typeof a_name == "string") {
      if (libOS.platform() == "android") {
        a_name = getCahceDirectory() + "/"+ a_name;
      }
      call(a_try ? "trylockNamedMutex" : "lockNamedMutex", a_name, stack, a_cb);
    } else {
      throw new Error("The argument is not a string containing the name of mutex");
    }
  } catch(e) {
    a_cb(e);
  }
};

function unLockNamedMutex(a_lock, a_cb) {
  let stack = (new Error()).stack;
  try {
    if (typeof a_lock == "number") {
      if (!a_lock){
        throw new Error("Invalid mutex lock id.");
      }
      call("unlockNamedMutex", a_lock, stack, a_cb);
    } else {
      throw new Error("The argument is not a number the index of the mutex.");
    }
  } catch(e) {
    a_cb(e);
  }
};

function isLockNamedMutex(a_name, a_cb) {
  lockNamedMutex(a_name, true, (a_error, a_lock) => {
    if (!a_error) {
      unLockNamedMutex(a_lock, (a_error)=>{
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

fcf.NLock.tryLockFile = (a_file, a_quiet, a_cb) => {
  if (typeof a_quiet == "function") {
    a_cb = a_quiet;
    a_quiet = false;
  }
  libUtil.promisify(lock)(a_file, true)
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

fcf.NLock.isLockFile = (a_file, a_cb) => {
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


fcf.NLock.lockNamedMutex = (a_name, a_cb) => {
  prepareCahceDirectory()
  .then(()=>{
    return libUtil.promisify(lockNamedMutex)(a_name, false)
  })
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

fcf.NLock.tryLockNamedMutex = (a_file, a_quiet, a_cb) => {
  if (typeof a_quiet == "function") {
    a_cb = a_quiet;
    a_quiet = false;
  }
  prepareCahceDirectory()
  .then(()=>{
    return libUtil.promisify(lockNamedMutex)(a_file, true)
  })
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


fcf.NLock.unlockNamedMutex = (a_lock, a_cb) => {
  libUtil.promisify(unLockNamedMutex)(a_lock)
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

fcf.NLock.isLockNamedMutex = (a_name, a_cb) => {
  prepareCahceDirectory()
  .then(()=>{
    return libUtil.promisify(isLockNamedMutex)(a_name)
  })
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
