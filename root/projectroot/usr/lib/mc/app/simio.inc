<? 
$simio = [];


function input($name, $type, $sim=false, $default=null) {
	$io = simio('in', $name, $type, $sim, $default);
	
	l('public function '.$io['name'].' as '.$type);
	ll('if not '.$io['ord'].' then');
	lll('call '.$io['get'].'('.$io['name'].')');
	ll('else');
	lll($io['name'].' = '.$io['app']);
	ll('end if');
	l('end function');
	l('public sub '.$io['get'].'(value_ as '.$type.')');
}


function output($name, $type, $sim=false, $default=null) {
	$io = simio('out', $name, $type, $sim, $default);
	
	l('public sub '.$io['name'].'(byval value_ as '.$type.')');
	ll('if not '.$io['ord'].' then');
	lll('call '.$io['set'].'(value_)');
	lll($io['app'].' = value_');
	lll($io['io'].'  = value_');
	ll('else');
	lll($io['app'].' = value_');
	ll('end if');
	l('end sub');
	l('public sub '.$io['set'].'(value_ as '.$type.')');
}


function simio($dir, $name, $type, $sim, $default) { global $simio, $lib, $out;
	static $id = 1;
	
	$name = $lib['name'].'_'.$name;
	
	$io = [
		'id'		=> $id,
		'dir'		=> $dir,
		'name'		=> $name,
		'type'		=> $type,
		'sim'		=> $sim,
		'default'	=> $default,
		'ord'		=> "{$name}O",
		'app'		=> "{$name}A",
		'io'		=> "{$name}I",
		'get'		=> "{$name}G",
		'set'		=> "{$name}S",
	];

	if (!$out) {
		$simio[$name] = $io;
		$id++;
	}

	return $io;
}
