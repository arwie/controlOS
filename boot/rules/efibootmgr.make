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
# Prepare
# ----------------------------------------------------------------------------

EFIBOOTMGR_CONF_TOOL	:= NO
EFIBOOTMGR_MAKE_ENV	:= $(CROSS_ENV)
EFIBOOTMGR_MAKE_OPT	:= EFIDIR="/boot/EFI"
EFIBOOTMGR_INSTALL_OPT	:= EFIDIR="/boot/EFI" install

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


# vim: syntax=make
