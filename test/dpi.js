const { setDPI } = require('../index');


setDPI(2)

setTimeout(() => {
	setDPI(0)
}, 5000)