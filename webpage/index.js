function sendCommand(cmd, args) {
	console.log('sendCommand');
	let argsStr = '';
	args.forEach((a) => {
		argsStr += encodeURI(a) + ' ';
	});
	if (argsStr.length > 0) argsStr = argsStr.substring(0, argsStr.length-1);
	$.ajax({
		contentType: 'text/plain',
		dataType: 'text/plain',
		data: cmd + ' ' + argsStr,
		method: 'POST',
		success: (data, status) => {
			console.log(data);
		},
	});
}

function parseTime(time) {
	const idx = time.indexOf(':');
	const hours = time.substring(0, idx);
	const minutes = time.substring(idx+1);
	return hours*60*60 + minutes*60;
}

$('#set-time-form').submit(function (event) {
	event.preventDefault();
	console.log('submit');
	const days = $('#alarm-days').val();
	let daysBin = 0;
	days.forEach((day) => {
		daysBin |= 1 < day;
	});
	sendCommand('newalarm', [
		$('#alarm-name').val(),
		$('#alarm-msg').val(),
		parseTime($('#alarm-time').val()),
		daysBin,
		$('#alarm-skip').val(),
		'default',
		$('#alarm-persist')[0].checked,
	]);
});

function getAlarms() {
	// Do get request for alarms
	// Place into <ul>
}

console.log('script loaded');