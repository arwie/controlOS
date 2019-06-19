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


function scara($a1, $a2, $a3, $a4) {
	return [
		'axes'		=> [$a1, $a2, $a3, $a4],
		'model'		=> 4,
		'type'		=> 'xyzr',
	];
}


function robot($rbt) { global $lib;
	
	$rbt['name']			= $lib['name'];
		
	$lib['robot']			= $rbt;

	set('robot')[$rbt['name']]	= $rbt;

	lib('debug', [
		'com_putBool("enable",	enable)',
		'com_putJoint'.ucfirst($rbt['type']).'("positionFeedback", positionFeedback)',
	]);
}


function robot_setup() { global $lib;
	$lib['robot']['robotSetup'] = true;
	l('sub robotSetup');
}


function robotLibEnd($rbt) { global $lib;
	l('public sub '.lib().'_robotSetup');
	ll('attach');
	defaultRobotSetup($rbt);
	if ($lib['robot']['robotSetup']) ll('robotSetup()');
	ll('detach');
	l('end sub');

	l('public sub '.lib().'_enable');
	ll('while not enable');
	lll('enable = true');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	l('end sub');
	l('public sub '.lib().'_enableAttach');
	ll(lib().'_enable()');
	ll('attach');
	l('end sub');

	l('public sub '.lib().'_disable');
	ll('while isMoving > 0');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('enable = false');
	ll('sleep 5*bus[0].cycletime/1000');
	l('end sub');
	l('public sub '.lib().'_disableDetach');
	ll('detach');
	ll(lib().'_disable()');
	l('end sub');
}


function defaultRobotSetup(&$rbt) {
	foreach ($rbt['axes'] as $ax) { $j += 1; $jnt = 'j'.$j;
		ll($jnt.'.positionmin			= '.$ax.'.positionmin');
		ll($jnt.'.positionmax			= '.$ax.'.positionmax');
	}
}
