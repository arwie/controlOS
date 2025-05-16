// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import systemIndex from 'system'
import { PageLink } from 'web/widgets'



const index = systemIndex.addPage('network', {
	setup() {
		return {
			links: index.children,
		}
	},
	components: { PageLink },
	template: //html
	`
	<div class="row h-100 mb-3">
		<div class="col-lg-3 nav nav-pills flex-column mb-4">
			<PageLink v-for="to in links" :to :data-l10n-id="'network_'+to.name" />
		</div>
		<div class="col-lg h-100">
			<RouterView />
		</div>
	</div>
	`
})
export default index;
