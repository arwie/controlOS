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
PACKAGES-$(PTXCONF_APP) += app

APP_VERSION	:= 1

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.compile:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/app.targetinstall:
	@$(call targetinfo)

	@$(call install_init, app)
	@$(call install_fixup,app,PRIORITY,optional)
	@$(call install_fixup,app,SECTION,base)
	@$(call install_fixup,app,AUTHOR,"Artur Wiebe <artur@4wiebe.de>")
	@$(call install_fixup,app,DESCRIPTION,missing)

	@$(call install_alternative, app, 0, 0, 0644, /etc/hostapd.conf)
	@$(call install_alternative, app, 0, 0, 0644, /etc/systemd/system/hostapd.service)
	@$(call install_link,        app, ../hostapd.service, /etc/systemd/system/multi-user.target.wants/hostapd.service)

	@$(call install_finish,app)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/app.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, APP)

# vim: syntax=make
