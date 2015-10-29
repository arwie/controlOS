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
PACKAGES-$(PTXCONF_STXMCWS) += stxmcws

#
# Paths and names
#
STXMCWS_VERSION	:= 1
STXMCWS			:= stxmcws-$(STXMCWS_VERSION)
STXMCWS_URL		:= file://$(PTXDIST_WORKSPACE)/local_src/stxmcws
STXMCWS_DIR		:= $(BUILDDIR)/$(STXMCWS)
STXMCWS_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STXMCWS_SOURCE):
#	@$(call targetinfo)
#	@$(call get, STXMCWS)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#STXMCWS_CONF_ENV	:= $(CROSS_ENV)

#
# cmake
#
STXMCWS_CONF_TOOL	:= cmake
#STXMCWS_CONF_OPT	:= $(CROSS_CMAKE_USR)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/stxmcws.targetinstall:
	@$(call targetinfo)

	@$(call install_init, stxmcws)
	@$(call install_fixup, stxmcws, PRIORITY, optional)
	@$(call install_fixup, stxmcws, SECTION, base)
	@$(call install_fixup, stxmcws, AUTHOR, "Artur Wiebe")
	@$(call install_fixup, stxmcws, DESCRIPTION, missing)

#	#
#	# example code:; copy all binaries
#	#

	@for i in $(shell cd $(STXMCWS_PKGDIR) && find bin sbin usr/bin usr/sbin -type f); do \
		$(call install_copy, stxmcws, 0, 0, 0755, -, /$$i); \
	done

#	#
#	# FIXME: add all necessary things here
#	#

	@$(call install_finish, stxmcws)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/stxmcws.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, STXMCWS)

# vim: syntax=make
