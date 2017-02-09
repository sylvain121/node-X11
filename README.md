# node-X11
nodejs binding that allow screen capture, mouse and keyboard interaction

sudo apt-get install libxtst-dev

exposed method : 

init(String: "display_name") => initialize connection to specified display ( default use current );

getImage() => return main screen picture with specifqique attribute :
width : image width
height: image height
depth: image depth
data : image raw buffer

keyPress(Number: keycode, Boolean: isDown) => simulate keyPress for the specified keycode 
mouseMove(Number: x, Number: y) => move mouse to specified coord
mouseButton(Number: button, Boolean: isDown) => allowed button value are 1: LEFT, 2: MIDDLE, 3: RIGHT



