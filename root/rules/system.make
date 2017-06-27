# -*-makefile-*-
#
# Copyright (C) 2015 by Artur Wiebe
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_SYSTEM) += system

SYSTEM_VERSION	:= 0.1

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.compile:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/system.targetinstall:
	@$(call targetinfo)
	@$(call install_init, system)
	@$(call install_fixup,system,PRIORITY,optional)
	@$(call install_fixup,system,SECTION,base)
	@$(call install_fixup,system,AUTHOR,"Artur Wiebe")
	@$(call install_fixup,system,DESCRIPTION,missing)
	
	# static machine-id
	@$(call install_alternative, system, 0, 0, 0444, /etc/machine-id)
	
	# firmware blobs
	@$(call install_alternative_tree, system, 0, 0, /usr/lib/firmware)

	# boot configuration
	@$(call install_alternative, system, 0, 0, 0644, /boot/boot.conf)

	# mountpoints
	@$(call install_copy,        system, 0, 0, 0755, /mnt/boot)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/root)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/init)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/usb)
	@$(call install_alternative, system, 0, 0, 0644, /usr/lib/systemd/system/var-etc.service)

	# networking
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/network/machine-bridge.netdev)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/network/10-machine-bridge.network)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/network/20-machine.network)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/network/99-public.network)
	@$(call install_link,        system, ../wpa_supplicant.service, /usr/lib/systemd/system/multi-user.target.wants/wpa_supplicant.service)
	
	# update
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/update-apply.service)
	@$(call install_alternative, system, 0, 0, 0644, /etc/systemd/system/update-apply.path)
	@$(call install_link,        system, ../update-apply.path, /etc/systemd/system/mnt-init.mount.wants/update-apply.path)

	# sshd root key
	@$(call install_copy,        system, 0, 0, 0700, /root/.ssh)
	@$(call install_alternative, system, 0, 0, 0600, /root/.ssh/authorized_keys)
	@$(call install_alternative, system, 0, 0, 0600, /root/.ssh/id_rsa)
	@$(call install_alternative, system, 0, 0, 0644, /root/.ssh/id_rsa.pub)
	
	# central syslog
	@$(call install_alternative, system, 0, 0, 0644, /etc/tmpfiles.d/persistent-journal.conf)

	@$(call install_copy,        system, 0, 0, 0755, /etc/app)

	@$(call install_finish,system)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, SYSTEM)

# vim: syntax=make
