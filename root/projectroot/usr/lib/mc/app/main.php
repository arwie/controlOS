<?

function l($basic)		{ echo $basic."\n"; }
function ll($basic)		{ echo "\t".$basic."\n"; }
function lll($basic)	{ echo "\t\t".$basic."\n"; }
function llll($basic)	{ echo "\t\t\t".$basic."\n"; }
function lllll($basic)	{ echo "\t\t\t\t".$basic."\n"; }


require 'lib.inc';
require 'device.inc';
require 'axis.inc';
require 'robot.inc';
require 'simio.inc';


function includeLib($file, $type='app') { global $out, $libs, $simio, $axis, $robot, $can, $etc, $drive, $sim;
	libBegin($file, $type);
	l("'--------------------");
	l('');
	include $file;
	l('');
	l("'--------------------");
	libEnd();
}


ob_start();

includeLib('LOG.LIB', 'system');
includeLib('BASIC.LIB', 'system');
includeLib('ETC.LIB', 'system');
includeLib('CAN.LIB', 'system');

include 'config.inc';

includeLib('DRIVELOG.LIB', 'system');
includeLib('SIMIO.LIB', 'system');
includeLib('DEBUG.LIB', 'system');
includeLib('SYSTEM.LIB', 'system');
includeLib('STATE.LIB', 'system');

ob_end_clean();



$out = $argv[1].'/';

foreach ($libs['all'] as $lib) {
	ob_start();
	includeLib($lib['file'], $lib['type']);
	file_put_contents($out.$lib['file'], ob_get_contents()); ob_end_clean();
	
	if ($lib['prg']) {
		ob_start();
		include 'libPrg.inc';
		file_put_contents($out.$lib['prg']['file'], ob_get_contents()); ob_end_clean();
	}
}

foreach(glob('*.PRG') as $file) {
	ob_start();
	include $file;
	file_put_contents($out.$file, ob_get_contents()); ob_end_clean();
}
