var fs = require('fs');

var grab = require('../src/js/index');
console.log("init x11 interface");
grab.init();
console.log("trying to get frame");

setInterval(function(){
    var date = new Date();
    var screenshot = grab.getImageSync();
    console.log(screenshot);

    console.log(new Date() - date);
}, 30)

