var fs = require('fs');

var grab = require('../src/js/index');
console.log("init x11 interface");
const {width, height, depth} = grab.init();
console.log('screen dimension', width, height, depth);
console.log("trying to get frame");

var date = new Date();
var screenshot = grab.getImageSync();
console.log(screenshot);
console.log(screenshot.data.length);
console.log(new Date() - date);
var screenshot = grab.getImageSync(800, 600, 800, 600);
console.log(screenshot);
console.log(screenshot.data.length);
console.log(new Date() - date);

