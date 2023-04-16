# -*-makefile-*-
#
# Copyright (C) 2018 by Artur Wiebe <artur@4wiebe.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_PYTHON3_PYTZ) += python3-pytz

PYTHON3_PYTZ_VERSION		:= 2023.3
PYTHON3_PYTZ_MD5		:= fe54c8f8a1544b4e78b523b264ab071b
PYTHON3_PYTZ			:= pytz-$(PYTHON3_PYTZ_VERSION)
PYTHON3_PYTZ_SUFFIX		:= tar.gz
PYTHON3_PYTZ_URL		:= $(call ptx/mirror-pypi, pytz, $(PYTHON3_PYTZ).$(PYTHON3_PYTZ_SUFFIX))
PYTHON3_PYTZ_SOURCE		:= $(SRCDIR)/$(PYTHON3_PYTZ).$(PYTHON3_PYTZ_SUFFIX)
PYTHON3_PYTZ_DIR		:= $(BUILDDIR)/$(PYTHON3_PYTZ)
PYTHON3_PYTZ_LICENSE		:= MIT
PYTHON3_PYTZ_LICENSE_FILES	:= file://LICENSE.txt;md5=1a67fc46c1b596cce5d21209bbe75999

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
		$(PYTHON3_SITEPACKAGES)/pytz,, *.py */zoneinfo*)

	@$(call install_finish,python3-pytz)

	@$(call touch)

# vim: syntax=make
