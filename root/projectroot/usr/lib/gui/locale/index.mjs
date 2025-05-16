// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { NavDropdown } from 'web/widgets'



export const LocaleDropdown = {
	setup() {
		return {
			langs: document.querySelector('meta[name=availableLanguages]').content.split(','),
			select(lang) {
				document.l10n.requestLanguages([lang]);
				document.getElementById('navbar-collapse')?.classList.remove('show');
			},
		}
	},
	components: { NavDropdown },
	template: //html
	`
	<NavDropdown icon="language" data-l10n-id="locale" right>
		<button v-for="lang in langs" @click="select(lang)" :data-l10n-id="'locale_'+lang" class="dropdown-item"></button>
	</NavDropdown>
	`
}
