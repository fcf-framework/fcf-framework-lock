let libLock = require("../index.js");
let libUtil = require("util");
let libFS   = require("fs");
let libOS   = require("os");

function invalidTest(a_message){
  a_message = a_message || "Invalid test";
  throw new Error(a_message);
}

async function main(){
  // while(true){
  //   {
  //     let lock = await libUtil.promisify(libLock.lockFile)("index.js");
  //     await libUtil.promisify(libLock.isLockFile)("index.js");
  //     await libUtil.promisify(libLock.unlockFile)(lock);
  //     await libUtil.promisify(libLock.isLockFile)("index.js");
  //   }
  //   {
  //     let lock = await libUtil.promisify(libLock.lockNamedMutex)("index.js");
  //     await libUtil.promisify(libLock.isLockNamedMutex)("index.js");
  //     await libUtil.promisify(libLock.unlockNamedMutex)(lock);
  //     await libUtil.promisify(libLock.isLockNamedMutex)("index.js");
  //   }
  // };

  // lockFile (by path) && unlockFile
  //
  {
    let start = Date.now();
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

    await Promise.all(promises);
    let diff = Date.now() - start;
    if (diff < 1000 || diff > 1200){
      invalidTest(`(1) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
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
          if (a_error){
            invalidTest(a_error);
          }
          libLock.lockFile(a_fd, (a_error, a_lock)=>{
              if (a_error){
                invalidTest(a_error);
              }
             setTimeout(()=>{
              libLock.unlockFile(a_lock, (a_error)=>{
                if (a_error){
                  invalidTest(a_error);
                }
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
      invalidTest(`(2) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
    }
  }

  // tryLockFile (by descriptor) && unlockFile
  //
  {
    let isFail = false;
    let lock = await libUtil.promisify(libLock.lockFile)("index.js");
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js");
      isFail = 1;
    } catch(e){
      if (!e.unavailable) {
        isFail = 2;
      }
    }
    let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js", true);
    if (lock2 !== undefined) {
      isFail = 3;
    }
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js1231231313123");
      isFail = 4;
    } catch(e){
      if (e.unavailable) {
        isFail = 5;
      }
    }
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js1231231313123", true);
      isFail = 6;
    } catch(e){
      if (e.unavailable) {
        isFail = 7;
      }
    }
    await libUtil.promisify(libLock.unlockFile)(lock);
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js");
      await libUtil.promisify(libLock.unlockFile)(lock2);
    } catch(e){
      isFail = 8;
    }
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockFile)("index.js", true);
      await libUtil.promisify(libLock.unlockFile)(lock2);
    } catch(e){
      isFail = 9;
    }
    if (isFail){
      invalidTest(`Function tryLockFile has invalid logic: ${isFail}`);
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


  // lockNamedMutex && unlockNamedMutex
  //
  {
    let start = Date.now();
    let promises = [];
    for(let i = 0; i < 10; ++i) {
      promises.push(new Promise((a_res, a_rej) => {
        libLock.lockNamedMutex("FCF_TEST_MUTEX", (a_error, a_lock)=>{
          if (a_error){
            invalidTest(a_error);
          }
          setTimeout(()=>{
            libLock.unlockNamedMutex(a_lock, (a_error)=>{
              if (a_error){
                invalidTest(a_error);
              }
              a_res();
            });
          }, 100);
        })
      }));
    }

    await Promise.all(promises);
    let diff = Date.now() - start;
    if (diff < 1000 || diff > 1200){
      invalidTest(`(3) Lock test on file did not complete in due time (diff < 1000 || diff > 1200)  [diff: ${diff}]`);
    }
  }



  // tryLockNamedMutex && unlockNamedMutex
  //
  {
    let mutexName = "FCF_TEST_MUTEX";
    let errorMutexName = "FCF_TEST_MUTEX/@#$%^&*()_111";
    errorMutexName = errorMutexName.padEnd(99999,".");
    let isFail = false;
    let lock = await libUtil.promisify(libLock.lockNamedMutex)(mutexName);
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName);
      isFail = true;
    } catch(e){
      if (!e.unavailable) {
        isFail = true;
      }
    }
    let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName, true);
    if (lock2 !== undefined) {
      isFail = true;
    }
    if (libOS.platform != "win32") {
      try {
        let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(errorMutexName);
        isFail = true;
      } catch(e){
        if (e.unavailable) {
          isFail = true;
        }
      }
      try {
        let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(errorMutexName, true);
        isFail = true;
      } catch(e){
        if (e.unavailable) {
          isFail = true;
        }
      }
    }
    await libUtil.promisify(libLock.unlockNamedMutex)(lock);
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName);
      if (!lock2){
        isFail = true;
      }
      let lock3 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName, true);
      if (lock3){
        isFail = true;
      }
      await libUtil.promisify(libLock.unlockNamedMutex)(lock2);
    } catch(e){
      isFail = true;
    }
    try {
      let lock2 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName, true);
      if (!lock2){
        isFail = true;
      }
      let lock3 = await libUtil.promisify(libLock.tryLockNamedMutex)(mutexName, true);
      if (lock3){
        isFail = true;
      }
      await libUtil.promisify(libLock.unlockNamedMutex)(lock2);
    } catch(e){
      isFail = true;
    }
    if (isFail){
      invalidTest("Function tryLockNamedMutex has invalid logic [Line: " + isFail + "]");
    }
  }

    // isLockNamedMutex && unlockNamedMutex
    //
    {
      let mutexName = "FCF_TEST_MUTEX";
      if ((await libUtil.promisify(libLock.isLockNamedMutex)(mutexName))){
        invalidTest("Function isLockFile has invalid logic");
      }
      let lock = await libUtil.promisify(libLock.lockNamedMutex)(mutexName);
      if (!(await libUtil.promisify(libLock.isLockNamedMutex)(mutexName))){
        invalidTest("Function isLockFile has invalid logic");
      }
      await libUtil.promisify(libLock.unlockNamedMutex)(lock);
      if (await libUtil.promisify(libLock.isLockNamedMutex)(mutexName)){
        invalidTest("Function isLockFile has invalid logic");
      }
    }

    {
      let mutexName1 = "FCF_TEST_MUTEX1";
      let mutexName2 = "FCF_TEST_MUTEX2";

      await libUtil.promisify(libLock.lockNamedMutex)(mutexName1);
      await libUtil.promisify(libLock.lockNamedMutex)(mutexName2);

      if (!(await libUtil.promisify(libLock.isLockNamedMutex)(mutexName1))){
        invalidTest("Function isLockFile has invalid logic");
      }
      if (!(await libUtil.promisify(libLock.isLockNamedMutex)(mutexName2))){
        invalidTest("Function isLockFile has invalid logic");
      }

    }

    {
      await libUtil.promisify(libLock.lockNamedMutex)("123/:\\123@#$%^&*()");
    }

  console.log("Complete");

}

main();
