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


function libBegin($file, $from, $type) { global $lib;
	$lib = [
		'file'	=> basename($file),
		'name'	=> strtolower(basename($file, '.LIB')),
		'from'	=> $from,
		'type'	=> $type,
	];

	l('common shared '.$lib['name'].'_active  as long');
	l('dim shared libActiveNotify as long');
}

function &lib($what='name') { global $lib;
	return $lib[$what];
}

function libPartsAdd($where, $part) {
	lib($where)[] = $part;
}
function libPartsExec($where, $reverse=false) {
	$parts = lib($where)?:[];
	foreach ($reverse?array_reverse($parts):$parts as $part) {
		if (is_string($part)) ll($part); else $part();
	}
}



function engage($libs) {
	$libs = array_filter($libs);
	libPartsAdd('start', function() use ($libs) {
		foreach ($libs as $l) lll('call '.$l.'_start');
	});
	$libs = array_reverse($libs);
	libPartsAdd('stop', function() use ($libs) {
		foreach ($libs as $l) lll('call '.$l.'_stopTrigger');
		foreach ($libs as $l) lll('call '.$l.'_stop');
	});
}

function waitActive($lib, $while='true', $interval=100) {
	lll('while '.lib().'_active and not '.$lib.'_wait('.$interval.') and ('.$while.')');
	lll('end while');
	if ($lib != lib())
		libPartsAdd('stopTrigger', 'call '.$lib.'_stopTrigger');
}
function engageActive($lib, $while='true', $interval=100) {
	lll('call '.$lib.'_start');
	waitActive($lib, $while, $interval);
	lll('call '.$lib.'_stop');
}

function logMessage($prio, $message, $report=false, $log=false) {
	ll('call log_message('.$prio.', "'.$message.'")');
	if ($report)
		ll('call log_putReport("'.$report.'")');
	if ($log)
		$log();
	ll('call log_putCodeFunc(scope)');
	ll('call com_putString("/lib", "'.lib().'")');
	ll('call '.lib().'_putDebug');
	ll('call log_send');
}
function logCritical($message, $report=false, $log=false) { logMessage('log_prioCritical', $message, $report, $log); }
function logError   ($message, $report=false, $log=false) { logMessage('log_prioError',    $message, $report, $log); }
function logWarning ($message, $report=false, $log=false) { logMessage('log_prioWarning',  $message, $report, $log); }
function logNotice  ($message, $report=false, $log=false) { logMessage('log_prioNotice',   $message, $report, $log); }
function logInfo    ($message, $report=false, $log=false) { logMessage('log_prioInfo',     $message, $report, $log); }
function logDebug   ($message, $report=false, $log=false) { logMessage('log_prioDebug',    $message, $report, $log); }

function throwErrorApplication($message, $report=false, $log=false) {
	logError($message, $report, $log);
	ll('throw errorApplication');
}

function end_try($finally=false) {
	ll('catch errorApplication.num');
	ll('catch else');
	lll('call logLibError(error)');
	lll('call com_putLong("/error", errornumber)');
	lll('call log_putCodeFunc(scope)');
	lll('call logSend');
	if ($finally) {
		ll('finally');
		$finally();
	}
	ll('end try');
}



function lib_init() {
	libPartsAdd('init', 'call libInit');
	l('sub libInit');
}


function lib_start() {
	libPartsAdd('start', 'call libStart');
	l('sub libStart');
}


function lib_stopTrigger() {
	libPartsAdd('stopTrigger', 'call libStopTrigger');
	l('sub libStopTrigger');
}


function lib_stop() {
	libPartsAdd('stop', 'call libStop');
	l('sub libStop');
}


function lib_prg($prio='prgPriorityNormal') {
	lib('prg')['name'] = lib();
	lib('prg')['file'] = strtoupper(lib().'.PRG');
	lib('prg')['prio'] = $prio;
	l('public sub '.lib().'_prg');
}
function lib_prgPoll($interval, $prio='prgPriorityNormal') {
	lib_prg($prio);
	ll('do');
	lll('call prgLoop');
	if ($interval)
		ll('loop until com_receive(libActiveNotify, '.$interval.')');
	else
		ll('loop while '.lib().'_active');
	l('end sub');
	l('sub prgLoop');
}
function lib_prgLoop($prio='prgPriorityNormal') {
	lib_prgPoll(false, $prio);
}

function lib_prgStart() {
	libPartsAdd('prgStart', 'call libPrgStart');
	l('sub libPrgStart');
}

function lib_prgStop() {
	libPartsAdd('prgStop', 'call libPrgStop');
	l('sub libPrgStop');
}

function lib_prgEvent($scantime=5) { global $lib;
	$lib['prg']['event'] = $scantime;
	l('public sub '.$lib['name'].'_prgEvent');
}

function lib_prgError() { global $lib;
	$lib['prg']['error'] = true;
	l('public sub '.$lib['name'].'_prgError');
}

function lib_prgThread($name, $prio='prgPriorityNormal') { global $lib;
	$thread = [
		'thread'	=> true,
		'name'		=> $name,
		'file'		=> strtoupper($name.'.PRG'),
		'prio'		=> $prio,
	];
	$lib['prg']['threads'][] = $thread;
	
	l('sub '.$name.'Start');
		ll('starttask '.$thread['file']);
	l('end sub');
	l('sub '.$name.'Wait');
		ll('while ('.$thread['file'].'.state band 0xF) = TASK_RUNNING');
		lll('sleep 1');
		ll('end while');
		ll('killtask '.$thread['file']);
	l('end sub');
	l('public sub '.$lib['name'].'_'.$name);
}


function lib_debug() {
	libPartsAdd('debug', 'call libPutDebug');
	l('sub libPutDebug');
}


function lib_systemError($errors) { global $lib;
	$lib['systemError'] = $errors;
	l('public sub '.$lib['name'].'_systemError');
	ll('call logLibError(system.error)');
	ll('call com_putLong("/error", system.errornumber)');
	ll('call log_putCodeFunc(scope)');
	ll('call logSend');
	ll('call libSystemError');
	l('end sub');
	l('sub libSystemError');
}



function libEnd() { global $lib;
	libPartsExec('end');
	
	l('public sub '.$lib['name'].'_init');
	com_openNotify('libActiveNotify');
	if ($lib['prg'] && $lib['prg']['prio']) {
		ll('load '.$lib['prg']['file'].' priority = '.$lib['prg']['prio']);
		foreach ($lib['prg']['threads']?:[] as $thread)
			ll('load '.$thread['file'].' priority = '.$thread['prio']);
	}
	libPartsExec('init');
	l('end sub');
	
	l('public sub '.$lib['name'].'_start');
	ll('if not '.$lib['name'].'_started then');
	libPartsExec('start');
	lll($lib['name'].'_started = true');
	lll('call com_sendSelf(state_websocket)');
	lll($lib['name'].'_active = true');
	lll('call com_reset(libActiveNotify)');
	if ($lib['prg']) {
		lll('starttask '.$lib['prg']['file']);
	}
	lll('call logLibInfo("'.$lib['name'].' lib started")');
	lll('call logSend');
	ll('end if');
	l('end sub');

	l('public sub '.lib().'_stopTrigger');
	ll('if '.lib().'_active then');
	lll(lib().'_active = false');
	lll('call com_send(libActiveNotify)');
	libPartsExec('stopTrigger', true);
	ll('end if');
	l('end sub');

	l('public function '.$lib['name'].'_wait(byval timeout_ as long) as long');
	ll($lib['name'].'_wait = com_receive(libActiveNotify, timeout_)');
	l('end function');

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
	lll('call com_sendSelf(state_websocket)');
	libPartsExec('stop', true);
	lll('call logLibInfo("'.$lib['name'].' lib stopped")');
	lll('call logSend');
	ll('end if');
	l('end sub');
	
	l('public sub '.lib().'_putDebug');
	ll('call com_with("/debug/'.lib().'/")');
	ll('call com_putBool("started", '.lib().'_started)');
	ll('call com_putBool("active", '.lib().'_active)');
	libPartsExec('debug');
	l('end sub');

	foreach (['Error', 'Warning', 'Notice', 'Info', 'Debug'] as $prio) {
		l('sub logLib'.$prio.'(byval message as string)');
		ll('call log_message(log_prio'.$prio.', message)');
		l('end sub');
	}
	l('sub logSend');
	ll('call com_putString("/lib", "'.$lib['name'].'")');
	ll('call '.lib().'_putDebug');
	ll('call log_send');
	l('end sub');

	l('sub sleepActive(byval milliseconds_ as long)');
	ll('dim dummy_ as long = com_receive(libActiveNotify, milliseconds_)');
	l('end sub');

	if (lib('prg'))
	{
		l('public sub '.lib().'_prgStart');
			logDebug(lib().' prg started', false, function(){
				ll('call com_putString("/prg", "'.lib().'")');
			});
			libPartsExec('prgStart');
		l('end sub');
		
		l('public sub '.lib().'_prgStop');
			libPartsExec('prgStop', true);
		l('end sub');
		
		l('function prgRunning as long');
		ll('prgRunning = (('.$lib['prg']['file'].'.state band 0xF) = TASK_RUNNING)');
		l('end function');
		l('sub prgExit');
		ll('idletask '.$lib['prg']['file']);
		l('end sub');
	}

	if ($lib['device'])
		deviceLibEnd($lib['device']);

	if ($lib['axis'])
		axisLibEnd($lib['axis']);

	if ($lib['robot'])
		robotLibEnd($lib['robot']);
	
	set('libs')['all'][]		= $lib;
	set('libs')[$lib['type']][]	= $lib;
}



function libPrg($lib, $thread) {
?>
program
	onError
		catch else
			call log_message(<?=$lib['prg']['error']?'log_prioError':'log_prioCritical'?>, <?=$thread['file']?>.error)
			call com_putLong("/error", <?=$thread['file']?>.errornumber)
			'call com_putString("/trace", backtrace$("<?=$thread['file']?>"))
			call com_putLong("/state", <?=$thread['file']?>.state)
			call log_putCodeFunc("onError")
			call com_putString("/prg", "<?=$thread['name']?>")
			call com_putString("/lib", "<?=$lib['name']?>")
			call log_send
<?if ($lib['prg']['error']) {?>
			call <?=$lib['name']?>_prgError
<?} elseif (!debug()) {?>
			call system_stopTrigger
<?}?>
	end onError


<?if ($thread['thread']) {?>

	call <?=$lib['name']?>_<?=$thread['name']?>

<?} else {?>

<?if ($lib['prg']['event']) {?>
	onevent <?=$lib['name']?>_prgEvent scantime=<?=$lib['prg']['event']?> priority=<?=$lib['prg']['prio']?>-1
		call <?=$lib['name']?>_prgEvent
	end onevent
<?}?>

	call <?=$lib['name']?>_prgStart

<?if ($lib['prg']['event']) {?>
	eventon <?=$lib['name']?>_prgEvent
<?}?>

	if <?=$lib['name']?>_active then
		call <?=$lib['name']?>_prg
		call <?=$lib['name']?>_stopTrigger
	end if

<?if ($lib['prg']['event']) {?>
	eventoff <?=$lib['name']?>_prgEvent
<?}?>

	call <?=$lib['name']?>_prgStop

<?}?>

end program
<?
}
