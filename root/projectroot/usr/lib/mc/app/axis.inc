<?

$axis = [];


function axis($ax, $simulated=false) { global $axis, $lib, $out;
	static $elementid = 1;

	$ax['elementid']	= $elementid;
	$ax['name']			= $lib['name'];

	$ax['simulated']	= $simulated;
	
	$ax['setup']		= $ax['setup']		?: strtoupper($ax['name']).'.DAT';
	$ax['firmware']		= $ax['firmware']	?: strtoupper($ax['device']).'.O';
	
	$ax['drive']		= parseDriveSetup($ax);

	$lib['axis']		= $ax;
	if ($ax['bus'])
		device($ax);
	
	if (!$out) {
		$axis[$ax['name']]	= $ax;
		$elementid++;
	}
}


function axis_setup() { global $lib;
	$lib['axis']['axisSetup'] = true;
	l('sub axisSetup');
}


function axisLibEnd($ax) { global $lib;
	l('public sub '.$lib['name'].'_axisSetup');
	ll('attach '.$ax['name']);
	defaultAxisSetup($ax);
	if ($lib['axis']['axisSetup']) ll('call axisSetup');
	ll('detach '.$ax['name']);
	l('end sub');

	l('public sub '.$lib['name'].'_enable');
	ll('while not '.$ax['name'].'.enable');
	lll($ax['name'].'.enable = true');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('sleep 5*bus[0].cycletime/1000');	#bug in MC
	l('end sub');

	l('public sub '.$lib['name'].'_disable');
	ll('while '.$ax['name'].'.isMoving');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('while '.$ax['name'].'.enable');
	lll($ax['name'].'.enable = false');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	l('end sub');
}



function parseDriveSetup(&$ax) { global $drive;
	switch ($ax['device']) {
		case 'cdhd':
			foreach (file($ax['setup'], FILE_USE_INCLUDE_PATH) as $line) {
				$line = explode(' ', preg_replace('/<.*>$/', '', trim($line)));
				$drive[$line[0]] = $line[1];
			}
			break;
	}
	return $drive;
}


function defaultAxisSetup(&$ax) { global $drive;
	switch ($ax['device']) {
		case 'cdhd':
			ll($ax['name'].'.positionerrordelay			= 4.11');
			ll($ax['name'].'.motionbusvelocityscale		= 0');
			ll($ax['name'].'.motionbusvelocitybase		= 1000');
			break;
		case 'stepim':
			ll($ax['name'].'.positionerrordelay			= 5');
			ll($ax['name'].'.motionbusvelocityscale		= 0');
			ll($ax['name'].'.motionbusvelocitybase		= 1000');
			break;
	}
	ll($ax['name'].'.smoothFactor				= -1');
	if ($ax['simulated']) {
		ll($ax['name'].'.positionerrordelay			= 1');
	}
}

