# SPDX-FileCopyrightText: 2026 Artur Wiebe <artur@4wiebe.de>
# SPDX-License-Identifier: MIT


trap 'sleep 10; reboot -f' EXIT
set -e


mount -t devtmpfs none /dev
mount -t proc     none /proc
mount -t sysfs    none /sys


CRYPTKEY_FILE=/tmp/crypt.key



waitfs()
{
	TIMEOUT=${2-100}

	while ! (findfs PARTLABEL=$1 2>/dev/null || findfs LABEL=$1 2>/dev/null)
	do
		[ $(( --TIMEOUT )) -gt 0 ] || return 1
		sleep 0.1
	done
}


dmcrypt_setup()
(
	for name in root data
	do
		echo "### setup $name crypt container"

		KEY=$(xxd -p -c128 $CRYPTKEY_FILE)
		DEV=$(waitfs $name)
		SIZE=$(blockdev --getsz $DEV)

		dmsetup create $name <<- +++
			0 $SIZE crypt aes-xts-plain64 $KEY 0 $DEV 0
		+++
	done
)