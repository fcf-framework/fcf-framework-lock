var mod = require("./tests_promise.js");
mod.promise.then(()=>{
  try {
    require("./tests_async.js");
  } catch(e) {
  };
})
