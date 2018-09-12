# -*-makefile-*-
#
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

#
# We provide this package
#
PACKAGES-$(PTXCONF_SYSTEM) += system

SYSTEM_VERSION	:= 0.1

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/system.targetinstall:
	@$(call targetinfo)
	@$(call install_init, system)
	
	# basics
	@$(call install_alternative, system, 0, 0, 0644, /boot/boot.conf)
	@$(call install_alternative, system, 0, 0, 0444, /etc/machine-id)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/etc.mount)
	
	# firmware blobs
	@$(call install_alternative_tree, system, 0, 0, /usr/lib/firmware)

	# networking
	@$(call install_alternative_tree, system, 0, 0, /etc/systemd/network)
	@$(call install_alternative, system, 0, 0, 0644, /etc/hostapd.conf)
	@$(call install_alternative_tree, system, 0, 0,  /etc/hostapd.conf.d)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/hostapd.service)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/sys-subsystem-net-devices-syswlan.device)
	@$(call install_alternative_tree, system, 0, 0,  /etc/wpa_supplicant.conf.d)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/sys-subsystem-net-devices-wlan.device)
	
	# update
	@$(call install_copy,        system, 0, 0, 0755, /mnt/init)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/update-apply.service)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/update-apply.path)
	@$(call install_link,        system, ../update-apply.path, /etc/systemd/system/mnt-init.mount.wants/update-apply.path)

	# sshd root key
	@$(call install_copy,        system, 0, 0, 0700, /root/.ssh)
	@$(call install_alternative, system, 0, 0, 0600, /root/.ssh/authorized_keys)
	@$(call install_alternative, system, 0, 0, 0600, /root/.ssh/id_rsa)
	@$(call install_alternative, system, 0, 0, 0644, /root/.ssh/id_rsa.pub)
	@$(call install_alternative, system, 0, 0, 0600, /etc/ssh/ssh_host_rsa_key)
	@$(call install_alternative, system, 0, 0, 0644, /etc/ssh/ssh_host_rsa_key.pub)

	# development tools
	@$(call install_alternative, system, 0, 0, 0644, /root/.profile)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/remote.service)
	
	# journal helpers
	@$(call install_alternative, system, 0, 0, 0644, /etc/tmpfiles.d/persistent-journal.conf)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/systemd-journald.service.d/persistent-journal.conf)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/journal-cleanup.service)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/journal-cleanup.timer)
	@$(call install_link,        system, ../journal-cleanup.timer, /usr/lib/systemd/system/timers.target.wants/journal-cleanup.timer)

	@$(call install_alternative_tree, system, 0, 0, /usr/share/setup)
	@$(call install_alternative, system, 0, 0, 0644, /etc/issue.conf)
	@$(call install_copy,        system, 0, 0, 0755, /etc/app)

	@$(call install_finish,system)
	@$(call touch)


# vim: syntax=make
