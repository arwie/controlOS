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


function axis($ax) {
	static $elementid = 1;

	$ax['elementid']	= $elementid++;
	$ax['name']			= lib();
	
	$ax['setup']		= $ax['setup']		?: strtoupper($ax['name']).'.DAT';
	$ax['firmware']		= $ax['firmware']	?: strtoupper($ax['device']).'.O';

	switch ($ax['device']) {
		case 'cdhd':
			$ax['readBit']  = [['index'=>0x60FD, 'subindex'=>0, 'size'=>32]];
			//$ax['writeBit'] = [['index'=>0x60FE, 'subindex'=>1, 'size'=>6,  'offset'=>16]];
			break;
	}

	device($ax);
	
	lib('debug', [
		'call com_putBool("enable", '.$ax['name'].'.enable)',
		'call com_putDouble("positionFeedback", '.$ax['name'].'.positionFeedback)',
		'call com_putDouble("torqueFeedback", '.$ax['name'].'.torqueFeedback)',
	]);

	lib('end', function() use ($ax) {
		_axisSetup($ax);

		l('public sub '.lib().'_enable');
			ll('while not '.lib().'.enable');
			lll(lib().'.enable = true');
			lll('sleep bus[0].cycletime/1000');
			ll('end while');
			ll('sleep 5*bus[0].cycletime/1000');	#bug in MC
		l('end sub');
		l('public sub '.lib().'_enableAttach');
			ll('call '.lib().'_enable');
			ll('attach '.lib());
		l('end sub');

		l('public sub '.lib().'_disable');
			ll('while '.lib().'.isMoving > 0');
			lll('sleep bus[0].cycletime/1000');
			ll('end while');
			ll('while '.lib().'.enable');
			lll(lib().'.enable = false');
			lll('sleep bus[0].cycletime/1000');
			ll('end while');
		l('end sub');
		l('public sub '.lib().'_disableDetach');
			ll('detach '.lib());
			ll('call '.lib().'_disable');
		l('end sub');
	});
}


function drive_update($hash) {
	lib('driveUpdate')['hash'][] = $hash;
	lib('driveUpdate', 'call driveUpdate');
	l('sub driveUpdate');
}

function driveCache($expr, $type='double') {
	$hash = hashsum([$expr, $type]);
	lib('driveCache', 'call com_put'.$type.'("/'.$hash.'", '.$expr.')');
	return 'com_get'.$type.'("/'.$hash.'")';
}

function drive_setup() {
	lib('driveSetup', 'call driveSetup');
	l('sub driveSetup');
}

function axis_setup() {
	lib('axisSetup', 'call axisSetup');
	l('sub axisSetup');
}



function _axisSetup($ax) {
	l('public sub '.lib().'_axisSetup');
	ll('dim driveUpdate_ as long = true');
	ll('dim driveCache_ as long');
	ll('dim driveHash_ as string');
	
	ll('with '.lib()); #withGlobal
	ll('attach');
	
	if (!$ax['simulated'])
	{
		com_openFile('driveCache_', '/home/mc/'.lib().'.drivecache');
		ll('driveHash_ = '.s(hashsum([libGet('driveUpdate'), libGet('driveCache')])).'+str$('.lib().'_deviceRead(0x1018, 4))');
		ll('try');
		lll('if com_receive(driveCache_, 0) then');
			llll('driveUpdate_ = driveHash_ <> com_getString("/#")');
		lll('end if');
		catchLog();
		ll('if driveUpdate_ then');
			logInfo([lib().' drive parameters update']);
			
			lll('call com_clear');
			_defaultDriveUpdate($ax);
			switch ($ax['device']) {
				case 'cdhd':
					lll('call '.lib().'_deviceWrite(0x1010, 1, 32, 0x65766173)');	#SAVE ALL
					lll('call '.lib().'_deviceWrite(0x2002, 0, 8, 1)');				#CONFIG
				break;case 'stepim':
					lll('call '.lib().'_deviceWrite(0x1010, 1, 32, 0x65766173)');	#SAVE
			}
			call(libGet('driveCache'));
			lll('call com_putString("/#", driveHash_)');
			lll('call com_send(driveCache_)');
			
		ll('end if');
		ll('call com_close(driveCache_)');
		
		_defaultDriveSetup($ax);
	}
	
	_defaultAxisSetup($ax);
	
	ll('detach');
	ll('end with');
	l('end sub');
}


function _defaultDriveUpdate($ax) { lib('driveUpdate')['hash'][] = 4;
	switch ($ax['device']) {
		case 'cdhd':
		break;case 'stepim':
	}
	call(libGet('driveUpdate'));
}


function _defaultDriveSetup($ax) {
	ll('call '.lib().'_deviceWrite(0x6060, 0, 8, 8)');					#Modes of Operation: 8 = cyclic synchronous position mode

	ll('motionbusvelocityscale		= 0');
	ll('motionbusvelocitybase		= 1000');
	
	switch ($ax['device']) {
		case 'cdhd':
			ll('call '.lib().'_deviceWrite(0x60C0, 0, 16, 2)');			#Interpolation: 2 = Cubic interpolation with position only – strict
			ll('positionfactor				= '.driveCache(lib().'_deviceRead(0x6092, 1)/'.lib().'_deviceRead(0x6092, 2)', 'double'));
			ll('positionerrordelay			= 4.11');
		break;case 'stepim':
			ll('positionfactor				= 4096');
			ll('positionerrordelay			= 5');
	}
	call(libGet('driveSetup'));
}


function _defaultAxisSetup($ax) {
	if ($ax['simulated']) {
		ll('positionerrordelay		= 4');
		ll('positionerrormax		= MAXDOUBLE');
	}
	
	ll('velocityfactor				= positionfactor / 1000');
	ll('accelerationfactor			= positionfactor / 1000000');
	ll('jerkfactor					= positionfactor / 1000000000');
	ll('velocityexternalfactor		= positionexternalfactor / 1000');
	
	ll('smoothFactor				= -1');
	
	call(libGet('axisSetup'));
}

