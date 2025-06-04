// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { ref } from 'vue'
import { url } from 'web/utils'
import { ButtonBar, FileButton, ConfirmButton, feedback } from 'web/widgets'
import { systemIndex } from 'system'



systemIndex.addPage('update', {
	setup() {

		const version = ref();
		const revertDate = ref();

		url('system.update.file').fetchJson().then((data)=>{
			version.value = data
		});
		url('system.update.revert').fetch().then((mtime)=>{
			if (mtime)
				revertDate.value = new Date(mtime*1000).toLocaleString(navigator.language, {hour12:false});
		});

		function upload(file, element) {
			feedback(element, url('system.update.file').put(file));
		}

		function revert(ev) {
			feedback(ev.target, url('system.update.revert').post());
		}

		return { version, upload, revertDate, revert }
	},
	components: { ButtonBar, FileButton, ConfirmButton },
	template: //html
	`
	<div class="accordion" id="update_accordion">
		<div class="accordion-item">
			<h2 class="accordion-header">
				<button class="accordion-button" type="button" data-bs-toggle="collapse" data-bs-target="#update_file" data-l10n-id="update_file"></button>
			</h2>
			<div id="update_file" class="accordion-collapse collapse show" data-bs-parent="#update_accordion">
				<div class="accordion-body">
					<div class="mb-3">
						<label class="form-label" data-l10n-id="update_version"></label>
						<dl v-if="version" class="form-control">
							<dt data-l10n-id="update_versionName"></dt>
							<dd v-text="version.name"></dd>
							<dt data-l10n-id="update_versionId"></dt>
							<dd v-text="version.id"></dd>
						</dl>
					</div>
					<ButtonBar>
						<FileButton @file="upload" class="btn-primary" data-l10n-id="update_file" />
					</ButtonBar>
				</div>
			</div>
		</div>
		<div v-if="revertDate" class="accordion-item">
			<h2 class="accordion-header">
				<button class="accordion-button collapsed" type="button" data-bs-toggle="collapse" data-bs-target="#update_revert" data-l10n-id="update_revert"></button>
			</h2>
			<div id="update_revert" class="accordion-collapse collapse" data-bs-parent="#update_accordion">
				<div class="accordion-body">
					<dl class="form-control">
						<dt data-l10n-id="update_revertDate"></dt>
						<dd v-text="revertDate"></dd>
					</dl>
					<ButtonBar>
						<ConfirmButton @click="revert" class="btn-danger" data-l10n-id="update_revert" />
					</ButtonBar>
				</div>
			</div>
		</div>
	</div>
	`
})
