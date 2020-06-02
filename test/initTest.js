var fs = require('fs');

var grab = require('../src/js/index');
console.log("init x11 interface");
const { width, height, depth } = grab.init();
console.log('screen dimension', width, height, depth);


