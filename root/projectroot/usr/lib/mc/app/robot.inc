<?

$robot = [];


function scara($a1, $a2, $a3, $a4) {
	return [
		'axes'		=> [$a1, $a2, $a3, $a4],
		'model'		=> 4,
		'type'		=> 'xyzr',
	];
}


function robot($rbt) { global $robot, $lib, $out;
	
	$rbt['name']			= $lib['name'];
		
	$lib['robot']			= $rbt;

	if (!$out) {
		$robot[$rbt['name']]	= $rbt;
	}
}


function robot_setup() { global $lib;
	$lib['robot']['robotSetup'] = true;
	l('sub robotSetup');
}


function robotLibEnd($rbt) { global $lib;
	l('public sub '.$lib['name'].'_robotSetup');
	ll('attach '.$rbt['name']);
	defaultRobotSetup($rbt);
	if ($lib['robot']['robotSetup']) ll('call robotSetup');
	ll('detach '.$rbt['name']);
	l('end sub');

	l('public sub '.$lib['name'].'_enable');
	ll('while not '.$rbt['name'].'.enable');
	lll($rbt['name'].'.enable = true');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('sleep 5*bus[0].cycletime/1000');	#bug in MC
	l('end sub');

	l('public sub '.$lib['name'].'_disable');
	ll('while '.$rbt['name'].'.isMoving');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	ll('while '.$rbt['name'].'.enable');
	lll($rbt['name'].'.enable = false');
	lll('sleep bus[0].cycletime/1000');
	ll('end while');
	l('end sub');
}


function defaultRobotSetup(&$rbt) {
	foreach ($rbt['axes'] as $ax) { $j += 1; $jnt = $rbt['name'].'.j'.$j;
		ll($jnt.'.positionmin			= '.$ax.'.positionmin');
		ll($jnt.'.positionmax			= '.$ax.'.positionmax');
	}
}
