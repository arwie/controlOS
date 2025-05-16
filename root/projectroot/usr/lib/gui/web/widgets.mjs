// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { router } from 'web'
import { poll } from 'web/utils'
import { ref, provide, inject } from 'vue'



export const RootView = {
	props: ['image'],
	setup(props) {
		return {
			image: props.image,
		}
	},
	template: //html
	`
	<nav class="navbar navbar-expand-lg fixed-top bg-light">
		<div class="container-fluid">
			<a href="#" class="navbar-brand">
				<img  v-if="image" :src="image" height="22" class="d-inline-block align-baseline"/>
				<span v-else data-l10n-id="title"></span>
			</a>
			<button type="button" class="navbar-toggler" data-bs-toggle="collapse" data-bs-target="#navbar-collapse">
				<span class="navbar-toggler-icon"></span>
			</button>
			<div class="collapse navbar-collapse navbar-nav-scroll" id="navbar-collapse">
				<div class="navbar-nav">
					<slot name="navbar"></slot>
				</div>
				<div class="navbar-nav ms-auto">
					<slot name="navbar-right"></slot>
				</div>
			</div>
		</div>
	</nav>
	<main class="container-fluid h-100 d-flex flex-column" style="padding-top:4.5rem">
		<RouterView />
	</main>
	`
}


export const PageLink = {
	props: ['to','class'],
	setup(props) {
		let to = props.to;
		if (typeof to === 'string')
			to = router.resolve(to);
		return {
			to,
			cls: inject('class', props['class'] || 'nav-link'),
			navbarCollapse() {
				document.getElementById('navbar-collapse')?.classList.remove('show');
			}
		}
	},
	template: //html
	`
	<RouterLink
		:to
		v-show="!to.beforeEnter||to.beforeEnter()"
		:class="cls"
		:data-l10n-id="to.name"
		@click="navbarCollapse"
		activeClass="active"
	/>
	`
}


export const NavDropdown = {
	props: {
		icon: String,
		dataL10nId: String,
		right: Boolean,
	},
	setup(props) {
		provide('class', 'dropdown-item');
		return {
			icon: props.icon,
			dataL10nId: props.dataL10nId,
			right: props.right,
		}
	},
	template: //html
	`
	<div class="nav-item dropdown">
		<a href="#" class="nav-link dropdown-toggle" data-bs-toggle="dropdown">
			<i v-if="icon" :class="'fas fa-'+icon"></i>
			<span v-if="dataL10nId" :class="{'d-none d-xxl-inline':icon}" :data-l10n-id></span>
		</a>
		<div class="dropdown-menu" :class="{'dropdown-menu-end':right}">
			<slot></slot>
		</div>
	</div>
	`
}


export const ButtonBar = {
	template: //html
	`
	<div class="py-3">
		<slot></slot>
	</div>
	`
}


export const ConfirmButton = {
	emits: ['click'],
	setup(props, { emit }) {

		const active = ref(null);
		const activeBlink = ref(false);

		poll(333, ()=>{
			activeBlink.value = active.value ? !activeBlink.value : false;
		});

		function clickConfirm(ev) {
			if (active.value) {
				emit('click', ev);
				active.value = null;
			} else {
				active.value = false;
				setTimeout(()=>{
					active.value = true;
					setTimeout(()=>{
						active.value = null;
					}, 3000);
				}, 500);
			}
		}

		return { clickConfirm, active, activeBlink }
	},
	template: //html
	`
	<button @click="clickConfirm" class="btn" :class="{'btn-dark':activeBlink}" :disabled="active===false"></button>
	`
}


export const FileButton = {
	props: ['data-l10n-id'],
	emits: ['file'],
	setup(props) {
		return { 
			dataL10nId: props.dataL10nId,
		}
	},
	template: //html
	`
	<label class="btn">
		<span :data-l10n-id></span>
		<input type="file" @change="$emit('file', $event.target.files[0], $event.target.parentElement)" hidden>
	</label>
	`
}



export async function feedback(element, promise) {
	const popover = bootstrap.Popover.getOrCreateInstance(element, {
		container:	element.parentElement,
		trigger:	'manual',
		html:		true,
		sanitize:	false,
		placement:	'top',
		animation:	false,
		content:	'feedback',
	});
	popover.feedback ??= function(icon, busy) {
		element.disabled = busy;
		popover.hide();
		popover.setContent({'.popover-body': icon});
		popover.show();
		if (!busy) {
			popover.tip.tabIndex = 0;
			popover.tip.onblur = ()=>popover.hide();
			popover.tip.focus({preventScroll:true});
		}
	};
	popover.feedback('<i class="fas fa-spinner fa-spin fs-4 text-primary"></i>', true);
	try {
		const result = await promise;
		popover.feedback('<i class="fas fa-check-circle fs-4 text-success"></i>', false);
		return result;
	} catch (error) {
		popover.feedback('<i class="fas fa-times-circle fs-4 text-danger"></i>',  false);
		//throw error;
	}
}
