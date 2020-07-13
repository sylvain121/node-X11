var fs = require('fs');
var jpeg = require('jpeg-js');

var grab = require('../src/js/index');
console.log("init x11 interface");
const {width, height, depth} = grab.init();
console.log('screen dimension', width, height, depth);
console.log("trying to get frame");

var date = new Date();
var screenshot = grab.getImageSync();
console.log(screenshot);
fs.writeFileSync('./test.jpg', jpeg.encode({
    width: screenshot.width,
    height: screenshot.height,
    data: screenshot.data
}).data)
console.log(screenshot.data.length);
console.log(new Date() - date);
setInterval(() => {

var screenshot = grab.getImageSync(1920, 0, 1920, 1080);
}, 15);
/*fs.writeFileSync('./test1.jpg', jpeg.encode({
    width: screenshot.width,
    height: screenshot.height,
    data: screenshot.data
}).data);
console.log(screenshot);
console.log(screenshot.data.length);
console.log(new Date() - date);*/

