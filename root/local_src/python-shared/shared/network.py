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


import os, subprocess



def networkStatus():
	def shell(cmd): return subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).stdout.decode()
	def format(name, content): return "##### {}\n{}\n\n".format(name, content)
	status  = format('status',				shell('networkctl --no-pager status'))
	status += format('internet access',		shell('ping -c1 -W1 google.com'))
	status += format('local connections',	shell('ping -c1 -W1 mc'))
	if interfaceAvailable('syswlan'):
		status += format('syswlan',			shell('systemctl --no-pager status hostapd'))
	if interfaceAvailable('wlan'):
		status += format('wlan status',		shell('wpa_cli status'))
		status += format('wlan stations',	shell('wpa_cli scan_result'))
	status += format('addresses',			shell('ip addr'))
	status += format('links',				shell('ip link'))
	status += format('routes',				shell('ip route'))
	status += format('neighbours',			shell('ip neigh'))
	return status



def interfaceAvailable(interface):
	return os.path.exists('/sys/class/net/'+interface)
