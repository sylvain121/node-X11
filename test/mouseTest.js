var x11 = require("../index.js");
var mouseCoord = {
    x: 50,
    y: 15
};
var mouseLeftButton = 1;

console.log("initialize");
x11.init();
console.log("trying to move mouse");
x11.mouseMove(mouseCoord.x, mouseCoord.y);
console.log("trying to do left click");
x11.mouseButton(mouseLeftButton, true);

setTimeout(function() {
    console.log("trying to release left click");
    x11.mouseButton(mouseLeftButton, false);
}, 10);
