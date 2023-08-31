let libLock = require("../index.js");
let libUtil = require("util");
let libFS   = require("fs");

function invalidTest(a_message){
  a_message = a_message || "Invalid test";
  throw new Error(a_message);
}

async function main(){
  /*
  for(let i = 0; i < 10000000; ++i){
    let lock = await libUtil.promisify(libLock.lockFile)("index.js");
    await libUtil.promisify(libLock.isLockFile)("index.js");
    await libUtil.promisify(libLock.unlockFile)(lock);
    await libUtil.promisify(libLock.isLockFile)("index.js");
  };
  */

  // lockFile (by path) && unlockFile
  //
  {
    let start = Date.now();
    let promises = [];
    for(let i = 0; i < 10; ++i) {
      promises.push(new Promise((a_res, a_rej) => {
        libLock.lockFile("index.js", (a_error, a_lock)=>{
          setTimeout(()=>{
            libLock.unlockFile(a_lock, (a_error)=>{
              a_res();
            });
          }, 100);
        })
      }));
    }

    await Promise.all(promises);
    let diff = Date.now() - start;
    if (diff < 1000 || diff > 1200){
      invalidTest(`Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
    }
  }

  // lockFile (by descriptor) && unlockFile
  //
  {
    let start = Date.now();
    let promises = [];
    for(let i = 0; i < 10; ++i) {
      promises.push(new Promise((a_res, a_rej) => {
        libFS.open("index.js", "r", (a_error, a_fd) => {
          libLock.lockFile(a_fd, (a_error, a_lock)=>{
             setTimeout(()=>{
              libLock.unlockFile(a_lock, (a_error)=>{

                libFS.close(a_fd);
                a_res();
              });
            }, 100);
          });
        });
      }));
    }

    await Promise.all(promises);
    let diff = Date.now() - start;
    if (diff < 1000 || diff > 1200){
      invalidTest(`Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
    }
  }

  // tryLockFile (by descriptor) && unlockFile
  //
  {
    let isFail = false;
    let lock = await libUtil.promisify(libLock.lockFile)("index.js");
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js");
      isFail = true;
    } catch(e){
      if (!e.unavailable) {
        isFail = true;
      }
    }
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js1231231313123");
      isFail = true;
    } catch(e){
      if (e.unavailable) {
        isFail = true;
      }
    }
    await libUtil.promisify(libLock.unlockFile)(lock);
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js");
      await libUtil.promisify(libLock.unlockFile)(lock2);
    } catch(e){
      isFail = true;
    }
    if (isFail){
      invalidTest("Function tryLockFile has invalid logic");
    }
  }

  // isLockFile (by descriptor) && unlockFile
  //
  {
    let lock = await libUtil.promisify(libLock.lockFile)("index.js");
    if (!(await libUtil.promisify(libLock.isLockFile)("index.js"))){
      invalidTest("Function isLockFile has invalid logic");
    }
    await libUtil.promisify(libLock.unlockFile)(lock);
    if (await libUtil.promisify(libLock.isLockFile)("index.js")){
      invalidTest("Function isLockFile has invalid logic");
    }
  }

}

main();
