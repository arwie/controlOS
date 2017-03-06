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


$etc = [];
$can = [];


function device($dev) { global $lib, $etc, $can, $out;
	$lib['device'] = $dev;
	
	if (!$out) {
		if (!$dev['simulated'])
			switch ($dev['bus']) {
				case 'etc': $etc[$dev['address']] = $dev; break;
				case 'can': $can[$dev['address']] = $dev; break;
			}
	}
	
	l('common shared '.$lib['name'].'_deviceAddress as const long = '.$dev['address']);
}


function deviceLibEnd($dev) { global $lib;
	l('public function '.$lib['name'].'_deviceRead(byval index_ as long, byval subindex_ as long) as long');
	ll($lib['name'].'_deviceRead = '.$dev['bus'].'_read('.$dev['address'].', index_, subindex_)');
	l('end function');
	l('public sub '.$lib['name'].'_deviceWrite(byval index_ as long, byval subindex_ as long, byval size_ as long, byval value_ as long)');
	ll('call '.$dev['bus'].'_write('.$dev['address'].', index_, subindex_, size_, value_)');
	l('end sub');
}










function io($dev, $simulated=false) {
	$dev['simulated'] = $simulated;
	device($dev);
}
