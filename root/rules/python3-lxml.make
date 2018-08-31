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
PACKAGES-$(PTXCONF_PYTHON3_LXML) += python3-lxml

PYTHON3_LXML_VERSION	:= 4.2.4
PYTHON3_LXML_MD5	:= 31239400ca4e130b69993bb3c14204b3
PYTHON3_LXML		:= lxml-$(PYTHON3_LXML_VERSION)
PYTHON3_LXML_SUFFIX	:= tar.gz
PYTHON3_LXML_URL	:= https://pypi.python.org/packages/ca/63/139b710671c1655aed3b20c1e6776118c62e9f9311152f4c6031e12a0554/$(PYTHON3_LXML).$(PYTHON3_LXML_SUFFIX)\#md5=$(PYTHON3_LXML_MD5)
PYTHON3_LXML_SOURCE	:= $(SRCDIR)/$(PYTHON3_LXML).$(PYTHON3_LXML_SUFFIX)
PYTHON3_LXML_DIR	:= $(BUILDDIR)/$(PYTHON3_LXML)
PYTHON3_LXML_LICENSE	:= BSD

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

PYTHON3_LXML_CONF_TOOL    := python3

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/python3-lxml.targetinstall:
	@$(call targetinfo)

	@$(call install_init, python3-lxml)
	@$(call install_fixup,python3-lxml,PRIORITY,optional)
	@$(call install_fixup,python3-lxml,SECTION,base)
	@$(call install_fixup,python3-lxml,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,python3-lxml,DESCRIPTION,missing)

	@$(call install_glob, python3-lxml, 0, 0, -, \
		/usr/lib/python$(PYTHON3_MAJORMINOR)/site-packages/lxml,, *.py)

	@$(call install_finish,python3-lxml)

	@$(call touch)

# vim: syntax=make
