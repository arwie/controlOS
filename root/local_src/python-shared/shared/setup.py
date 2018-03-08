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
