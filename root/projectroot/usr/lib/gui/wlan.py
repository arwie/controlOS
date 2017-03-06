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
		
