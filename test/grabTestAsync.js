var fs = require('fs');

var grab = require('../src/js/index');
console.log("init x11 interface");
grab.init();
console.log("trying to get frame");

setInterval(function() {
	var date = new Date();
	grab.getImage(false, function(err, image){
		console.log(image);
		console.log("data size "+ image.data.length);
		console.log(new Date() - date);

	});
}, 30)

