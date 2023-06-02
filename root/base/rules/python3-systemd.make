# -*-makefile-*-
#
# Copyright (C) 2016 by Michael Olbrich <m.olbrich@pengutronix.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_PYTHON3_SYSTEMD) += python3-systemd

#
# Paths and names
#
PYTHON3_SYSTEMD_VERSION		:= 235
PYTHON3_SYSTEMD_MD5		:= 760003406e2fac84ff1bcc40ae696af6
PYTHON3_SYSTEMD			:= python3-systemd-$(PYTHON3_SYSTEMD_VERSION)
PYTHON3_SYSTEMD_SUFFIX		:= tar.gz
PYTHON3_SYSTEMD_URL		:= https://github.com/systemd/python-systemd/archive/v$(PYTHON3_SYSTEMD_VERSION).$(PYTHON3_SYSTEMD_SUFFIX)
PYTHON3_SYSTEMD_SOURCE		:= $(SRCDIR)/$(PYTHON3_SYSTEMD).$(PYTHON3_SYSTEMD_SUFFIX)
PYTHON3_SYSTEMD_DIR		:= $(BUILDDIR)/$(PYTHON3_SYSTEMD)
PYTHON3_SYSTEMD_LICENSE		:= LGPL-2.1-or-later
PYTHON3_SYSTEMD_LICENSE_FILES	:= file://LICENSE.txt;md5=4fbd65380cdd255951079008b364516c

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

PYTHON3_SYSTEMD_CONF_TOOL	:= python3

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/python3-systemd.targetinstall:
	@$(call targetinfo)

	@$(call install_init, python3-systemd)
	@$(call install_fixup, python3-systemd,PRIORITY,optional)
	@$(call install_fixup, python3-systemd,SECTION,base)
	@$(call install_fixup, python3-systemd,AUTHOR,"Michael Olbrich <m.olbrich@pengutronix.de>")
	@$(call install_fixup, python3-systemd,DESCRIPTION,missing)

	@$(call install_glob, python3-systemd, 0, 0, -, \
		$(PYTHON3_SITEPACKAGES)/systemd,, *.py)

	@$(call install_finish, python3-systemd)

	@$(call touch)

# vim: syntax=make
