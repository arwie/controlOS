# Copyright (c) 2018 Artur Wiebe <artur@4wiebe.de>
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


import os, json, collections, glob, re
from shared.conf import Conf


def _construct():
	confFile	= '/etc/app/setup.conf'
	dir			= '/usr/share/setup/'
	ext			= '.json'

	def decode(name):
		return json.load(open(dir+name+ext))

	def replaceRecursive(d, u):
		for k, v in u.items():
			if isinstance(v, collections.Mapping):
				d[k] = replaceRecursive(d.get(k, {}), v)
			else:
				d[k] = v
		return d
	
	setup = decode('setup')
	
	if os.path.exists(confFile):
		conf = Conf(confFile)
		type = conf.get('setup','type')
		rev  = conf.get('setup','revision')

		setup = replaceRecursive(setup, decode(type))

		patches = []
		for path in glob.glob(dir+type+'-*'+ext):
			match = re.match(r'.*-(\d*)\..*', path)
			if match:
				patch = int(match.group(1))
				if patch >= int(rev):
					patches.append(patch)
		patches.sort(reverse=True)
		for patch in patches:
			setup = replaceRecursive(setup, decode(type+'-'+str(patch)))

		setup = replaceRecursive(setup, conf.dict())

	return setup


setup = _construct()
