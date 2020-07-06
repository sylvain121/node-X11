var fs = require('fs');
var jpeg = require('jpeg-js');

var grab = require('../src/js/index');
console.log("init x11 interface");
const { width, height, depth } = grab.init();
console.log('screen dimension', width, height, depth);
console.log("trying to get frame");

var date = new Date();
var screenshot = grab.getMouseCursorSync();
console.log(screenshot);
console.log(new Date() - date);
console.log(screenshot.data.length);
/*fs.writeFileSync('./test.jpg', jpeg.encode({
    width: screenshot.width * 2,
    height: screenshot.height,
    data: screenshot.data
}).data);*/

printRGBA(screenshot.data, screenshot.width * 2, screenshot.height);
console.log(screenshot.data.length);



function printRGBA(data, width, height) {
    for (let y = 0; y < height * width *4; y = y + width * 4) {
        for (let i = 0; i < width * 4; i = i + 4) {
            const char = (data.readUInt32LE(y + i) > 0) ? "#" : ".";
            process.stdout.write(char);
        }
        process.stdout.write('\n');
    }
}

console.log("width", screenshot.width, "height",screenshot.height);

setInterval(()=> {
    grab.getImageSync();
    console.log(grab.getMouseCursorSync());
}, 30)