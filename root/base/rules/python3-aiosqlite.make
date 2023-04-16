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
PACKAGES-$(PTXCONF_PYTHON3_AIOSQLITE) += python3-aiosqlite

#
# Paths and names
#
PYTHON3_AIOSQLITE_VERSION	:= 0.18.0
PYTHON3_AIOSQLITE_MD5		:= 1ebed736d07d246f473d0e09a5d1cbf7
PYTHON3_AIOSQLITE		:= aiosqlite-$(PYTHON3_AIOSQLITE_VERSION)
PYTHON3_AIOSQLITE_SUFFIX	:= tar.gz
PYTHON3_AIOSQLITE_URL		:= $(call ptx/mirror-pypi, aiosqlite, $(PYTHON3_AIOSQLITE).$(PYTHON3_AIOSQLITE_SUFFIX))
PYTHON3_AIOSQLITE_SOURCE	:= $(SRCDIR)/$(PYTHON3_AIOSQLITE).$(PYTHON3_AIOSQLITE_SUFFIX)
PYTHON3_AIOSQLITE_DIR		:= $(BUILDDIR)/$(PYTHON3_AIOSQLITE)
PYTHON3_AIOSQLITE_LICENSE	:= MIT
PYTHON3_AIOSQLITE_LICENSE_FILES	:= file://LICENSE;md5=f0c422eaa1f23d09f8203dc0af3e2d54

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

PYTHON3_AIOSQLITE_CONF_TOOL	:= python3

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/python3-aiosqlite.targetinstall:
	@$(call targetinfo)

	@$(call install_init, python3-aiosqlite)
	@$(call install_fixup, python3-aiosqlite,PRIORITY,optional)
	@$(call install_fixup, python3-aiosqlite,SECTION,base)
	@$(call install_fixup, python3-aiosqlite,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup, python3-aiosqlite,DESCRIPTION,missing)

	@$(call install_glob, python3-aiosqlite, 0, 0, -, \
		$(PYTHON3_SITEPACKAGES)/aiosqlite,, *.py)

	@$(call install_finish, python3-aiosqlite)

	@$(call touch)

# vim: syntax=make
