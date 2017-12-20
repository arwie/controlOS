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


import server
import shlex, json
from tornado import websocket, gen, process, iostream



def setArgs(journalFile):
	global journalArgs
	journalArgs = ['/usr/bin/journalctl', '--file='+journalFile]

setArgs('/var/log/journal/*/*')



class Handler(server.WebSocketHandler):
	
	@gen.coroutine
	def readJournal(self):
		try:
			while True:
				msg = yield self.journalProc.stdout.read_until(b'\n')
				msg = json.loads(msg.decode())
				
				if not '_SOURCE_REALTIME_TIMESTAMP' in msg:
					msg['_SOURCE_REALTIME_TIMESTAMP'] = msg['__REALTIME_TIMESTAMP']
				msg = {k: v for k,v in msg.items() if not k.startswith('__')}
				
				self.write_messageJson(msg)
				
		except (iostream.StreamClosedError, websocket.WebSocketClosedError): pass
		self.close()
	
	
	def open(self):
		args = journalArgs.copy()
		args.extend(shlex.split(self.get_argument('args')))
		args.append('--lines=300')
		args.append('--output=json')
		args.append('--all')
		args.append('--follow')
		args.append('--merge')
		
		self.journalProc = process.Subprocess(args, stdout=process.Subprocess.STREAM)
		gen.Task(self.readJournal)


	@gen.coroutine
	def on_close(self):
		try:
			self.journalProc.proc.terminate()
			yield self.journalProc.wait_for_exit(raise_error=False)
		except: pass




class LogFieldHandler(server.RequestHandler):
	
	@gen.coroutine
	def get(self, field):
		args = journalArgs.copy()
		args.append('--field={}'.format(field))
		journalProc = process.Subprocess(args, stdout=process.Subprocess.STREAM)
		
		values = yield journalProc.stdout.read_until_close()
		self.writeJson(values.decode().splitlines())

		yield journalProc.wait_for_exit(raise_error=False)



server.addAjax(__name__,			Handler)
server.addAjax(__name__+'/(.*)',	LogFieldHandler)
