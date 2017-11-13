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


import subprocess
from tornado import gen, process



@gen.coroutine
def store():
	rpoc = process.Subprocess(['/usr/bin/tar', '-cJ', '-C/var/etc', '.'], stdout=process.Subprocess.STREAM)
	backup = yield rpoc.stdout.read_until_close()
	yield rpoc.wait_for_exit(raise_error=False)
	return backup


@gen.coroutine
def restore(backup):
	subprocess.run(['/usr/bin/find', '/var/etc', '-delete', '-mindepth', '1'])
	subprocess.run(['/usr/bin/tar', '-xJ', '-C/var/etc'], input=backup)
	subprocess.run(['/usr/bin/systemctl', '--no-block', 'reboot'])

