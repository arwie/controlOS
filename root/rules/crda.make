# -*-makefile-*-
#
# Copyright (C) 2012 by Jan Luebbe <jlu@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_CRDA) += crda

#
# Paths and names
#
CRDA_VERSION	:= 3.18
CRDA_MD5	:= 0431fef3067bf503dfb464069f06163a
CRDA		:= crda-$(CRDA_VERSION)
CRDA_SUFFIX	:= tar.xz
CRDA_URL	:= $(call ptx/mirror, KERNEL, ../software/network/crda/$(CRDA).$(CRDA_SUFFIX))
CRDA_SOURCE	:= $(SRCDIR)/$(CRDA).$(CRDA_SUFFIX)
CRDA_DIR	:= $(BUILDDIR)/$(CRDA)
CRDA_LICENSE	:= ISC, copyleft-next-0.3.0
CRDA_LICENSE_FILES := \
	file://LICENSE;md5=ef8b69b43141352d821fd66b64ff0ee7 \
	file://copyleft-next-0.3.0;md5=8743a2c359037d4d329a31e79eabeffe

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/crda.prepare:
	@$(call targetinfo)
ifdef PTXCONF_ARCH_LP64
	@cp $(CRDA_DIR)/keys-ssl.c.64 $(CRDA_DIR)/keys-ssl.c
else
	@cp $(CRDA_DIR)/keys-ssl.c.32 $(CRDA_DIR)/keys-ssl.c
endif
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

CRDA_MAKE_ENV	:= \
	$(CROSS_ENV) \
	LIBDIR=/usr/lib \
	SBINDIR=/usr/sbin/ \
	UDEV_RULE_DIR=/usr/lib/udev/rules.d/ \
	USE_OPENSSL=1

CRDA_MAKE_OPT	:= all_noverify

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/crda.targetinstall:
	@$(call targetinfo)

	@$(call install_init, crda)
	@$(call install_fixup, crda,PRIORITY,optional)
	@$(call install_fixup, crda,SECTION,base)
	@$(call install_fixup, crda,AUTHOR,"Jan Luebbe <jlu@pengutronix.de>")
	@$(call install_fixup, crda,DESCRIPTION,missing)

	@$(call install_copy, crda, 0, 0, 0755, -, /usr/sbin/crda)
	@$(call install_copy, crda, 0, 0, 0755, -, /usr/sbin/regdbdump)
	@$(call install_copy, crda, 0, 0, 0644, -, \
		/usr/lib/udev/rules.d/85-regulatory.rules)
	@$(call install_lib, crda, 0, 0, 0644, libreg)

	# regulatory.bin was downloaded from:
	# https://git.kernel.org/cgit/linux/kernel/git/sforshee/wireless-regdb.git/plain/regulatory.bin
	@$(call install_alternative, crda, 0, 0, 0644, \
		/usr/lib/crda/regulatory.bin)

	@$(call install_finish, crda)

	@$(call touch)

# vim: syntax=make
