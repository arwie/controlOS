
import os, json
from tornado.web import RequestHandler


class UpdateHandler(RequestHandler):
	
	def get(self):
		info = {}
		with open('/version-id', 'r') as f:
			info['version-id'] = f.read()
		self.write(json.dumps(info).encode('utf8'))
	
	def put(self):
		with open('/mnt/switch/.update', 'wb') as f:
			f.write(self.request.files['update'][0]['body'])
		os.rename('/mnt/switch/.update', '/mnt/switch/update')
