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


import shutil, subprocess
from tornado.web import RequestHandler
from tornado import web, gen, process


class BackupHandler(web.RequestHandler):

	@gen.coroutine
	def get(self):
		self.set_header('Content-Type',        'application/octet-stream')
		self.set_header('Content-Disposition', 'attachment; filename=backup.txz')
		
		backupProc = process.Subprocess(['/bin/tar', '-cJ', '-C/var/etc', '.'], stdout=process.Subprocess.STREAM)
		backup = yield backupProc.stdout.read_until_close()
		self.write(backup)
		yield backupProc.wait_for_exit(raise_error=False)
	

	def put(self):
		subprocess.run(['/usr/bin/find', '/var/etc', '-delete', '-mindepth', '1'])	
		subprocess.run(['/bin/tar', '-xJ', '-C/var/etc'], input=self.request.files['backup'][0]['body'])	
		subprocess.run(['/bin/systemctl', '--no-block', 'reboot'])
