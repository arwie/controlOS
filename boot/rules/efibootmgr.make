# -*-makefile-*-
#
# Copyright (C) 2018 by Artur Wiebe <artur@4wiebe.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_EFIBOOTMGR) += efibootmgr

#
# Paths and names
#
EFIBOOTMGR_VERSION	:= 16
EFIBOOTMGR_MD5		:= ab7cf46774fda951a0e8a40beb65a90e
EFIBOOTMGR		:= efibootmgr-$(EFIBOOTMGR_VERSION)
EFIBOOTMGR_SUFFIX	:= tar.bz2
EFIBOOTMGR_URL		:= https://github.com/rhboot/efibootmgr/releases/download/$(EFIBOOTMGR_VERSION)/$(EFIBOOTMGR).$(EFIBOOTMGR_SUFFIX)
EFIBOOTMGR_SOURCE	:= $(SRCDIR)/$(EFIBOOTMGR).$(EFIBOOTMGR_SUFFIX)
EFIBOOTMGR_DIR		:= $(BUILDDIR)/$(EFIBOOTMGR)
EFIBOOTMGR_LICENSE	:= GPL-2.0-only

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(EFIBOOTMGR_SOURCE):
#	@$(call targetinfo)
#	@$(call get, EFIBOOTMGR)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

EFIBOOTMGR_MAKE_ENV	:= $(CROSS_ENV)
EFIBOOTMGR_MAKE_OPT	:= EFIDIR="/boot/EFI"
EFIBOOTMGR_INSTALL_OPT	:= EFIDIR="/boot/EFI" install

#
# autoconf
#
EFIBOOTMGR_CONF_TOOL	:= NO
#EFIBOOTMGR_CONF_OPT	:= $(CROSS_AUTOCONF_USR)

#$(STATEDIR)/efibootmgr.prepare:
#	@$(call targetinfo)
#	@$(call clean, $(EFIBOOTMGR_DIR)/config.cache)
#	cd $(EFIBOOTMGR_DIR) && \
#		$(EFIBOOTMGR_PATH) $(EFIBOOTMGR_ENV) \
#		./configure $(EFIBOOTMGR_CONF_OPT)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/efibootmgr.compile:
#	@$(call targetinfo)
#	@$(call world/compile, EFIBOOTMGR)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/efibootmgr.install:
#	@$(call targetinfo)
#	@$(call world/install, EFIBOOTMGR)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/efibootmgr.targetinstall:
	@$(call targetinfo)

	@$(call install_init, efibootmgr)
	@$(call install_fixup, efibootmgr,PRIORITY,optional)
	@$(call install_fixup, efibootmgr,SECTION,base)
	@$(call install_fixup, efibootmgr,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup, efibootmgr,DESCRIPTION,missing)

	@$(call install_copy, efibootmgr, 0, 0, 0755, -, /usr/sbin/efibootmgr)

	@$(call install_finish, efibootmgr)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/efibootmgr.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, EFIBOOTMGR)

# vim: syntax=make
