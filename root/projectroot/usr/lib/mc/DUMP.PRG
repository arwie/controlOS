' Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
'
' Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
' associated documentation files (the "Software"), to deal in the Software without restriction,
' including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
' and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
' subject to the following conditions:
'
' The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
'
' THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
' INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
' IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
' WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
' OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


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
