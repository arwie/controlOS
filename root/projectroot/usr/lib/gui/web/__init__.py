# SPDX-FileCopyrightText: 2025 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT

from __future__ import annotations
from typing import Any, Self

import json
from contextlib import suppress
from pathlib import Path
import logging
import asyncio
from itertools import chain

import tornado.web
import tornado.httpserver
import tornado.websocket
from tornado.websocket import WebSocketClosedError


gui_path = Path('/usr/lib/gui')



class RequestHandler(tornado.web.RequestHandler):

	def initialize(self):
		self.set_header('Cache-Control', 'no-store, must-revalidate')

	def read_json(self):
		return json.loads(self.request.body.decode())
	
	def write(self, msg: bytes | Any):
		if not isinstance(msg, bytes):
			msg = json.dumps(msg).encode()
		super().write(msg)



class ModuleHandler(RequestHandler):

	def initialize(self):
		super().initialize()
		self.set_header('Content-Type', 'text/javascript; charset=utf-8')

	async def get(self):
		self.write(b'export default ')
		self.write(await self.export_default())

	async def export_default(self):
		return {}



class WebSocketHandler(tornado.websocket.WebSocketHandler):

	def write_message(self, msg: bytes | Any, **kwargs):
		if not isinstance(msg, bytes):
			msg = json.dumps(msg).encode()
		with suppress(WebSocketClosedError):
			super().write_message(msg, **kwargs).cancel()

	def on_message(self, msg):
		self.on_message_json(json.loads(msg))

	def on_message_json(self, msg:dict):
		raise NotImplemented



_handlers = []

def handler[T:type[RequestHandler | WebSocketHandler]](handler:T) -> T:
	_handlers.append((f'/{handler.__module__}.{handler.__name__}', handler))
	return handler



@handler
class imports(ModuleHandler):
	mjs = list[str]()

	@classmethod
	def add(cls, *mjs:str):
		cls.mjs.extend(mjs)

	def get(self):
		self.write('\n'.join(f'import "{m}"' for m in self.mjs).encode())


@handler
class files(ModuleHandler):
	globs = list[str]()

	@classmethod
	def glob(cls, glob:str):
		cls.globs.append(glob)

	async def export_default(self):
		return {
			file: self.static_url(file) for file in
				(str(path.relative_to(gui_path)) for path in
					chain(*(gui_path.glob(glob) for glob in self.globs)))
		}


@handler
class setup(ModuleHandler):
	async def export_default(self):
		from shared.setup import setup
		return setup



@handler
class locale(RequestHandler):

	@staticmethod
	def get_template_path():
		return Path(gui_path, 'locale')

	available_languages = ','.join(f.stem for f in get_template_path().glob('*.ftl'))

	def get(self):
		self.render(f'{self.get_query_argument('ftl')}.ftl')



@handler
class targets(WebSocketHandler):
	all = set[Self]()
	targets = list[str]()
	
	@classmethod
	def start(cls):

		def send_all(msg):
			msg = json.dumps(msg).encode()
			for client in cls.all:
				client.write_message(msg)

		async def watchdog():
			while True:
				await asyncio.sleep(1)
				send_all(None)

		async def update():
			journalctl = await asyncio.create_subprocess_exec('journalctl','--follow','--output=cat','--lines=0','_PID=1', stdout=asyncio.subprocess.PIPE)
			assert journalctl.stdout is not None
			
			timeout = 0
			while True:
				try:
					line = await asyncio.wait_for(journalctl.stdout.readline(), timeout)
					if b'target' in line:
						timeout = 0.1
					continue
				except asyncio.TimeoutError:
					timeout = None
				
				systemctl  = await asyncio.create_subprocess_exec('systemctl','list-units','--no-pager','--no-legend','--plain','--type=target','--state=active', stdout=asyncio.subprocess.PIPE)
				targets,*_ = await systemctl.communicate()
				cls.targets = list(t.partition('.target')[0] for t in targets.decode().splitlines())
				
				send_all(cls.targets)

		cls.tasks = asyncio.create_task(watchdog()), asyncio.create_task(update())

	def open(self):
		self.all.add(self)
		self.write_message(self.targets)
	
	def on_close(self):
		self.all.discard(self)

	def post(self):
		pass # connection test



def server():
	logging.getLogger('tornado.access').setLevel(logging.WARNING)

	class DocumentHandler(RequestHandler):
		importmap = {}

		def get(self):
			if not self.importmap:
				for mjs in gui_path.rglob('*.*js'):
					path = str(mjs.relative_to(gui_path))
					surl = self.static_url(
						str(mjs.resolve().relative_to(gui_path)) if mjs.is_symlink() else path
					)
					self.importmap[f'/~/{path}'] = surl
					module = path.rpartition('.')[0].removesuffix('/index')
					self.importmap[module] = surl

			self.render(
				'web.html',
				importmap=json.dumps(self.importmap, indent=2),
				available_languages=locale.available_languages
			)

	class StaticFileHandler(tornado.web.StaticFileHandler):
		def validate_absolute_path(self, root, absolute_path):
			if absolute_path.endswith('.py'):
				raise tornado.web.HTTPError(404)
			return super().validate_absolute_path(root, absolute_path)


	targets.start()

	return tornado.httpserver.HTTPServer(
		tornado.web.Application(
			[*_handlers, ('/.*', DocumentHandler)],
			static_path=gui_path,
			static_url_prefix='/~/',
			static_handler_class=StaticFileHandler,
			websocket_ping_interval=10,
			compiled_template_cache=False,
			static_hash_cache=False,
		),
		max_buffer_size=128*1024*1024,
	)
