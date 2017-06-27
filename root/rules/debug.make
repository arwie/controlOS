# -*-makefile-*-
#
# Copyright (C) 2016 by Artur Wiebe <artur@4wiebe.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_DEBUG) += debug

DEBUG_VERSION	:= 1

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.compile:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/debug.targetinstall:
	@$(call targetinfo)
	@$(call install_init, debug)
	@$(call install_fixup,debug,PRIORITY,optional)
	@$(call install_fixup,debug,SECTION,base)
	@$(call install_fixup,debug,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,debug,DESCRIPTION,missing)

	$(call install_alternative, debug, 0, 0, 0644, /etc/systemd/system/mount-root-rw.service)

	$(call install_alternative, debug, 0, 0, 0644, /etc/systemd/system/debug.target)
	$(call install_link,        debug, ../debug.target, /usr/lib/systemd/system/sshd@.service.wants/debug.target)

	$(call install_alternative, debug, 0, 0, 0644, /etc/systemd/system/remote.service)

	@$(call install_finish,debug)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/debug.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, DEBUG)

# vim: syntax=make
