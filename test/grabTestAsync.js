var fs = require('fs');
var jpg = require('jpeg-turbo');

var grab = require('../index.js');
console.log("init x11 interface");
grab.init();
console.log("trying to get frame");

var date = new Date();
grab.getImage(false, function(err, image){
	console.log(image);
	console.log("data size "+ image.data.length);
	console.log(new Date() - date);

	var options = {
		format: jpg.FORMAT_BGRA,
		width: image.width,
		height: image.height,
		subsampling: jpg.SAMP_444,

	}

	var preallocated = new Buffer(jpg.bufferSize(options))

	var encoded = jpg.compressSync(image.data, preallocated, options)
	fs.writeFileSync("out.jpg", encoded);

});

