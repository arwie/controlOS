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



function &set($what) { global $real, $out;
	static $fake;
	
	if ($out) {
		$fake = $fake ?: $real;
		return $fake[$what];
	}
	
	$real[$what] = $real[$what] ?: [];
	return $real[$what];
}
function get($what)  { global $real;
	return $real[$what];
}


if (file_exists('/etc/app/sim.conf'))
	$sim = parse_ini_file('/etc/app/sim.conf') ?: true;


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


function includeLib($file, $type='app') { global $out, $libs, $simio, $axis, $robot, $drive, $sim;
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
includeLib('ETC.LIB', 'system');
includeLib('CAN.LIB', 'system');

include 'config.inc';

includeLib('DRIVELOG.LIB', 'system');
includeLib('SIMIO.LIB', 'system');
includeLib('DEBUG.LIB', 'system');
includeLib('SYSTEM.LIB', 'system');
includeLib('STATE.LIB', 'system');

ob_end_clean();



$out = $argv[1].'/SSMC/';
if (!is_dir($out)) mkdir($out);

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



if (file_exists('/etc/mc.conf')) {
	$mc = parse_ini_file('/etc/mc.conf');
	file_put_contents($argv[1].'/SN', <<<EOT
program
	sn "{$mc['sn']}"
end program
EOT
	);
	file_put_contents($argv[1].'/UAC', <<<EOT
program
	uac "{$mc['uac']}"
end program
EOT
	);
}
