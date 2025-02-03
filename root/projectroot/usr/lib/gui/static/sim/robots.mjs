// Copyright (c) 2024 Artur Wiebe <artur@4wiebe.de>
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


import {
	Bone,
	Vector3,
	MathUtils,
	AxesHelper,
	MeshPhongMaterial,
	Mesh,
	CylinderGeometry,
} from '/static/three/three.module.js';



export class Robot extends Bone {
	constructor(params) {
		super();

		this.tool = new Bone();
	}
}



export class LinearDeltaRobot extends Robot {
	constructor(params) {
		super();

		this.plate = new Bone();
		this.add(this.plate);
		this.plate.add(this.tool);

		this.arms = [];
		this.rods = [];

		for (let theta of [0, 120, 240]) {

			let rail = new Bone();
			rail.rotateZ(MathUtils.degToRad(theta));
			rail.translateX(params.outerRadius);
			rail.rotateY(MathUtils.degToRad(180 - params.axisAngle));
			this.add(rail);

			let arm = new Bone();
			rail.add(arm);
			this.arms.push(arm);

			for (let side of [1, -1]) {

				let rodsOnPlate = new Bone();
				rodsOnPlate.rotateZ(MathUtils.degToRad(theta));
				rodsOnPlate.translateX(params.innerRadius);
				this.plate.add(rodsOnPlate);

				let rod = new Bone();
				rod.translateY(side * params.rodsDistance / 2);
				rod.connectPlate = function() {
					rod.lookAt(rodsOnPlate.localToWorld(new Vector3(0, rod.position.y, 0)));
					rod.rotation.z = 0;
				};
				arm.add(rod);
				this.rods.push(rod);
			}
		}
	}
	
	setPose(axes, pos) {
		this.arms[0].position.x = axes[0];
		this.arms[1].position.x = axes[1];
		this.arms[2].position.x = axes[2];
		this.plate.position.x = pos.x;
		this.plate.position.y = pos.y;
		this.plate.position.z = pos.z;
		this.tool.rotation.z = MathUtils.degToRad(pos.r);
		for (let rod of this.rods)
			rod.connectPlate();
	}
}



export class RotaryDeltaRobot extends Robot {
	constructor(params, genericLinks=false) {
		super();

		if (genericLinks) {
			let mesh = new Mesh(
				new CylinderGeometry(params.outerRadius, params.outerRadius, 40, 24),
				new MeshPhongMaterial({color:'DarkSlateGray'})
			);
			mesh.rotation.x = MathUtils.degToRad(90);
			this.add(mesh);
		}

		this.plate = new Bone();
		if (genericLinks) {
			const radius = Math.sqrt(Math.pow(params.innerRadius, 2) + Math.pow(params.rodsDistance / 2, 2));
			let mesh = new Mesh(
				new CylinderGeometry(radius, radius, 10, 24),
				new MeshPhongMaterial({color:'OrangeRed'})
			);
			mesh.rotation.x = MathUtils.degToRad(90);
			this.plate.add(mesh);
		}
		this.add(this.plate);
		this.plate.add(this.tool);

		this.arms = [];
		this.rods = [];

		for (let theta of [180, 60, -60]) {

			let axis = new Bone();
			axis.rotateZ(MathUtils.degToRad(theta));
			axis.translateX(params.outerRadius);
			this.add(axis);

			let arm = new Bone();
			if (genericLinks) {
				let mesh = new Mesh(
					new CylinderGeometry(20, 20, params.armLength, 16),
					new MeshPhongMaterial({color:'OrangeRed'})
				);
				mesh.position.x = params.armLength / 2;
				mesh.rotation.z = MathUtils.degToRad(90);
				arm.add(mesh);
			}
			axis.add(arm);
			this.arms.push(arm);

			for (let side of [1, -1]) {

				let rodsOnPlate = new Bone();
				rodsOnPlate.rotateZ(MathUtils.degToRad(theta));
				rodsOnPlate.translateX(params.innerRadius);
				this.plate.add(rodsOnPlate);

				let rod = new Bone();
				rod.translateX(params.armLength);
				rod.translateY(side * params.rodsDistance / 2);
				rod.connectPlate = function() {
					rod.lookAt(rodsOnPlate.localToWorld(new Vector3(0, rod.position.y, 0)));
					rod.rotation.z = 0;
				};
				if (genericLinks) {
					let mesh = new Mesh(
						new CylinderGeometry(5, 5, params.rodsLength, 16),
						new MeshPhongMaterial({color:'SlateGray'})
					);
					mesh.rotation.x = MathUtils.degToRad(90);
					mesh.position.z = params.rodsLength / 2;
					rod.add(mesh);
				}
				arm.add(rod);
				this.rods.push(rod);
			}
		}

	}
	
	setPose(axes, pos) {
		this.arms[0].rotation.y = -MathUtils.degToRad(axes[0]);
		this.arms[1].rotation.y = -MathUtils.degToRad(axes[1]);
		this.arms[2].rotation.y = -MathUtils.degToRad(axes[2]);
		this.plate.position.x = pos.x;
		this.plate.position.y = pos.y;
		this.plate.position.z = pos.z;
		this.tool.rotation.z = MathUtils.degToRad(pos.r);
		for (let rod of this.rods)
			rod.connectPlate();
	}
}
