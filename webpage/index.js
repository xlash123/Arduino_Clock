function sendCommand(cmd, args) {
	const argsStr = '';
	args.forEach((a) => {
		argsStr += a + ' ';
	});
	argsStr = argsStr.substring(0, argsStr.length-1);
	$.ajax({
		contentType: 'text/plain',
		dataType: 'text/plain',
		data: cmd + ' ' + argsStr,
		success: (data, status) => {
			console.log(data);
		},
	});
}

$('#set-time-form').submit(function (event) {
	// Send command
	event.preventDefault();
});

console.log('script loaded');