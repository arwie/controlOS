# -*-makefile-*-
#
# Copyright (C) 2007 by Sascha Hauer
#               2008, 2009 by Marc Kleine-Budde
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_BAREBOX) += barebox

#
# Paths and names
#
BAREBOX_VERSION	:= $(call remove_quotes,$(PTXCONF_BAREBOX_VERSION))
BAREBOX_MD5	:= $(call remove_quotes,$(PTXCONF_BAREBOX_MD5))
BAREBOX		:= barebox-$(BAREBOX_VERSION)
BAREBOX_SUFFIX	:= tar.bz2
BAREBOX_URL	:= $(call barebox-url, BAREBOX)
BAREBOX_SOURCE	:= $(SRCDIR)/$(BAREBOX).$(BAREBOX_SUFFIX)
BAREBOX_DIR	:= $(BUILDDIR)/$(BAREBOX)
BAREBOX_LICENSE	:= GPL-2.0

BAREBOX_CONFIG	:= $(call remove_quotes, $(PTXDIST_PLATFORMCONFIGDIR)/$(PTXCONF_BAREBOX_CONFIG))

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

BAREBOX_WRAPPER_BLACKLIST := \
	TARGET_HARDEN_RELRO \
	TARGET_HARDEN_BINDNOW \
	TARGET_HARDEN_PIE \
	TARGET_DEBUG

BAREBOX_ENV := \
	KCONFIG_NOTIMESTAMP=1 \
	pkg_wrapper_blacklist="$(BAREBOX_WRAPPER_BLACKLIST)"

BAREBOX_MAKEVARS := \
	V=$(PTXDIST_VERBOSE) \
	HOSTCC=$(HOSTCC) \
	ARCH=efi \
	CROSS_COMPILE=$(BOOTLOADER_CROSS_COMPILE) \
	$(PARALLELMFLAGS)

BAREBOX_TAGS_OPT := TAGS tags cscope

ifdef PTXCONF_BAREBOX
$(BAREBOX_CONFIG):
	@echo
	@echo "****************************************************************************"
	@echo "**** Please generate a barebox config with 'ptxdist menuconfig barebox' ****"
	@echo "****************************************************************************"
	@echo
	@echo
	@exit 1
endif

ifdef PTXCONF_BAREBOX_EXTRA_ENV_PATH
$(STATEDIR)/barebox.prepare: $(call remove_quotes,$(PTXCONF_BAREBOX_EXTRA_ENV_PATH))
$(STATEDIR)/barebox.prepare: $(shell find $(call remove_quotes,$(PTXCONF_BAREBOX_EXTRA_ENV_PATH)) -print 2>/dev/null)
endif

$(STATEDIR)/barebox.prepare: $(BAREBOX_CONFIG)
	@$(call targetinfo)

	@echo "Using barebox config file: $(<)"
	@install -m 644 "$(<)" "$(BAREBOX_DIR)/.config"

	@$(call ptx/oldconfig, BAREBOX)
	@diff -q -I "# [^C]" "$(BAREBOX_DIR)/.config" "$(<)" > /dev/null || cp "$(BAREBOX_DIR)/.config" "$(<)"

ifdef PTXCONF_BAREBOX_EXTRA_ENV
	@rm -rf $(BAREBOX_DIR)/.ptxdist-defaultenv
	@ptxd_source_kconfig "${PTXDIST_PTXCONFIG}" && \
	ptxd_source_kconfig "${PTXDIST_PLATFORMCONFIG}" && \
	$(foreach path, $(call remove_quotes,$(PTXCONF_BAREBOX_EXTRA_ENV_PATH)), \
		if [ -d "$(path)" ]; then \
			ptxd_filter_dir "$(path)" \
			$(BAREBOX_DIR)/.ptxdist-defaultenv; \
		else \
			cp "$(path)" $(BAREBOX_DIR)/.ptxdist-defaultenv/; \
		fi;)
	@sed -i -e "s,^\(CONFIG_DEFAULT_ENVIRONMENT_PATH=.*\)\"$$,\1 .ptxdist-defaultenv\"," \
		$(BAREBOX_DIR)/.config
endif

	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/barebox.compile:
	@$(call targetinfo)
	@+cd $(BAREBOX_DIR) && $(BAREBOX_PATH) $(BAREBOX_ENV) \
		$(MAKE) $(BAREBOX_MAKEVARS)
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

BAREBOX_PROGS_HOST := \
	bareboxenv \
	kernel-install \
	bareboxcrc32 \
	bareboximd \
	setupmbr/setupmbr

BAREBOX_PROGS_TARGET_y :=
BAREBOX_PROGS_TARGET_$(PTXCONF_BAREBOX_BAREBOXENV) += bareboxenv
BAREBOX_PROGS_TARGET_$(PTXCONF_BAREBOX_KERNEL_INSTALL) += kernel-install
BAREBOX_PROGS_TARGET_$(PTXCONF_BAREBOX_BAREBOXCRC32) += bareboxcrc32
BAREBOX_PROGS_TARGET_$(PTXCONF_BAREBOX_BAREBOXIMD) += bareboximd

$(STATEDIR)/barebox.install:
	@$(call targetinfo)

	@$(foreach prog, $(BAREBOX_PROGS_HOST), \
		if [ -e $(BAREBOX_DIR)/scripts/$(prog) ]; then \
			install -v -D -m755 $(BAREBOX_DIR)/scripts/$(prog) \
				$(PTXCONF_SYSROOT_HOST)/bin/$(notdir $(prog)) || exit; \
		fi;)

	@$(foreach prog, $(BAREBOX_PROGS_TARGET_y), \
		install -v -D -m755 $(BAREBOX_DIR)/scripts/$(prog)-target \
			$(BAREBOX_PKGDIR)/usr/bin/$(prog) || exit;)

	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/barebox.targetinstall:
	@$(call targetinfo)

ifneq ($(strip $(BAREBOX_PROGS_TARGET_y)),)
	@$(call install_init, barebox)
	@$(call install_fixup, barebox,PRIORITY,optional)
	@$(call install_fixup, barebox,SECTION,base)
	@$(call install_fixup, barebox,AUTHOR,"Robert Schwebel <r.schwebel@pengutronix.de>")
	@$(call install_fixup, barebox,DESCRIPTION,missing)

	@$(foreach prog, $(BAREBOX_PROGS_TARGET_y), \
		$(call install_copy, barebox, 0, 0, 0755, -, \
			/usr/bin/$(prog));)

	@$(call install_finish, barebox)
endif

	@rm -f $(IMAGEDIR)/barebox-image
	@find $(BAREBOX_DIR)/images/ -name "barebox-*.img" | sort | while read image; do \
		install -D -m644 $$image $(IMAGEDIR)/`basename $$image`; \
		if [ ! -e $(IMAGEDIR)/barebox-image ]; then \
			ln -sf `basename $$image` $(IMAGEDIR)/barebox-image; \
		fi; \
	done
	@if [ -e $(IMAGEDIR)/barebox-image ]; then \
		:; \
	elif [ -e $(BAREBOX_DIR)/barebox-flash-image ]; then \
		install -D -m644 $(BAREBOX_DIR)/barebox-flash-image $(IMAGEDIR)/barebox-image; \
	else \
		install -D -m644 $(BAREBOX_DIR)/barebox.bin $(IMAGEDIR)/barebox-image; \
	fi
	@if [ -e $(BAREBOX_DIR)/defaultenv/barebox_zero_env ]; then \
		install -D -m644 $(BAREBOX_DIR)/defaultenv/barebox_zero_env $(IMAGEDIR)/barebox-default-environment; \
	elif [ -e $(BAREBOX_DIR)/common/barebox_default_env ]; then \
		install -D -m644 $(BAREBOX_DIR)/common/barebox_default_env $(IMAGEDIR)/barebox-default-environment; \
	elif [ -e $(BAREBOX_DIR)/barebox_default_env ]; then \
		install -D -m644 $(BAREBOX_DIR)/barebox_default_env $(IMAGEDIR)/barebox-default-environment; \
	fi
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/barebox.clean:
	@$(call targetinfo)
	@$(call clean_pkg, BAREBOX)
	@$(foreach prog, $(BAREBOX_PROGS_HOST), \
		rm -rf $(PTXCONF_SYSROOT_HOST)/bin/$(notdir $(prog));)
	rm -rf $(IMAGEDIR)/barebox-image $(IMAGEDIR)/barebox-default-environment

# ----------------------------------------------------------------------------
# oldconfig / menuconfig
# ----------------------------------------------------------------------------

barebox_oldconfig barebox_menuconfig barebox_nconfig: $(STATEDIR)/barebox.extract
	@if test -e $(BAREBOX_CONFIG); then \
		cp $(BAREBOX_CONFIG) $(BAREBOX_DIR)/.config; \
	fi
	@cd $(BAREBOX_DIR) && \
		$(BAREBOX_PATH) $(BAREBOX_ENV) $(MAKE) $(BAREBOX_MAKEVARS) $(subst barebox_,,$@)
	@if cmp -s $(BAREBOX_DIR)/.config $(BAREBOX_CONFIG); then \
		echo "barebox configuration unchanged"; \
	else \
		cp $(BAREBOX_DIR)/.config $(BAREBOX_CONFIG); \
	fi

# vim: syntax=make
