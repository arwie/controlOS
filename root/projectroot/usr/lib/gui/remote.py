
import subprocess, json
from tornado.web import RequestHandler


class RemoteHandler(RequestHandler):
	
	def get(self):
		pass
	
	def post(self):
		subprocess.run(['/bin/systemctl', '--no-block', 'start', 'remote.service'])
