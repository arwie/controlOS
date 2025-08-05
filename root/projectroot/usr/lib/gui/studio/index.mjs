// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { setRootView, addPage } from 'web'
import { RootView, PageLink } from 'web/widgets'

import { SystemDropdown } from 'system'
import { PoweroffDropdown } from 'system/power'
import { LocaleDropdown } from 'web/locale'
import { DiagDropdown } from 'diag'



export const studioIndex = addPage('studio');


setRootView({
	setup() {
		return {
			links: studioIndex.children,
		}
	},
	components: { RootView, PageLink, DiagDropdown, SystemDropdown, LocaleDropdown, PoweroffDropdown },
	template: //html
	`
	<RootView :title="$t('studio.title', { site: $t('title') })">
		<template #navbar>
			<PageLink to="/diag/log" />
			<PageLink v-for="to in links" :to />
		</template>
		<template #navbar-right>
			<DiagDropdown />
			<SystemDropdown />
			<LocaleDropdown />
			<PoweroffDropdown />
		</template>
	</RootView>
	`
})
