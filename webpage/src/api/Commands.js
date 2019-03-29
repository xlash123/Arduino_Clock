function combine(){
	var args = arguments;
	var ret = "";
	for(var i=0; i<args.length; i++){
		ret += args[0] + ' ';
	}
	return ret.substring(0, ret.length-1);
}

export newalarm = function(name, message, time, repeat, skipWeek, skipInterval, sound, persistent){
	var rep = 0;
	for(var i=0; i<repeat.length; i++){
		rep = rep | (1 << repeat[i]);
	}
	if(skipWeek) rep = rep | (1 << 8);
	return combine('newalarm', message, time.getHours()*60*60 + time.getMinutes()*60 + time.getSeconds(), 
		rep, skipInterval, sound, persistent);
}