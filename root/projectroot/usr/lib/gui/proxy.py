# Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
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


import logging, json
from tornado import web, websocket, gen



class WebsocketJsonProxy(websocket.WebSocketHandler):

	def initialize(self, url):
		self.url = url
		self.closed = False


	@gen.coroutine
	def receiveFromClient(self):
		try:
			while not self.closed:
				msg = yield self.clientConn.read_message()
				if msg is None: break
				self.write_message(msg)
		except websocket.WebSocketClosedError: pass
		self.clientConn.close()
		self.close()


	@gen.coroutine
	def open(self):
		self.set_nodelay(True)
		try:
			self.clientConn = yield websocket.websocket_connect(self.url)
			gen.Task(self.receiveFromClient)
		except Exception as e:
			logging.debug(e)
			self.close()


#	def on_message(self, message):
#		self.client_conn.write_message(message)


	def on_close(self):
		self.closed = True
		try:
			self.clientConn.close()
		except: pass




class HttpWebsocketJsonProxy(web.RequestHandler):

	def initialize(self, url):
		self.url = url


	@gen.coroutine
	def prepare(self):
		self.clientConn = yield websocket.websocket_connect(self.url)

		
	@gen.coroutine
	def post(self):
		rx = json.loads(self.request.body.decode('utf8'))
		self.clientConn.write_message(json.dumps(rx).encode('utf8'))
		yield self.get()

		
	@gen.coroutine
	def get(self):
		tx = yield self.clientConn.read_message()
		if tx is not None:
			self.write(tx)


	def on_finish(self):
		self.clientConn.close()
