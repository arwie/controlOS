<?// Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


function libBegin($file, $type='app') { global $lib;
	$lib = [
		'file'	=> basename($file),
		'name'	=> strtolower(basename($file, '.LIB')),
		'type'	=> $type,
	];

	l('common shared '.$lib['name'].'_started as long');
	l('common shared '.$lib['name'].'_active  as long');
	l('dim shared libActiveNotify as long');
}


function libComOpenServerWebsocket($name, $port, $args=[])	{ libComOpen($name, 'websocket', array_merge(['long("/port", '.$port.')'], $args)); }
function libComOpenFifo($name, $args=[])					{ libComOpen($name, 'fifo', $args); }

function libComOpen($name, $type, $args=[]) { global $lib;
	ll('call com_clear');
	ll('call com_putString("/log/lib",  "'.$lib['name'].'")');
	ll('call com_putString("/log/name", "'.$name.'")');
	foreach ($args as $arg) {
		ll('call com_put'.ucfirst($arg)); 
	}
	ll($name.' = com_open(com_channel'.ucfirst($type).')');
}



function end_try($finally=false) {
	l('catch else');
	lll('call logLibError(error)');
	lll('call com_putLong("/error", errornumber)');
	lll('call log_putCodeFunc(scope)');
	lll('call logSend');
	if ($finally) {
		ll('finally');
		lll($finally);
	}
	ll('end try');
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


function lib_prgError() { global $lib;
	$lib['prg']['error'] = true;
	l('public sub '.$lib['name'].'_prgError');
}



function libEnd() { global $lib, $libs, $out;
	l('public sub '.$lib['name'].'_init');
	libComOpen('libActiveNotify', 'notify');
	if ($lib['init']) ll('call libInit');
	if ($lib['prg']) {
		ll('load '.$lib['prg']['file'].' priority = '.$lib['prg']['prio']);
	}
	l('end sub');
		
	l('public sub '.$lib['name'].'_start');
	ll('if not '.$lib['name'].'_started then');
	if ($lib['start']) lll('call libStart');
	lll($lib['name'].'_started = true');
	lll('call com_sendSelf(state_comStateServer)');
	lll($lib['name'].'_active = true');
	lll('call com_reset(libActiveNotify)');
	if ($lib['prg']) {
		lll('starttask '.$lib['prg']['file']);
	}
	lll('call logLibInfo("'.$lib['name'].' lib started")');
	lll('call logSend');
	ll('end if');
	l('end sub');

	l('public sub '.$lib['name'].'_stopTrigger');
	ll($lib['name'].'_active = false');
	lll('com_send(libActiveNotify)');
	if ($lib['stopTrigger']) ll('call libStopTrigger');
	l('end sub');

	l('public function '.$lib['name'].'_wait(byval timeout_ as long) as long');
	ll($lib['name'].'_wait = com_receive(libActiveNotify, timeout_)');
	l('end function');
	l('public sub '.$lib['name'].'_waitWhile(while_ as long)');
	ll('while not com_receive(libActiveNotify, 100) and while_');
	ll('end while');
	l('end sub');

	l('public sub '.$lib['name'].'_stop');
	ll('if '.$lib['name'].'_started then');
	lll('call '.$lib['name'].'_stopTrigger');
	if ($lib['prg']) {
		lll('while prgRunning');
		llll('sleep 1');
		lll('end while');
		lll('killtask '.$lib['prg']['file']);
	}
	lll($lib['name'].'_started = false');
	lll('call com_sendSelf(state_comStateServer)');
	if ($lib['stop']) lll('call libStop');
	lll('call logLibInfo("'.$lib['name'].' lib stopped")');
	lll('call logSend');
	ll('end if');
	l('end sub');

	foreach (['Error', 'Warning', 'Notice', 'Info', 'Debug'] as $prio) {
		l('sub logLib'.$prio.'(byval message as string)');
		ll('call logLib(log_prio'.$prio.', message)');
		l('end sub');
	}
	l('sub logLib(byval prio as long, message as string)');
	ll('call com_store');
	ll('call log_messageRef(prio, message)');
	ll('call com_putString("/lib", "'.$lib['name'].'")');
	l('end sub');

	l('sub logSend');
	ll('call log_send');
	ll('call com_restore');
	l('end sub');

	l('sub sleepActive(byval milliseconds_ as long)');
	ll('dim dummy_ as long = com_receive(libActiveNotify, milliseconds_)');
	l('end sub');

	if ($lib['prg']) {
		l('function prgRunning as long');
		ll('prgRunning = ('.$lib['prg']['file'].'.state = TASK_RUNNING)');
		l('end function');
	}

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
