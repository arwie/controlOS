<?

function libBegin($file, $type='app') { global $lib;
	$lib = [
		'file'	=> basename($file),
		'name'	=> strtolower(basename($file, '.LIB')),
		'type'	=> $type,
	];

	l('common shared '.$lib['name'].'_started   as long');
	l('common shared '.$lib['name'].'_prgActive as long');
}


function libComOpenServerWebsocket($name, $port, $args=[])	{ libComOpen($name, 'websocket', array_merge(['long("port", '.$port.')'], $args)); }
function libComOpenFifo($name, $args=[])					{ libComOpen($name, 'fifo', $args); }

function libComOpen($name, $type, $args=[]) { global $lib;
	ll('call com_clear');
	ll('call com_putString("log.lib",  "'.$lib['name'].'")');
	ll('call com_putString("log.name", "'.$name.'")');
	foreach ($args as $arg) {
		ll('call com_put'.ucfirst($arg)); 
	}
	ll($name.' = com_open(com_channel'.ucfirst($type).')');
}



function lib_init() { global $lib;
	$lib['init'] = true;
	l('sub libInit');
}


function lib_start() { global $lib;
	$lib['start'] = true;
	l('sub libStart');
}


function lib_stopTrigger() { global $lib;
	$lib['stopTrigger'] = true;
	l('sub libStopTrigger');
}


function lib_stop() { global $lib;
	$lib['stop'] = true;
	l('sub libStop');
}


function lib_prg($prio='prgPriorityNormal') { global $lib;
	$lib['prg']['file']	= strtoupper($lib['name'].'.PRG');
	$lib['prg']['prio'] = $prio;
	l('public sub '.$lib['name'].'_prg');
}
function lib_prgLoop($prio='prgPriorityNormal') { global $lib;
	$lib['prg']['loop'] = true;
	lib_prg($prio);
}


function lib_prgStart() { global $lib;
	$lib['prg']['start'] = true;
	l('public sub '.$lib['name'].'_prgStart');
}


function lib_prgStop() { global $lib;
	$lib['prg']['stop'] = true;
	l('public sub '.$lib['name'].'_prgStop');
}


function lib_prgEvent($scantime=1) { global $lib;
	$lib['prg']['event'] = $scantime;
	l('public sub '.$lib['name'].'_prgEvent');
}



function libEnd() { global $lib, $libs, $out;
	l('public sub '.$lib['name'].'_init');
	if ($lib['init']) ll('call libInit');
	if ($lib['prg']) {
		ll('load '.$lib['prg']['file'].' priority = '.$lib['prg']['prio']);
	}
	l('end sub');
		
	if ($lib['prg']) {
		l('public sub '.$lib['name'].'_start');
		ll('if not '.$lib['name'].'_started then');
		if ($lib['start']) lll('call libStart');
		lll($lib['name'].'_started = true');
		lll('call com_sendSelf(state_comStateServer)');
		lll($lib['name'].'_prgActive = true');
		lll('starttask '.$lib['prg']['file']);
		lll('call logLibInfo("'.$lib['name'].' lib started")');
		lll('call logSend');
		ll('end if');
		l('end sub');

		l('public sub '.$lib['name'].'_stopTrigger');
		ll($lib['name'].'_prgActive = false');
		if ($lib['stopTrigger']) ll('call libStopTrigger');
		l('end sub');

		l('public function '.$lib['name'].'_isRunning as long');
		ll($lib['name'].'_isRunning = '.$lib['name'].'_prgActive or not ('.$lib['prg']['file'].'.state = TASK_KILLED or '.$lib['prg']['file'].'.state = TASK_READY)');
		l('end function');

		l('public sub '.$lib['name'].'_wait');
		ll('dim true_ as long = true');
		ll('call '.$lib['name'].'_waitWhile(true_)');
		l('end sub');

		l('public sub '.$lib['name'].'_waitWhile(while_ as long)');
		ll('while '.$lib['name'].'_isRunning and while_');
		lll('sleep 10');
		ll('end while');
		l('end sub');

		l('public sub '.$lib['name'].'_stop');
		ll('if '.$lib['name'].'_started then');
		lll('call '.$lib['name'].'_stopTrigger');
		lll('call '.$lib['name'].'_wait');
		lll($lib['name'].'_started = false');
		lll('call com_sendSelf(state_comStateServer)');
		if ($lib['stop']) lll('call libStop');
		lll('call logLibInfo("'.$lib['name'].' lib stopped")');
		lll('call logSend');
		ll('end if');
		l('end sub');
	}

	foreach (['Error', 'Warning', 'Notice', 'Info', 'Debug'] as $prio) {
		l('sub logLib'.$prio.'(byval message as string)');
		ll('call logLib(log_prio'.$prio.', message)');
		l('end sub');
	}
	l('sub logLib(byval prio as long, message as string)');
	ll('call com_store');
	ll('call log_messageRef(prio, message)');
	ll('call com_putString("lib", "'.$lib['name'].'")');
	l('end sub');

	if ($lib['prg']) {
		l('sub logPrgError');
		ll('call logLibError('.$lib['prg']['file'].'.error)');
		ll('call com_putLong("error", '.$lib['prg']['file'].'.errornumber)');
		l('end sub');
	}

	l('sub logSend');
	ll('call log_send');
	ll('call com_restore');
	l('end sub');

	if ($lib['device'])
		deviceLibEnd($lib['device']);

	if ($lib['axis'])
		axisLibEnd($lib['axis']);

	if ($lib['robot'])
		robotLibEnd($lib['robot']);
	
	if (!$out) {
		$libs['all'][]			= $lib;
		$libs[$lib['type']][]	= $lib;
	}
}
