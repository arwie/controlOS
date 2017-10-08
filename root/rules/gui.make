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
	@$(call install_alternative, gui, 0, 0, 0644, /usr/lib/systemd/system/gui.socket)
	@$(call install_link,        gui, ../gui.socket, /usr/lib/systemd/system/sockets.target.wants/gui.socket)
	
	@$(call install_alternative, gui, 0, 0, 0644, /usr/lib/systemd/system/studio.service)
	@$(call install_alternative, gui, 0, 0, 0644, /usr/lib/systemd/system/studio.socket)
	
	@$(call install_finish,gui)
	@$(call touch)


# vim: syntax=make
