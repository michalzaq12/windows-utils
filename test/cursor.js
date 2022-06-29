const { showCursor } = require('../index');


showCursor(false)

setTimeout(() => {
	showCursor(true)
}, 5000)