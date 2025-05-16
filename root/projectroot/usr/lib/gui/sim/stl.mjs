// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import {
	Bone,
	Mesh,
	MeshPhongMaterial,
} from 'three';
import { STLLoader } from 'three/loaders/STLLoader';



//material: Material or Material properties
//bones: Bone or [Bone]
export default function loadStl(file, material, bones = new Bone())
{
	if (file)
		new STLLoader().load(file, (geometry) => {

			if (!material.isMaterial)
				material = new MeshPhongMaterial(material);

			if (geometry.hasColors) {
				material.vertexColors = true;
				material.opacity = geometry.alpha;
			}

			for (let bone of (Array.isArray(bones) ? bones : [bones]))
				bone.add(new Mesh(geometry, material));

		});

	return bones;
}
