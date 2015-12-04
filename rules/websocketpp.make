# -*-makefile-*-
#
# Copyright (C) 2015 by Artur Wiebe <artur@4wiebe.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_WEBSOCKETPP) += websocketpp

WEBSOCKETPP_VERSION	:= 0.6.0
WEBSOCKETPP			:= WEBSOCKETPP-$(WEBSOCKETPP_VERSION)
WEBSOCKETPP_URL		:= file://$(PTXDIST_WORKSPACE)/local_src/websocketpp
WEBSOCKETPP_DIR		:= $(BUILDDIR)/$(WEBSOCKETPP)

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/websocketpp.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/websocketpp.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/websocketpp.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/websocketpp.compile:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/websocketpp.install:
	@$(call targetinfo)

	cp -rL $(WEBSOCKETPP_DIR)/ $(PTXDIST_SYSROOT_TARGET)/usr/include/websocketpp

	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/websocketpp.targetinstall:
	@$(call targetinfo)
#
# TODO: To build your own package, if this step requires one
#	@$(call install_init, websocketpp)
#	@$(call install_fixup,websocketpp,PRIORITY,optional)
#	@$(call install_fixup,websocketpp,SECTION,base)
#	@$(call install_fixup,websocketpp,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
#	@$(call install_fixup,websocketpp,DESCRIPTION,missing)
#
# TODO: Add here all files that should be copied to the target
# Note: Add everything before(!) call to macro install_finish
#
#	@$(call install_copy, websocketpp, 0, 0, 0755, $(WEBSOCKETPP_DIR)/foobar, /dev/null)
#
#	@$(call install_finish,websocketpp)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/websocketpp.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, WEBSOCKETPP)

# vim: syntax=make
