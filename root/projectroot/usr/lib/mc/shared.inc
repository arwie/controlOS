<?// Copyright (c) 2018 Artur Wiebe <artur@4wiebe.de>
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



function conf($file=null, $section=null, $key=null, $default=null) {
	static $conf = null;
	
	if (is_null($conf)) {
		$conf = [];
		foreach (glob('/etc/app/*.conf') as $path) {
			$conf[basename($path, '.conf')] = parse_ini_file($path, true);
		}
	}
	
	if (is_null($file))
		return $conf;
	
	if (is_null($section))
		return array_key_exists($file, $conf);
	
	if (is_null($key))
		return array_key_exists($file, $conf) && array_key_exists($section, $conf[$file]); 
	
	return (array_key_exists($file, $conf) && array_key_exists($section, $conf[$file]) && array_key_exists($key, $conf[$file][$section])) ? $conf[$file][$section][$key] : $default;
}


function setup($section=null, $key=null) {
	static $setup = null;
	
	if (is_null($setup)) {
		$dir = '/usr/share/setup/'; $ext = '.json';
		$type = conf('setup','setup','type');
		$rev  = conf('setup','setup','revision');
		$decode = function($name) use ($dir, $ext) { return json_decode(file_get_contents($dir.$name.$ext), true); };
		
		$setup = array_replace_recursive($decode('setup'), $decode($type));
		
		$patches = [];
		foreach (glob($dir.$type.'-*'.$ext) as $path) 
			if (preg_match("/.*-(\d*)\..*/", $path, $match))
				if (intval($match[1]) >= intval($rev))
					$patches[] = $match[1];
		rsort($patches, SORT_NUMERIC);
		foreach ($patches as $patch)
			$setup = array_replace_recursive($setup, $decode($type.'-'.$patch));
		
		$setup = array_replace_recursive($setup, conf()['setup']);
	}
	
	return is_null($section) ? $setup : (is_null($key) ? $setup[$section] : $setup[$section][$key]);
}


function debug() {
	return file_exists('/etc/app/debug');
}


function l($basic)		{ echo $basic."\n"; }
function ll($basic)		{ echo "\t".$basic."\n"; }
function lll($basic)	{ echo "\t\t".$basic."\n"; }
function llll($basic)	{ echo "\t\t\t".$basic."\n"; }
function lllll($basic)	{ echo "\t\t\t\t".$basic."\n"; }


function dump($var, $caption='dump') {
	file_put_contents('php://stderr', $caption.":\n".print_r($var, true)."\n\n");
}