
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
