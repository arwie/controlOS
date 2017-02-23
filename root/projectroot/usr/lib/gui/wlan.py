
import os, subprocess, json
from tornado.web import RequestHandler


wlanConfPath = '/etc/wlan.conf'


class WlanHandler(RequestHandler):

	def get(self):
		pass
	

	def post(self):
		do = self.get_query_argument('do')
		
		if do.startswith('set'):
			with open(wlanConfPath, 'wb') as fp:
				subprocess.run(['/sbin/wpa_passphrase', self.get_argument('ssid').encode('utf8'), self.get_argument('psk').encode('utf8')], stdout=fp)

		elif do.startswith('remove'):
			os.remove(wlanConfPath)
			
		subprocess.run(['/bin/systemctl', '--no-block', 'restart', 'wpa_supplicant.service'])
		
