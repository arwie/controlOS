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
PACKAGES-$(PTXCONF_STX_MC) += stx-mc

STX_MC_VERSION	:= 1

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/stx-mc.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/stx-mc.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/stx-mc.compile:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/stx-mc.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/stx-mc.targetinstall:
	@$(call targetinfo)
	@$(call install_init, stx-mc)
	@$(call install_fixup, stx-mc,PRIORITY,optional)
	@$(call install_fixup, stx-mc,SECTION,base)
	@$(call install_fixup, stx-mc,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup, stx-mc,DESCRIPTION,missing)

	@$(call install_alternative_tree, stx-mc, 0, 0, /usr/lib/mc)
	
	@$(call install_alternative, stx-mc, 0, 0, 0755, /usr/bin/mc-update)
	@$(call install_alternative, stx-mc, 0, 0, 0644, /etc/systemd/system/mc-update.service)
	@$(call install_link,        stx-mc, ../mc-update.service, /etc/systemd/system/multi-user.target.wants/mc-update.service)
	
	@$(call install_alternative, stx-mc, 0, 0, 0755, /usr/sbin/mc-state)
	@$(call install_alternative, stx-mc, 0, 0, 0644, /lib/systemd/system/mc-state@.target)
	@$(call install_alternative, stx-mc, 0, 0, 0644, /lib/systemd/system/mc-state.service)
	@$(call install_link,        stx-mc, ../mc-state.service, /lib/systemd/system/multi-user.target.wants/mc-state.service)
	
	@$(call install_alternative, stx-mc, 0, 0, 0755, /usr/sbin/mc-log)
	@$(call install_alternative, stx-mc, 0, 0, 0644, /lib/systemd/system/mc-log.service)
	@$(call install_link,        stx-mc, ../mc-log.service, /lib/systemd/system/mc-state@log.target.wants/mc-log.service)

	@$(call install_finish, stx-mc)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/stx-mc.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, STX_MC)

# vim: syntax=make
