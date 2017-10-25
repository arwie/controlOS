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


set('etc');
set('can');


function device($dev) { global $lib, $etc, $can, $out;
	static $dio = 64;
	
	$dev['readBit']  = $dev['readBit']  ?: [];
	$dev['writeBit'] = $dev['writeBit'] ?: [];
	
	foreach ($dev['readBit'] as &$bit) {
		$bit['offset'] = $bit['offset'] ?: 0;
		$bit['dio'] = $dio;
		$dio += $bit['size'];
	}
	foreach ($dev['writeBit'] as &$bit) {
		$bit['offset'] = $bit['offset'] ?: 0;
		$bit['dio'] = $dio;
		$dio += $bit['size'];
	}

	if (!$dev['simulated'])
		switch ($dev['bus']) {
			case 'etc': set('etc')[$dev['address']] = $dev; break;
			case 'can': set('can')[$dev['address']] = $dev; break;
		}
		
	set('device')[$lib['name']] = $dev;
	$lib['device'] = get('device')[$lib['name']];
	
	l('common shared '.$lib['name'].'_deviceAddress as const long = '.$dev['address']);
}


function deviceLibEnd($dev) { global $lib;
	l('public function '.$lib['name'].'_deviceRead(byval index_ as long, byval subindex_ as long) as long');
	ll($lib['name'].'_deviceRead = '.$dev['bus'].'_read('.$dev['address'].', index_, subindex_)');
	l('end function');
	l('public sub '.$lib['name'].'_deviceWrite(byval index_ as long, byval subindex_ as long, byval size_ as long, byval value_ as long)');
	ll('call '.$dev['bus'].'_write('.$dev['address'].', index_, subindex_, size_, value_)');
	l('end sub');
	
	l('public function '.$lib['name'].'_deviceReadString(byval index_ as long, byval subindex_ as long) as string');
	ll($lib['name'].'_deviceReadString = '.$dev['bus'].'_readString('.$dev['address'].', index_, subindex_)');
	l('end function');
	
	$accessBit = function($rwBit) { global $lib;
		ll('dim dio_    as long = -1');
		ll('dim offset_ as long');
		ll('dim size_   as long');
		ll('select case (index_ shl 16) + subindex_');
		foreach (get('device')[$lib['name']][$rwBit] as $bit) {
			ll('case 0x'.dechex(($bit['index'] << 16) + $bit['subindex']));
			lll('dio_    = '.$bit['dio']);
			lll('offset_ = '.$bit['offset']);
			lll('size_   = '.$bit['size']);
		}
		ll('end select');
		ll('if (dio_ > 0) and (bit_ >= offset_) and (bit_-offset_+range_ <= size_) then');
		if ($rwBit == 'readBit')
			lll($lib['name'].'_deviceReadBits = system.din[dio_+bit_-offset_][range_]');
		else
			lll('system.dout[dio_+bit_-offset_][range_] = value_');
		ll('else');
		lll('call logLibError("bitwise access to unassociated pdo")');
		lll('call log_putCodeFunc(scope)');
		lll('call com_putString("/index", hex$(index_)+"h")');
		lll('call com_putLong("/subindex", subindex_)');
		lll('call com_putLong("/bit", bit_)');
		lll('call com_putLong("/range", range_)');
		lll('call logSend');
		lll('throw errorApplication');
		ll('end if');
	};
	if ($dev['readBit']) {
		l('public function '.$lib['name'].'_deviceReadBit(byval index_ as long, byval subindex_ as long, byval bit_ as long) as long');
		ll($lib['name'].'_deviceReadBit = '.$lib['name'].'_deviceReadBits(index_, subindex_, bit_, 1)');
		l('end function');
		l('public function '.$lib['name'].'_deviceReadBits(byval index_ as long, byval subindex_ as long, byval bit_ as long, byval range_ as long) as long');
		$accessBit('readBit');
		l('end function');
	}
	if ($dev['writeBit']) {
		l('public sub '.$lib['name'].'_deviceWriteBit(byval index_ as long, byval subindex_ as long, byval bit_ as long, byval value_ as long)');
		ll('call '.$lib['name'].'_deviceWriteBits(index_, subindex_, bit_, 1, value_)');
		l('end sub');
		l('public sub '.$lib['name'].'_deviceWriteBits(byval index_ as long, byval subindex_ as long, byval bit_ as long, byval range_ as long, byval value_ as long)');
		$accessBit('writeBit');
		l('end sub');
	}
}










function io($dev, $simulated=false) {
	$dev['simulated'] = $simulated;
	device($dev);
}
