# Copyright (c) 2017 Artur Wiebe <artur@4wiebe.de>
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


import os, socket, json, logging, base64
from systemd import journal
from tornado import web, httpserver, ioloop, gen, websocket
from concurrent.futures import ThreadPoolExecutor



class RequestHandler(web.RequestHandler):
	pass



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




class DocumentHandler(web.RequestHandler):
	def initialize(self, html):
		self.html = html
	def get(self):
		self.set_header('Cache-Control', 'no-store, must-revalidate')
		self.render(self.html)


class LocaleHandler(web.RequestHandler):
	def get(self, locale):
		self.render('locale/'+locale+'.ftl')


class PageModule(web.UIModule):
	def render(self, page):
		_id = os.path.splitext(os.path.basename(page))[0]
		return self.render_string(page, id=_id)




def addDocument(match, html):
	handlers.append(('/'+match, DocumentHandler, {'html': html}))

def addAjax(match, handler, params={}):
	handlers.append((ajaxPrefix+match, handler, params))

def ajax_url(handler, url=''):
	return ajaxPrefix+url


def run():
	application = web.Application(handlers, **settings)
	server = httpserver.HTTPServer(application, max_buffer_size=128*1024*1024)
	systemdSocket = socket.fromfd(3, socket.AF_INET6, socket.SOCK_STREAM)
	systemdSocket.setblocking(False)
	server.add_socket(systemdSocket)
	ioloop.IOLoop.current().start()



logging.root.addHandler(journal.JournalHandler())
logging.root.setLevel(logging.DEBUG)
logging.getLogger('tornado.access').setLevel(logging.WARNING)


ajaxPrefix = '/xhr/'

settings = {
	'static_path':				os.path.dirname(__file__)+'/static',
	'cookie_secret':			base64.b64encode(os.urandom(64)).decode('ascii'),
	"ui_modules":				{"page": PageModule},
	"ui_methods":				{"ajax_url": ajax_url},
	"compiled_template_cache":	False,
}

handlers = []

addAjax('state', 				WebsocketJsonProxy,		{'url': 'ws://mc:33000'})
addAjax('locale/(.*).ftl',		LocaleHandler)


executor = ThreadPoolExecutor(max_workers=2)
