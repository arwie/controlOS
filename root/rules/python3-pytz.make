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
PACKAGES-$(PTXCONF_PYTHON3_PYTZ) += python3-pytz

PYTHON3_PYTZ_VERSION	:= 2018.5
PYTHON3_PYTZ_MD5	:= 45409cbfa3927bdd2f3ee914dd5b1060
PYTHON3_PYTZ		:= pytz-$(PYTHON3_PYTZ_VERSION)
PYTHON3_PYTZ_SUFFIX	:= tar.gz
PYTHON3_PYTZ_URL	:= https://pypi.python.org/packages/ca/a9/62f96decb1e309d6300ebe7eee9acfd7bccaeedd693794437005b9067b44/$(PYTHON3_PYTZ).$(PYTHON3_PYTZ_SUFFIX)\#md5=$(PYTHON3_PYTZ_MD5)
PYTHON3_PYTZ_SOURCE	:= $(SRCDIR)/$(PYTHON3_PYTZ).$(PYTHON3_PYTZ_SUFFIX)
PYTHON3_PYTZ_DIR	:= $(BUILDDIR)/$(PYTHON3_PYTZ)
PYTHON3_PYTZ_LICENSE	:= MIT

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

PYTHON3_PYTZ_CONF_TOOL    := python3

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/python3-pytz.targetinstall:
	@$(call targetinfo)

	@$(call install_init, python3-pytz)
	@$(call install_fixup,python3-pytz,PRIORITY,optional)
	@$(call install_fixup,python3-pytz,SECTION,base)
	@$(call install_fixup,python3-pytz,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,python3-pytz,DESCRIPTION,missing)

	@$(call install_glob, python3-pytz, 0, 0, -, \
		/usr/lib/python$(PYTHON3_MAJORMINOR)/site-packages/pytz,, *.py */zoneinfo*)

	@$(call install_finish,python3-pytz)

	@$(call touch)

# vim: syntax=make
