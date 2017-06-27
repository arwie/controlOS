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
PACKAGES-$(PTXCONF_GUI) += gui

GUI_VERSION	:= 1

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/gui.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/gui.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/gui.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/gui.compile:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/gui.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/gui.targetinstall:
	@$(call targetinfo)
	@$(call install_init, gui)
	@$(call install_fixup,gui,PRIORITY,optional)
	@$(call install_fixup,gui,SECTION,base)
	@$(call install_fixup,gui,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,gui,DESCRIPTION,missing)

	@$(call install_alternative_tree, gui, 0, 0, /usr/lib/gui)
	
	@$(call install_alternative, gui, 0, 0, 0644, /usr/lib/systemd/system/gui.service)
	@$(call install_link,        gui, ../gui.service, /usr/lib/systemd/system/multi-user.target.wants/gui.service)
	
	@$(call install_finish,gui)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/gui.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, GUI)

# vim: syntax=make
