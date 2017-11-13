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


from email.mime.multipart	import MIMEMultipart
from email.mime.text		import MIMEText
from email.mime.application	import MIMEApplication
from tornado import gen, process, template
from datetime import datetime
from shared import backup
import logging, json



def renderLogHtml(data):
	
	def processMessage(msg):
		msg['_SOURCE_REALTIME_TIMESTAMP'] = datetime.utcfromtimestamp(int(msg.get('_SOURCE_REALTIME_TIMESTAMP' if '_SOURCE_REALTIME_TIMESTAMP' in msg else '__REALTIME_TIMESTAMP', 0))/1000000).strftime('%d.%m.%Y, %H:%M:%S')
		msg['PRIORITY'] = int(msg.get('PRIORITY', 6))
		msg = {k:v for k,v in msg.items() if not k.startswith('__')}
		return msg
	
	return template.Template("""\
		<!DOCTYPE html>
		<html>
		<head>
			<meta charset="utf-8"/>
			<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
			<style>{{bootstrapCss}}</style>
		</head>
		<script>
			function toggleDisplay(div) { div.style.display = div.style.display == "none" ? "block" : "none"; }
		</script>
		<body>
			<table class="table table-sm table-hover">
			<tbody>
			{% for msg in reversed(messages) %}
				<tr class="{{ {2:'table-danger', 3:'table-danger', 4:'table-warning', 5:'table-info'}.get(msg['PRIORITY']) }}">
					<td>{{msg.get('_SOURCE_REALTIME_TIMESTAMP')}}</td>
					<td>{{ {0:'Emergency', 1:'Alert', 2:'Critical', 3:'Error', 4:'Warning', 5:'Notice', 6:'Info', 7:'Debug'}.get(msg['PRIORITY']) }}</td>
					<td>{{msg.get('_HOSTNAME')}}</td>
					<td>{{msg.get('SYSLOG_IDENTIFIER')}}</td>
					<td width="70%">
						<a onclick="toggleDisplay(this.nextElementSibling);return false" href="#"><strong style="white-space:pre-line">{{msg.get('MESSAGE')}}</strong></a>
						<div style="display:none">
							<p></p>
							<table class="table table-sm">
							<tbody>
							{% for key,value in msg.items() %}
								<tr>
									<td>{{key}}</td>
									<td style="white-space:pre-line">{{value}}</td>
								</tr>
							{% end %}
							</tbody>
							</table>
						</div>
					</td>
				</tr>
			{% end %}
			</tbody>
			</table>
		</body>
		</html>
		""").generate(
				bootstrapCss	= open('/usr/lib/gui/static/bootstrap.css', 'r').read(),
				messages		= [ processMessage(json.loads(l.decode())) for l in data.splitlines() ],
			)



class Issue(MIMEMultipart):
	
	def __init__(self, text):
		super().__init__()
		self['Subject'] = "issue"
		self.attach(MIMEText(text))
	
	
	@gen.coroutine
	def gather(self):
		
		# attach backup
		self.attach(MIMEApplication((yield backup.store()), name='backup.txz'))
		
		# attach full journal export
		proc = process.Subprocess(['/usr/bin/sh', '-c', '/usr/bin/journalctl --merge --no-pager --output=export --since=-3months | xz'], stdout=process.Subprocess.STREAM)
		self.attach(MIMEApplication((yield proc.stdout.read_until_close()), name='journal.xz'))
		yield proc.wait_for_exit(raise_error=False)
		
		# attach rendered journal html
		proc = process.Subprocess(['/usr/bin/journalctl', '--merge', '--no-pager', '--output=json', '--all', '--priority=notice', '--lines=300'], stdout=process.Subprocess.STREAM)
		try:
			self.attach(MIMEApplication(renderLogHtml((yield proc.stdout.read_until_close())), name='log.html'))
		except Exception as e:
			logging.error(e)
		yield proc.wait_for_exit(raise_error=False)
	
	
	def send(self):
		pass
	
	def encode(self):
		return bytes(self)

