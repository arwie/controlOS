# Copyright (c) 2023 Artur Wiebe <artur@4wiebe.de>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


from typing import Self
from contextlib import asynccontextmanager, suppress
import logging
import asyncio
import json
import tornado.web
import tornado.websocket



class RequestHandler(tornado.web.RequestHandler):
	
	def prepare(self):
		self.set_header('Access-Control-Allow-Origin', self.request.headers['Origin'])
	
	def read_json(self):
		return json.loads(self.request.body.decode())
	
	def write_json(self, data):
		self.write(json.dumps(data).encode())



class WebSocketHandler(tornado.websocket.WebSocketHandler):

	def check_origin(self, origin):
		return True

	@classmethod
	def __init_subclass__(cls):
		cls.all_clients: list[Self] = []

	@classmethod
	def all_write_message(cls, msg, **kwargs):
		for client in cls.all_clients:
			client.write_message(msg, **kwargs)

	@classmethod
	def all_write_message_json(cls, msg):
		cls.all_write_message(json.dumps(msg))

	def open(self):
		self.set_nodelay(True)
		self.on_open()

		async def update(coro):
			try:
				with suppress(asyncio.CancelledError):
					await coro()
			except:
				logging.exception('web: update task error')

		if not self.all_clients:
			cls = type(self)
			cls.all_update_task = asyncio.create_task(update(cls.all_update)) if hasattr(cls, 'all_update') else None
		self.all_clients.append(self)
		self.update_task = asyncio.create_task(update(self.update)) if hasattr(self, 'update') else None

	def on_open(self):
		pass

	def on_close(self):
		if self.update_task:
			self.update_task.cancel()
		self.all_clients.remove(self)
		if not self.all_clients:
			if self.all_update_task:
				self.all_update_task.cancel()
	
	def write_message_json(self, msg):
		self.write_message(json.dumps(msg))

	def on_message(self, msg):
		self.on_message_json(json.loads(msg))

	def on_message_json(self, msg):
		raise NotImplemented




handlers = []

def handler(name, params={}):
	def add_handler(handler):
		handlers.append((f'/{name}', handler, params))
		return handler
	return add_handler



@asynccontextmanager
async def exec():
	server = tornado.web.Application(handlers).listen(33000, 'sys')
	yield
	server.stop()
	await server.close_all_connections()