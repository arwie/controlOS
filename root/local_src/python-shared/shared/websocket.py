# Copyright (c) 2018 Artur Wiebe <artur@4wiebe.de>
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


import json, logging
from tornado import ioloop, gen, websocket



class Client:
	def initialize(self, host, port):
		self.url = 'ws://{}:{}'.format(host,port)
	
	def writeMessageJson(self, data):
		self.ws.write_message(json.dumps(data))
	
	
	def close(self):
		self.ws.close()
	
	
	@gen.coroutine
	def execute(self):
		self.ws = yield websocket.websocket_connect(self.url)
		
		while True:
			msg = yield self.ws.read_message()
			
			if msg is None:
				self.onClose()
				break
			
			self.onMessage(msg)
	
	
	def onMessage(self, data):
		self.onMessageJson(json.loads(data))
	
	def onMessageJson(self, data):
		pass
	
	def onClose(self):
		pass



def run(Client):
	try:
		ioloop.IOLoop.current().run_sync(Client().execute)
	except:
		logging.exception('websocket: error in client')
