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


require 'shared.inc';


function &set($what, $set=true) { global $out;
	static $real, $fake;
	
	if ($out && $set) 
		return $fake;
	
	$real[$what] = $real[$what] ?: [];
	return $real[$what];
}
function get($what) {
	return set($what, false);
}


function common($name, $type='long', $init=null) {
	set('common')[] = [
		'lib'	=> lib(),
		'name'	=> $name,
		'type'	=> $type,
		'init'	=> $init,
	];
}


require 'lib.inc';
require 'device.inc';
require 'axis.inc';
require 'robot.inc';
require 'simio.inc';


function includeLib($file, $from=null, $type='app') {
	libBegin($file, $from, $type);
	l("'--------------------");
	l('');
	include $from ?: $file;
	l('');
	l("'--------------------");
	libEnd();
}


ob_start();

includeLib('LOG.LIB', null, 'system');
includeLib('ETC.LIB', null, 'system');
includeLib('CAN.LIB', null, 'system');

include 'config.inc';

includeLib('DRIVELOG.LIB', null, 'system');
includeLib('SIMIO.LIB',    null, 'system');
includeLib('DEBUG.LIB',    null, 'system');
includeLib('SYSTEM.LIB',   null, 'system');
includeLib('STATE.LIB',    null, 'system');

ob_end_clean();



$out = $argv[1].'/SSMC/';
if (!is_dir($out)) mkdir($out);

foreach (get('libs')['all'] as $lib) {
	ob_start();
	includeLib($lib['file'], $lib['from'], $lib['type']);
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
