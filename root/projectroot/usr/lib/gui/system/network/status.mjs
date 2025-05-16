// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { ref } from 'vue'
import { url, poll } from 'web/utils'
import networkIndex from 'system/network'


networkIndex.addPage('status', {
	setup() {

		const status = ref();

		poll(3000, async ()=>{
			status.value = await url('system.network.status.info').fetch();
		});

		return { status }
	},
	template: //html
	`
	<textarea v-text="status" type="text" class="form-control h-100 mb-3" style="font-family:monospace" readonly></textarea>
	`
})
