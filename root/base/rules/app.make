#
# We provide this package
#
PACKAGES-$(PTXCONF_APP) += app

APP_VERSION		:= 1

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/app.targetinstall:
	@$(call targetinfo)
	@$(call install_init, app)

	@$(call install_alternative_tree, app, 0, 0, /usr/lib/app)
	@$(call install_alternative_tree, app, 0, 0, /etc/app)

	@$(call install_alternative, app, 0, 0, 0644, /usr/lib/systemd/system/app.service)
	@$(call install_link,        app, ../app.service, /usr/lib/systemd/system/multi-user.target.wants/app.service)

	@$(call install_finish,app)
	@$(call touch)


# vim: syntax=make
