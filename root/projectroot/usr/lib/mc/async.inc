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


function async($prio='prgPriorityNormal')
{
	l('common shared '.lib().'_asyncBusy as long');
	l('dim shared asyncBuffer   as long');
	l('dim shared asyncNotify   as long');
	l('dim shared asyncFunction as string = "{}"');
	
	lib('init', function(){
		com_openFifo('asyncBuffer');
		com_openNotify('asyncNotify');
	});
	
	lib('stopTrigger', function(){
		ll('call com_clear');
		ll('call com_send(asyncNotify)');
		ll('call com_send(asyncBuffer)');
	});
	
	lib('stop', function(){
		ll('call com_reset(asyncBuffer)');
		ll(lib().'_asyncBusy = false');
		ll('asyncFunction = "{}"');
	});
	
	lib('debug', [
		'call com_putJson("asyncFunction", asyncFunction)',
		'call com_putLong("asyncBusy", '.lib().'_asyncBusy)',
	]);
	
	lib('end', function() use ($prio) {
		com_prgReceive('asyncBuffer', 250, $prio);
		ll('select case receive_');
		ll('case com_receiveMessage');
		lll('asyncFunction = com_getJson("")');
		lll('select case com_getLong("/cmd")');
		lll('case -1');
		llll('call com_send(asyncNotify)');
		foreach(lib('async')['functions'] as $f) {
			lll('case '.$f['cmd']);
			llll('call '.$f['nameSync'].$f['comGet']);
		}
		lll('end select');
		lll('asyncFunction = "{}"');
		lll('decrement('.lib().'_asyncBusy)');
		ll('end select');
		l('end sub');
	
		l('public sub '.lib().'_asyncWait');
		ll('if '.lib().'_asyncBusy then');
		lll('call com_reset(asyncNotify)');
		lll('if '.lib().'_active then');
		llll('call com_store');
		llll('call asyncExec(-1)');
		llll('while not com_receive(asyncNotify, 100) and '.lib().'_active');
		llll('end while');
		lll('end if');
		ll('end if');
		ll('if not '.lib().'_active then');
		lll('throw errorApplication');
		ll('end if');
		l('end sub');
	
		l('sub asyncExec(byval cmd_ as long)');
		ll('call com_putLong("/cmd", cmd_)');
		ll('call com_send(asyncBuffer)');
		ll('call com_restore');
		ll('increment('.lib().'_asyncBusy)');
		l('end sub');
	});
}


function async_function($name, $args=[], $pre=false, $post=false)
{
	foreach ($args as $arg=>$type) {
		if ($type) {
			$params[] = $paramsPure[] = 'byval '.$arg.' as '.(is_string($type)?$type:$type[0]);
			$paramsCall[] = $arg;
			$comPut[] = (is_string($type)?'com_put'.$type:$type[1]).'("/args/'.$arg.'", '.$arg.')';
			$comGet[] = (is_string($type)?'com_get'.$type:$type[2]).'("/args/'.$arg.'")';
		} else {
			$paramsPure[] = $arg;
		}
	}
	
	$pp = function($tokens) { return $tokens ? '('.implode(', ', $tokens).')' : ''; };
	
	$f = [
		'name'			=> $name,
		'nameSync'		=> $name.'_',
		'cmd'			=> count(lib('async')['functions']?:[]),
		'comGet'		=> $pp($comGet),
	];
	lib('async')['functions'][] = $f;
	
	l('public sub '.lib().'_'.$f['name'].$pp($paramsPure));
	call($pre);
	ll('if '.lib().'_started then');
	lll('call com_store');
	lll('call com_putString("/function", "'.$name.'")');
	foreach ($comPut?:[] as $p) lll($p);
	lll('call asyncExec('.$f['cmd'].')');
	if ($post) lll('call '.lib().'_asyncWait');
	ll('else');
	lll('call '.$f['nameSync'].$pp($paramsCall));
	ll('end if');
	call($post);
	l('end sub');
	
	l('sub '.$f['nameSync'].$pp($params));
}


function async_call($name) {
	echo 'call '.$name.'_';
}
