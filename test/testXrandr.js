const grab = require('../src/js/index');

grab.init();
console.log("====== get current resolution");
console.log(grab.getCurrentResolution());
console.log("======= get screen resolution");
console.log(grab.getScreenResolutions());
console.log("======= change resolution");
console.log(grab.setResolution(27));