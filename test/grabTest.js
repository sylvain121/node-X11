var fs = require('fs');

var grab = require('../src/js/index');
console.log("init x11 interface");
const { width, height, depth } = grab.init();
console.log('screen dimension', width, height, depth);
console.log("trying to get frame");

setInterval(function(){
    var date = new Date();
    var screenshot = grab.getImageSync();
    console.log(screenshot);
    delete screenshot;
    console.log(new Date() - date);
    console.log('mem : ', process.memoryUsage());
}, 30)

