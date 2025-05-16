// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { addPage } from 'web'
import { NavDropdown, PageLink } from 'web/widgets'



const index = addPage('/diag');
export default index;



export const DiagDropdown = {
	setup() {
		return {
			links: index.children,
		}
	},
	components: { NavDropdown, PageLink },
	template: //html
	`
	<NavDropdown icon="eye" data-l10n-id="diag" right>
		<PageLink v-for="to in links" :to />
	</NavDropdown>
	`
}
