
import shlex, json
from tornado import web, websocket, gen, process, iostream


journalArgs = ['/bin/journalctl', '--file=/var/log/journal/*/*']



class LogHandler(websocket.WebSocketHandler):
	
	@gen.coroutine
	def readJournal(self):
		try:
			while True:
				msg = yield self.journalProc.stdout.read_until(b'\n')
				msg = json.loads(msg.decode('utf8'))
				
				if not '_SOURCE_REALTIME_TIMESTAMP' in msg:
					msg['_SOURCE_REALTIME_TIMESTAMP'] = msg['__REALTIME_TIMESTAMP']
				msg = {k: v for k,v in msg.items() if not k.startswith('__')}
				
				self.write_message(json.dumps(msg).encode('utf8'))
				
		except (iostream.StreamClosedError, websocket.WebSocketClosedError): pass
		self.close()
	
	
	def open(self):
		args = journalArgs.copy()
		args.extend(shlex.split(self.get_argument('args')))
		args.append('--lines=300')
		args.append('--output=json')
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




class LogFieldHandler(web.RequestHandler):
	
	@gen.coroutine
	def get(self, field):
		args = journalArgs.copy()
		args.append('--field={}'.format(field))
		journalProc = process.Subprocess(args, stdout=process.Subprocess.STREAM)
		
		values = yield journalProc.stdout.read_until_close()
		values = values.decode('utf8').splitlines()
		self.write(json.dumps(values).encode('utf8'))

		yield journalProc.wait_for_exit(raise_error=False)
