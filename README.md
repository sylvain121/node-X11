# node-X11
nodejs binding that allow screen capture, mouse and keyboard interaction
```bash
sudo apt-get install libxtst-dev
```

## API

### `init(String: "display_name")`
 initialize connection to specified display ( default use current );

### `getImage()`

 return main screen picture with specifics attributes 

 ```js

var x11 = require("node_X11");

x11.init() // connect on current user screen see $DISPLAY in shell

var screenshot = x11.getImage();

console.log(screenshot.width); // image width ->Number;
console.log(screenshot.height); // image height -> Number;
console.log(screenshot.depth); // image depth -> Number;
console.log(screenshot.data); // image raw pixel -> Buffer;

 ```

### `keyPress(Number: keycode, Boolean: isDown)`

 simulate keyPress for the specified keycode 
### `mouseMove(Number: x, Number: y)`

 move mouse to specified coord
### `mouseButton(Number: button, Boolean: isDown)`

 allowed buttons values are ->  1: LEFT, 2: MIDDLE, 3: RIGHT



