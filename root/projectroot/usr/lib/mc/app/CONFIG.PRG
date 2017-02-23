
system.numberaxes = <?=count($axis)?> 

<?
foreach($robot as $rbt) {
	foreach ($rbt['axes'] as $ax)
		$axisname .= ' axisname=a'.$axis[$ax]['elementid'];
	l('common shared '.$rbt['name'].' as group '.$axisname.' model='.$rbt['model'].' of '.$rbt['type']);
}
?>

<?
foreach($simio as $io) {
	l('common shared '.$io['ord'].' as long = '.($io['sim'] ? '1' : '0'));
	l('common shared '.$io['app'].' as '.$io['type'].($io['default']!=null ? ' = '.($io['type']=='string' ? '"'.$io['default'].'"' : $io['default']) : ''));
	l('common shared '.$io['io'].' as '.$io['type']);
}
?>

common shared state_comStateServer as long


program

	system.doubleformat	= 1

<?
	foreach($axis as $ax)
		ll('a'.$ax['elementid'].'.axisname = '.$ax['name']);
?>

if instr(version, "SIM") then
	load SIMMC.LIB
else
	load ETHERCAT.LIB
	load CANDS402.LIB
	load EC_CDHD.LIB
	load ISMAPPER.LIB
end if
	
	oload MCCOM.O
	load  MCCOM.LIB

<?
	foreach ($libs['all'] as $l)
		ll('load '.$l['file']);
?>

end program
