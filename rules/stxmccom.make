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
PACKAGES-$(PTXCONF_STXMCCOM) += stxmccom

#
# Paths and names
#
STXMCCOM_VERSION	:= 1
STXMCCOM			:= stxmccom-$(STXMCCOM_VERSION)
STXMCCOM_URL		:= file://$(PTXDIST_WORKSPACE)/local_src/stxmccom
STXMCCOM_DIR		:= $(BUILDDIR)/$(STXMCCOM)
STXMCCOM_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STXMCCOM_SOURCE):
#	@$(call targetinfo)
#	@$(call get, STXMCCOM)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#STXMCCOM_CONF_ENV	:= $(CROSS_ENV)

#
# cmake
#
STXMCCOM_CONF_TOOL	:= cmake

STXMCCOM_CONF_OPT	:= $(CROSS_CMAKE_USR) -DCMAKE_BUILD_TYPE=Release
#STXMCCOM_CONF_OPT	:= $(CROSS_CMAKE_USR) -DCMAKE_BUILD_TYPE=Debug

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/stxmccom.targetinstall:
	@$(call targetinfo)

	@$(call install_init, stxmccom)
	@$(call install_fixup, stxmccom, PRIORITY, optional)
	@$(call install_fixup, stxmccom, SECTION, base)
	@$(call install_fixup, stxmccom, AUTHOR, "Artur Wiebe")
	@$(call install_fixup, stxmccom, DESCRIPTION, missing)

	$(call install_copy, stxmccom, 0, 0, 0644, $(STXMCCOM_PKGDIR)/usr/lib/libstxmccom.so,   /FFS0/SSMC/STXMCCOM.O)
	$(call install_copy, stxmccom, 0, 0, 0644, $(STXMCCOM_PKGDIR)/usr/mcbasic/STXMCCOM.LIB, /FFS0/SSMC/STXMCCOM.LIB)

	@$(call install_finish, stxmccom)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/stxmccom.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, STXMCCOM)

# vim: syntax=make
