let libLock = require("../index.js");
let libUtil = require("util");
let libFS   = require("fs");
let libOS   = require("os");

function main() {
  let lock;
  new Promise((a_res, a_rej)=>{
    a_res();
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockFile("index.js", (a_error, a_lock)=>{
        if (a_error) {
          a_rej(a_error);
        } else {
          lock = a_lock;
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockFile("index.js", (a_error, a_islock)=>{
        if (a_error) {
          a_rej(a_error);
        } else if (!a_islock) {
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.unlockFile(lock, (a_error)=>{
        if (a_error) {
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockFile("index.js", (a_error, a_islock)=>{
        if (a_error) {
          a_rej(a_error);
        } else if (a_islock) {
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  // mutex
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.lockNamedMutex("index.js", (a_error, a_lock)=>{
        if (a_error) {
          a_rej(a_error);
        } else {
          lock = a_lock;
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockNamedMutex("index.js", (a_error, a_islock)=>{
        if (a_error) {
          a_rej(a_error);
        } else if (!a_islock) {
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
        if (a_error) {
          a_rej(a_error);
        } else {
          a_res();
        }
      });
    });
  })
  .then(()=>{
    return new Promise((a_res, a_rej)=>{
      libLock.isLockNamedMutex("index.js", (a_error, a_islock)=>{
        if (a_error) {
          a_rej(a_error);
        } else if (a_islock) {
          a_rej(new Error("Invalid logic"));
        } else {
          a_res();
        }
      });
    });
  })
  .catch((a_error)=>{
    console.log(a_error);
    process.exit(1);
  })
  .then(()=>{
    console.log(new Date())
    main();
  });
}

main();
