# -*-makefile-*-
#
# Copyright (C) 2017 by Artur Wiebe <artur@4wiebe.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_VERSION) += version

VERSION_VERSION	:= 1

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/version.targetinstall: FORCE
	@$(call targetinfo)
	@$(call install_init, version)
	@$(call install_fixup,version,PRIORITY,optional)
	@$(call install_fixup,version,SECTION,base)
	@$(call install_fixup,version,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,version,DESCRIPTION,missing)

	@$(call install_alternative, version, 0, 0, 0644, /version-id)
	@$(call install_replace,     version, /version-id, @ID@, $(shell date +%Y%m%d%H%M))

	@$(call install_finish,version)
	@$(call touch)

FORCE:



# vim: syntax=make
