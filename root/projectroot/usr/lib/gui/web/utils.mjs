// SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
// SPDX-License-Identifier: MIT

import { onUnmounted } from 'vue'


const ports = {
	app:	33000,
}



export function url(url='', port=null, host=null) {

	host ??= location.hostname;
	port = ports[port] || port || location.port;
	let query = '';

	return {

		query(params) {
			if (params)
				query = `?${new URLSearchParams(params).toString()}`;
			return this;
		},

		async fetch(options={}, type='text') {
			const response = await fetch(this, options);
			if (!response.ok) {
				throw new Error(`url fetch error: ${response.status}`);
			}
			return response[type]();
		},

		async fetchJson(options={}) {
			const body = await this.fetch(options)
			return jsonParse(body)
		},

		post(body, options={}) {
			return this.fetch({ method:'POST', body, ...options});
		},

		postJson(body, options={}) {
			return this.post(jsonStringify(body), options);
		},

		put(body, options={}) {
			return this.fetch({ method:'PUT', body, ...options});
		},

		webSocket(handler) {
			const ws = new WebSocket(this.toString('ws:'));
			ws.onmessage = msg => handler(msg.data);
			onUnmounted(()=>{
				ws.onclose = ws.onerror = ws.onmessage = null;
				ws.close();
			});
			return ws;
		},

		webSocketJson(handler) {
			const ws = this.webSocket(
				msg => handler(jsonParse(msg))
			);
			ws.sendJson = function(data={}) {
				ws.send(jsonStringify(data));
			};
			return ws;
		},

		toString(protocol=location.protocol) {
			return `${protocol}//${host}:${port}/${url}${query}`;
		},
	};
}


export function poll(period, func) {
	func();
	func.interval = setInterval(func, period);
	func.clear = () => clearInterval(func.interval);
	onUnmounted(func.clear);
	return func;
}


export function jsonStringify(msg) {
	return JSON.stringify(msg, (k, v) => (v === undefined) ? null : v);
}

export function jsonParse(msg) {
	return msg ? JSON.parse(msg) : null;
}


export function isObject(item) {
	return (item && typeof item === 'object' && !Array.isArray(item));
}

export function updateDeep(target, source) {
	if (isObject(source)) {
		for (const key in target) {
			if (isObject(target[key])) {
				updateDeep(target[key], source[key]);
			} else if(Object.hasOwn(source, key)) {
				target[key] = source[key];
			}
		}
	}
}
