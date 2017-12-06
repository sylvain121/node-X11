var x11 = require("../index.js");
//var keycode = 65; // A key 
var keycode = 58;
console.log("initialize");
x11.init();
console.log("trying to send keycode");
x11.keyPressWithKeysym(keycode, true);
console.log("trying to relase key");
x11.keyPressWithKeysym(keycode, false);


