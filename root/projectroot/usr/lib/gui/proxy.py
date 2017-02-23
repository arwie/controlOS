import logging, json
from tornado import web, websocket, gen



class WebsocketJsonProxy(websocket.WebSocketHandler):

	def initialize(self, url):
		self.url = url


	@gen.coroutine
	def receiveFromClient(self):
		try:
			while True:
				msg = yield self.clientConn.read_message()
				if msg is None: break
				self.write_message(msg)
		except websocket.WebSocketClosedError: pass
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
		try:
			self.clientConn.close()
		except: pass
