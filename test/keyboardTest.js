var x11 = require("../index.js");
var keycode = "24" // A key 

console.log("initialize");
x11.init();
console.log("trying to send keycode");
x11.keyPress(keycode, true);
console.log("trying to relase key");
x11.keyPress(keycode, false);


