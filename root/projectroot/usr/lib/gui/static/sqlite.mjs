// Copyright (c) 2020 Artur Wiebe <artur@4wiebe.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


export default class SqliteTable {
	constructor(handler) {
		this.handler = handler;
	}
	
	url(action, args={}) {
		args['do'] = action;
		return ajaxUrl(this.handler, args);
	}
	
	stringify(data) {
		return JSON.stringify(data, function(k, v) { return (v === undefined) ? null : v; });
	}
	
	
	list() {
		return $.getJSON(this.url('list'));
	}

	load(id) {
		return $.getJSON(this.url('load', {id:id}));
	}
	
	
	create() {
		return $.post(this.url('create'), null, null, 'json');
	}
	
	copy(id) {
		return $.post(this.url('copy', {id:id}), null, null, 'json');
	}
	
	remove(id) {
		return $.post(this.url('remove', {id:id}));
	}
	
	save(id, data) {
		return $.post(
			this.url('save', {id:id}), 
			this.stringify(data)
		);
	}
	
	swap(id, swap) {
		return $.post(
			this.url('swap', {id:id, swap:swap})
		);
	}
	
	loadMapping(id, mapping) {
		return this.load(id).done((data)=>{
			if ('__autosave__' in mapping)
				mapping.__autosave__.dispose();
			ko.mapping.fromJS(data, mapping);
			mapping.__autosave__ = ko.subscribeMapping(mapping, ()=>{
				//console.log("autosave", ko.mapping.toJS(mapping));
				this.save(id, ko.mapping.toJS(mapping));
			});
		});
	}
}
