// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { createApp, shallowRef } from 'vue'
import { createRouter, createWebHashHistory } from 'vue/router'
import { url, poll } from 'web/utils'



let rootView;

export function setRootView(component) {
	rootView = component;
}


const routes = [];
export let router;

export function addPage(path, component, parent=routes) {
	const route = {
		path,
		name: path,
		component,
		children: [],
		addPage(path, component) {
			return addPage(path, component, this.children);
		}
	}
	if (component && component.targetGuard)
		route.beforeEnter = () => target(component.targetGuard);
	parent.push(route);
	return route;
}


const targets = shallowRef([]);

export function target(target) {
	return targets.value.includes(target);
}



export default function() {
	const app = createApp(rootView);

	router = createRouter({
		history: createWebHashHistory(),
		routes,
	})
	app.use(router);

	app.config.compilerOptions.whitespace = 'preserve';
	app.mount('#gui-view');

	function disconnected() {
		if (watchdog !== null) {
			watchdog = null;
			console.log('GUI: disconnected!');
			new bootstrap.Modal('#gui-disconnected').show();
			poll(3000, ()=>{
				url('web.targets').post(null, { signal: AbortSignal.timeout(3000) })
					.then(()=>location.reload())
					.catch(()=>console.log('GUI: trying to reconnect...'));
			});
		}
	}
	let watchdog = true;

	const ws = url('web.targets').webSocketJson((msg)=>{
		if (Array.isArray(msg)) {
			targets.value = msg;
			const route = router.currentRoute.value;
			const guard = route.matched.at(-1)?.beforeEnter;
			if (guard && !guard()) {
				router.replace(route.matched.at(-2) || '/');
			}
		}
		clearTimeout(watchdog);
		watchdog = setTimeout(disconnected, 15000);
	});
	ws.onclose = ws.onerror = disconnected;
}
