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


function conf($file, $section=null, $key=null, $default=null) {
	static $conf = null;
	
	if (is_null($conf)) {
		$conf = [];
		foreach (glob('/etc/app/*.conf') as $file) {
			$conf[basename($file, '.conf')] = parse_ini_file($file, true);
		}
	}
	
	if (is_null($section))
		return array_key_exists($file, $conf);
	
	if (is_null($key))
		return array_key_exists($file, $conf) && array_key_exists($section, $conf[$file]); 
	
	return (array_key_exists($file, $conf) && array_key_exists($section, $conf[$file]) && array_key_exists($key, $conf[$file][$section])) ? $conf[$file][$section][$key] : $default;
}
