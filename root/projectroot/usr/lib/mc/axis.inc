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


$axis = [];


function axis($ax, $simulated=false) { global $axis, $lib, $out;
	static $elementid = 1;

	$ax['elementid']	= $elementid;
	$ax['name']			= $lib['name'];

	$ax['simulated']	= $simulated;
	
	$ax['setup']		= $ax['setup']		?: strtoupper($ax['name']).'.DAT';
	$ax['firmware']		= $ax['firmware']	?: strtoupper($ax['device']).'.O';
	
	$ax['drive']		= parseDriveSetup($ax);

	switch ($ax['device']) {
		case 'cdhd':
			$ax['readBit']  = [['index'=>0x60FD, 'subindex'=>0, 'size'=>32]];
			//$ax['writeBit'] = [['index'=>0x60FE, 'subindex'=>1, 'size'=>6,  'offset'=>16]];
			break;
	}

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
	l('public sub '.$lib['name'].'_enableAttach');
	ll('call '.$lib['name'].'_enable');
	ll('attach '.$ax['name']);
	l('end sub');

	l('public sub '.$lib['name'].'_disable');
	ll('while '.$ax['name'].'.isMoving > 0');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('while '.$ax['name'].'.enable');
	lll($ax['name'].'.enable = false');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	l('end sub');
	l('public sub '.$lib['name'].'_disableDetach');
	ll('detach '.$ax['name']);
	ll('call '.$lib['name'].'_disable');
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
		ll($ax['name'].'.positionerrordelay			= 4');
	}
}
