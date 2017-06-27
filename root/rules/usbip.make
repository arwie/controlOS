# -*-makefile-*-
#
# Copyright (C) 2016 by Artur Wiebe <artur@4wiebe.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_USBIP) += usbip

#
# Paths and names
#
USBIP_VERSION	:= 1
USBIP			:= usbip-$(USBIP_VERSION)
USBIP_URL		:= file://$(BUILDDIR)/linux-$(KERNEL_VERSION)/tools/usb/usbip
USBIP_DIR		:= $(BUILDDIR)/$(USBIP)
USBIP_BUILD_OOT	:= YES
USBIP_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

$(STATEDIR)/usbip.extract: $(STATEDIR)/autogen-tools

$(STATEDIR)/usbip.extract:
	@$(call targetinfo)
	@$(call clean, $(USBIP_DIR))
	@$(call extract, USBIP)
	cd $(USBIP_DIR) && [ -f configure ] || sh autogen.sh
	@$(call patchin, USBIP)
	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#USBIP_CONF_ENV	:= $(CROSS_ENV)

#
# autoconf
#
USBIP_CONF_TOOL	:= autoconf
#USBIP_CONF_OPT	:= $(CROSS_AUTOCONF_USR)

USBIP_CFLAGS := -I$(KERNEL_HEADERS_INCLUDE_DIR)

#$(STATEDIR)/usbip.prepare:
#	@$(call targetinfo)
#	@$(call world/prepare, USBIP)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/usbip.compile:
#	@$(call targetinfo)
#	@$(call world/compile, USBIP)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/usbip.install:
#	@$(call targetinfo)
#	@$(call world/install, USBIP)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/usbip.targetinstall:
	@$(call targetinfo)
	@$(call install_init, usbip)
	@$(call install_fixup, usbip, PRIORITY, optional)
	@$(call install_fixup, usbip, SECTION, base)
	@$(call install_fixup, usbip, AUTHOR, "Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup, usbip, DESCRIPTION, missing)

	@$(call install_lib,  usbip, 0, 0, 644, libusbip)
	
	@$(call install_copy, usbip, 0, 0, 755, -, /usr/sbin/usbipd)
	@$(call install_copy, usbip, 0, 0, 755, -, /usr/sbin/usbip)
	
	@$(call install_alternative, usbip, 0, 0, 0644, /usr/lib/systemd/system/usbipd.service)

	@$(call install_finish, usbip)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/usbip.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, USBIP)

# vim: syntax=make
