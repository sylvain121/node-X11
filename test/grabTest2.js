var fs = require('fs');

var grab = require('../index.js');
console.log("init x11 interface");
grab.init();

var count = 0;

var timer = setInterval(function() {


    console.log("trying to get frame : " + count++);
    var date = new Date();
    var screenshot = grab.getImage();
    console.log(new Date() - date);
}, 1000 / 60);
