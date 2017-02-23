<?

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
