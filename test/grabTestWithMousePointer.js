var fs = require('fs');
var jpg = require('jpeg-turbo');

var grab = require('../index.js');
console.log("init x11 interface");
grab.init();
console.log("trying to get frame");

var date = new Date();
var withPointer = true;
var screenshot = grab.getImage(withPointer);
console.log(screenshot);

console.log("capture time : "+(new Date() - date));

var options = {
  format: jpg.FORMAT_BGRA,
  width: screenshot.width,
  height: screenshot.height,
  subsampling: jpg.SAMP_444,

}

var preallocated = new Buffer(jpg.bufferSize(options))

var encoded = jpg.compressSync(screenshot.data, preallocated, options)
fs.writeFileSync("out.jpg", encoded);

