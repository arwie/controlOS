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
PACKAGES-$(PTXCONF_PYTHON3_SORTEDCONTAINERS) += python3-sortedcontainers

#
# Paths and names
#
PYTHON3_SORTEDCONTAINERS_VERSION	:= 2.4.0
PYTHON3_SORTEDCONTAINERS_MD5		:= 50eeb6cb739568b590b28f9a3f445c78
PYTHON3_SORTEDCONTAINERS		:= sortedcontainers-$(PYTHON3_SORTEDCONTAINERS_VERSION)
PYTHON3_SORTEDCONTAINERS_SUFFIX		:= tar.gz
PYTHON3_SORTEDCONTAINERS_URL		:= $(call ptx/mirror-pypi, sortedcontainers, $(PYTHON3_SORTEDCONTAINERS).$(PYTHON3_SORTEDCONTAINERS_SUFFIX))
PYTHON3_SORTEDCONTAINERS_SOURCE		:= $(SRCDIR)/$(PYTHON3_SORTEDCONTAINERS).$(PYTHON3_SORTEDCONTAINERS_SUFFIX)
PYTHON3_SORTEDCONTAINERS_DIR		:= $(BUILDDIR)/$(PYTHON3_SORTEDCONTAINERS)
PYTHON3_SORTEDCONTAINERS_LICENSE	:= Apache-2.0
PYTHON3_SORTEDCONTAINERS_LICENSE_FILES	:= file://LICENSE;md5=7c7c6a1a12ec816da16c1839137d53ae

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

PYTHON3_SORTEDCONTAINERS_CONF_TOOL	:= python3

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/python3-sortedcontainers.targetinstall:
	@$(call targetinfo)

	@$(call install_init, python3-sortedcontainers)
	@$(call install_fixup, python3-sortedcontainers,PRIORITY,optional)
	@$(call install_fixup, python3-sortedcontainers,SECTION,base)
	@$(call install_fixup, python3-sortedcontainers,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup, python3-sortedcontainers,DESCRIPTION,missing)

	@$(call install_glob, python3-sortedcontainers, 0, 0, -, \
		$(PYTHON3_SITEPACKAGES)/sortedcontainers,, *.py)

	@$(call install_finish, python3-sortedcontainers)

	@$(call touch)

# vim: syntax=make
