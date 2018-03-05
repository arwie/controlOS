import json, collections, glob, re
from shared.conf import Conf


def _construct():
	conf = Conf('/etc/app/setup.conf')
	dir  = '/usr/share/setup/'
	ext  = '.json'
	type = conf.get('setup','type')
	rev  = conf.get('setup','revision')

	def decode(name):
		return json.load(open(dir+name+ext))

	def replaceRecursive(d, u):
		for k, v in u.items():
			if isinstance(v, collections.Mapping):
				d[k] = replaceRecursive(d.get(k, {}), v)
			else:
				d[k] = v
		return d

	setup = replaceRecursive(decode('setup'), decode(type))

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

	return replaceRecursive(setup, conf.dict())



setup = _construct()
