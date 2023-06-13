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


import logging
import asyncio
import json
import tornado.web
import tornado.websocket



class RequestHandler(tornado.web.RequestHandler):
	def read_json(self):
		return json.loads(self.request.body.decode())
	def write_json(self, data):
		self.write(json.dumps(data).encode())


class WebSocketHandler(tornado.websocket.WebSocketHandler):
	class Task:
		def __init__(self, cycle_time, task):
			self.cycle_time = cycle_time
			self.task = task
			self.clients = set()

		async def task_body(self):
			try:
				while not self.task_handle.cancelled():
					msg = self.task()
					for client in self.clients:
						client.write_message(msg)
					await asyncio.sleep(self.cycle_time)
			except asyncio.CancelledError:
				pass
			except Exception:
				logging.exception('web: task error')

		def add_client(self, client):
			if not self.clients:
				self.task_handle = asyncio.create_task(self.task_body())
			self.clients.add(client)

		def remove_client(self, client):
			self.clients.remove(client)
			if not self.clients:
				self.task_handle.cancel()

	def check_origin(self, origin):
		return True
	def write_message_json(self, data):
		self.write_message(json.dumps(data))



handlers = []

def add_handler(name, handler, params={}):
	handlers.append(('/'+name, handler, params))



def start():
	tornado.web.Application(handlers).listen(33000, 'sys')
