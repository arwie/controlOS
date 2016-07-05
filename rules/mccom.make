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
PACKAGES-$(PTXCONF_MCCOM) += mccom

#
# Paths and names
#
MCCOM_VERSION	:= 1
MCCOM			:= mccom-$(MCCOM_VERSION)
MCCOM_URL		:= file://$(PTXDIST_WORKSPACE)/local_src/mccom
MCCOM_DIR		:= $(BUILDDIR)/$(MCCOM)
MCCOM_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(MCCOM_SOURCE):
#	@$(call targetinfo)
#	@$(call get, MCCOM)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#MCCOM_CONF_ENV	:= $(CROSS_ENV)

#
# cmake
#
MCCOM_CONF_TOOL	:= cmake

MCCOM_CONF_OPT	:= $(CROSS_CMAKE_USR) -DCMAKE_BUILD_TYPE=Release
#MCCOM_CONF_OPT	:= $(CROSS_CMAKE_USR) -DCMAKE_BUILD_TYPE=Debug

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/mccom.targetinstall:
	@$(call targetinfo)

	@$(call install_init, mccom)
	@$(call install_fixup, mccom, PRIORITY, optional)
	@$(call install_fixup, mccom, SECTION, base)
	@$(call install_fixup, mccom, AUTHOR, "Artur Wiebe")
	@$(call install_fixup, mccom, DESCRIPTION, missing)

	$(call install_copy, mccom, 0, 0, 0644, -, /FFS0/SSMC/MCCOM.O)
	$(call install_copy, mccom, 0, 0, 0644, -, /FFS0/SSMC/MCCOM.LIB)

	@$(call install_finish, mccom)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/mccom.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, MCCOM)

# vim: syntax=make
