program continue

<? 

$baseProperties  = [
'absolute',
'profilerType',
'smoothFactor',
'blendingMethod',
'blendingFactor',
'velocityMax',
'velocityCruise',
'velocityOverride',
'accelerationMax',
'acceleration',
'decelerationMax',
'deceleration',
'decelerationStop',
'jerkMax',
'jerk',
'starttype',
'stoptype',
'rescuemode',
'positionErrorMax',
'positionErrorSettle',
'positionErrorDelay',
'velocityFactor',
'accelerationFactor',
'jerkFactor',
'dynamicModel',
];

$jointProperties  = array_merge($baseProperties, [
'axisType',
'direction',
'displacement',
'positionFactor',
'torqueFactor',
'positionMax',
'positionMaxEnable',
'positionMin',
'positionMinEnable',
'positionRollover',
'positionRolloverMin',
'positionRollOverEnable',
'velocityOverspeed',
'motionbusVelocityScale',
'motionbusVelocityBase',
'viscousFriction',
'coulombFriction',
'slaveDisconnect',
]);

$axesProperties  = array_merge($jointProperties, [
'enable',
'simulated',
'positionexternalfactor',
'directionexternal',
]);


$groupProperties = array_merge($baseProperties, [
'enable',
'rMin',
'rMax',
'xMin',
'xMax',
'yMin',
'yMax',
'zMin',
'zMax',
'velocityMaxTrans',
'velocityMaxRot',
'velocityTrans',
'velocityRot',
'accelerationMaxTrans',
'accelerationMaxRot',
'accelerationTrans',
'accelerationRot',
'decelerationTrans',
'decelerationRot',
'jerkMaxTrans',
'jerkMaxRot',
'jerkTrans',
'jerkRot',
'coupled',
'orientationFollowing',
'inertiaReduction',
'inertiaThreshold',
]);


function dumpObject($object, $properties) {
?>
	print "##########"
<?
	foreach ($properties as $property) {
		printProperty($object, $property);
	}
}

function printProperty($object, $property) {
?>
	print "<?=str_pad("$object.$property", 32)?> = "; <?="$object.$property"?> 
<?
}

function printMatrix($object, $property, $n, $m) {
	for ($a = 1; $a <= $n; $a++) {
		for ($b = 1; $b <= $m; $b++) {
			printProperty($object, $property."[$a][$b]");
		}
	}
}


dumpObject('cAxis', $axesProperties);
dumpObject('bAxis', $axesProperties);
dumpObject('zAxis', $axesProperties);
dumpObject('aAxis', $axesProperties);

dumpObject('robot.j1', $jointProperties);
dumpObject('robot.j2', $jointProperties);
dumpObject('robot.j3', $jointProperties);
dumpObject('robot.j4', $jointProperties);

dumpObject('robot', $groupProperties);
printMatrix('robot', 'couplingMatrix', 4, 4);
printMatrix('robot', 'axis',           4, 3);
printMatrix('robot', 'link',           4, 3);

?>

terminate program
