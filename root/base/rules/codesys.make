# -*-makefile-*-
#
# Copyright (C) 2023 by Artur Wiebe <artur@4wiebe.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_CODESYS) += codesys

CODESYS_VERSION		:= 4.7.0.0-b.trunk.39
CODESYS_MD5		:= 7ece0408b9566d1778842d82b5b21ad7
CODESYS			:= codesys-$(CODESYS_VERSION)
CODESYS_SOURCE		:= $(SRCDIR)/codesyscontrol_linux_$(CODESYS_VERSION)_amd64.ipk
CODESYS_DIR		:= $(BUILDDIR)/$(CODESYS)
CODESYS_LICENSE		:= unknown

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/codesys.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/codesys.extract:
	@$(call targetinfo)
	fakeroot opkg -o $(CODESYS_DIR) install $(CODESYS_SOURCE)
	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/codesys.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/codesys.compile:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/codesys.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/codesys.targetinstall:
	@$(call targetinfo)
	@$(call install_init, codesys)
	@$(call install_fixup,codesys,PRIORITY,optional)
	@$(call install_fixup,codesys,SECTION,base)
	@$(call install_fixup,codesys,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,codesys,DESCRIPTION,missing)

	@$(call install_tree, codesys, 0, 0, $(CODESYS_DIR)/opt, /opt)

	@$(call install_alternative, codesys, 0, 0, 0644, /etc/CODESYSControl.cfg)
	@$(call install_alternative, codesys, 0, 0, 0644, /etc/systemd/network/ethercat.network)
	@$(call install_alternative, codesys, 0, 0, 0644, /etc/tmpfiles.d/codesys.conf)

	@$(call install_alternative, codesys, 0, 0, 0644, /usr/lib/systemd/system/codesys.service)
#	@$(call install_link,        codesys, ../codesys.service, /usr/lib/systemd/system/multi-user.target.wants/codesys.service)

	@$(call install_finish,codesys)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/codesys.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, CODESYS)

# vim: syntax=make
