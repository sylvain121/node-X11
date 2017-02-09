var fs = require('fs');
var jpg = require('jpeg-turbo');

var grab = require('../index.js');
console.log("init x11 interface");
grab.init();

var oc = 10;
var i = 0;
var timer = setInterval(function() {

    console.log("trying to get frame");
    var date = new Date();
    var screenshot = grab.getImage();
    console.log(new Date() - date);

if(oc === i) clearInterval(timer);
i++;

}, 1000 / 30);
