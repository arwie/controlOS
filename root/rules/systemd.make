# -*-makefile-*-
#
# Copyright (C) 2010 by Robert Schwebel <r.schwebel@pengutronix.de>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_SYSTEMD) += systemd

#
# Paths and names
#
SYSTEMD_VERSION	:= 233
SYSTEMD_MD5	:= 11d3ff48f3361b8bdcfcdc076a31b537
SYSTEMD		:= systemd-$(SYSTEMD_VERSION)
SYSTEMD_SUFFIX	:= tar.gz
SYSTEMD_URL	:= https://github.com/systemd/systemd/archive/v$(SYSTEMD_VERSION).$(SYSTEMD_SUFFIX)
SYSTEMD_SOURCE	:= $(SRCDIR)/$(SYSTEMD).$(SYSTEMD_SUFFIX)
SYSTEMD_DIR	:= $(BUILDDIR)/$(SYSTEMD)
SYSTEMD_LICENSE	:= GPL-2.0+, LGPL-2.1
SYSTEMD_LICENSE_FILES := \
	file://LICENSE.GPL2;md5=751419260aa954499f7abaabaa882bbe \
	file://LICENSE.LGPL2.1;md5=4fbd65380cdd255951079008b364516c

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

# x86: systemd segfaults at startup when built with PIE
# PPC: compiling fails when building with PIE
ifneq ($(PTXCONF_ARCH_X86)$(PTXCONF_ARCH_PPC),)
SYSTEMD_WRAPPER_BLACKLIST := TARGET_HARDEN_PIE
endif
SYSTEMD_CONF_ENV	:= \
	$(CROSS_ENV) \
	cc_cv_CFLAGS__Werror_shadow=no \
	ac_cv_path_KEXEC=/sbin/kexec \
	ac_cv_path_KILL=/bin/kill \
	ac_cv_path_KMOD=/bin/kmod \
	ac_cv_path_MOUNT_PATH=/bin/mount \
	ac_cv_path_UMOUNT_PATH=/bin/umount \
	ac_cv_path_SULOGIN=/sbin/sulogin \
	ac_cv_path_QUOTACHECK=/usr/sbin/quotacheck \
	ac_cv_path_QUOTAON=/usr/sbin/quotaon

ifdef PTXCONF_KERNEL_HEADER
SYSTEMD_CPPFLAGS	:= \
	-I$(KERNEL_HEADERS_INCLUDE_DIR)
endif

#
# autoconf
#
SYSTEMD_CONF_TOOL	:= autoconf
SYSTEMD_CONF_OPT	:= \
	$(CROSS_AUTOCONF_USR) \
	$(GLOBAL_LARGE_FILE_OPTION) \
	--disable-gcrypt \
	--enable-silent-rules \
	--disable-static \
	--disable-address-sanitizer \
	--disable-undefined-sanitizer \
	--disable-lto \
	--disable-dbus \
	--disable-utmp \
	--disable-coverage \
	--enable-kmod \
	--disable-xkbcommon \
	--enable-blkid \
	--$(call ptx/endis,PTXCONF_SYSTEMD_SECCOMP)-seccomp \
	--disable-ima \
	$(GLOBAL_SELINUX_OPTION) \
	--disable-apparmor \
	--enable-adm-group \
	--disable-wheel-group \
	--$(call ptx/endis,PTXCONF_SYSTEMD_XZ)-xz \
	--disable-zlib \
	--disable-bzip2 \
	--$(call ptx/endis,PTXCONF_SYSTEMD_LZ4)-lz4 \
	--disable-pam \
	--disable-acl \
	--disable-smack \
	--disable-audit \
	--$(call ptx/endis,PTXCONF_SYSTEMD_COREDUMP)-elfutils \
	--disable-libcryptsetup \
	--disable-qrencode \
	--disable-gnutls \
	--enable-microhttpd \
	--disable-libcurl \
	--disable-libidn \
	--$(call ptx/endis,PTXCONF_SYSTEMD_IPMASQUERADE)-libiptc \
	--disable-binfmt \
	--$(call ptx/endis,PTXCONF_SYSTEMD_VCONSOLE)-vconsole \
	--enable-quotacheck \
	--enable-tmpfiles \
	--disable-environment-d \
	--disable-sysusers \
	--disable-firstboot \
	--$(call ptx/disen,PTXCONF_SYSTEMD_DISABLE_RANDOM_SEED)-randomseed \
	--disable-backlight \
	--disable-rfkill \
	--$(call ptx/endis,PTXCONF_SYSTEMD_LOGIND)-logind \
	--disable-machined \
	--disable-importd \
	--enable-hostnamed \
	--$(call ptx/endis,PTXCONF_SYSTEMD_TIMEDATE)-timedated \
	--$(call ptx/endis,PTXCONF_SYSTEMD_TIMEDATE)-timesyncd \
	--$(call ptx/endis,PTXCONF_SYSTEMD_LOCALES)-localed \
	--$(call ptx/endis,PTXCONF_SYSTEMD_COREDUMP)-coredump \
	--disable-polkit \
	--$(call ptx/endis,PTXCONF_SYSTEMD_NETWORK)-resolved \
	--$(call ptx/endis,PTXCONF_SYSTEMD_NETWORK)-networkd \
	--enable-efi \
	--disable-gnuefi \
	--disable-tpm \
	--enable-myhostname \
	--$(call ptx/endis,PTXCONF_SYSTEMD_UDEV_HWDB)-hwdb \
	--disable-manpages \
	--disable-hibernate \
	--disable-ldconfig \
	--disable-split-usr \
	--disable-tests \
	--disable-debug \
	--without-python \
	--with-nobody-user=nobody \
	--with-nobody-group=nogroup \
	--with-fallback-hostname=$(call ptx/ifdef,PTXCONF_ROOTFS_ETC_HOSTNAME,$(PTXCONF_ROOTFS_ETC_HOSTNAME),ptxdist) \
	--with-default-hierarchy=hybrid \
	--with-ntp-servers= \
	--with-time-epoch=`date --date "$(PTXDIST_VERSION_YEAR)-$(PTXDIST_VERSION_MONTH)-01 UTC" +%s` \
	--with-system-uid-max=999 \
	--with-system-gid-max=999 \
	--with-dns-servers= \
	--with-sysvinit-path="" \
	--with-sysvrcnd-path="" \
	--with-tty-gid=112 \
	--with-dbuspolicydir=/usr/share/dbus-1/system.d \
	--with-dbussessionservicedir=/usr/share/dbus-1/services \
	--with-dbussystemservicedir=/usr/share/dbus-1/system-services

# needed for private libsystemd-shared
SYSTEMD_LDFLAGS	:= -Wl,-rpath,/usr/lib/systemd

# FIXME kernel from systemd README:
# - devtmpfs, cgroups are mandatory.
# - autofs4, ipv6  optional but strongly recommended

# FIXME busybox tools:
# - mount fails

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/systemd.install:
	@$(call targetinfo)
	@$(call world/install, SYSTEMD)
ifdef PTXCONF_SYSTEMD_UDEV_HWDB
	@$(PTXDIST_SYSROOT_HOST)/usr/bin/systemd-hwdb update --usr --root $(SYSTEMD_PKGDIR)
endif
ifndef PTXCONF_SYSTEMD_VCONSOLE
	@rm -v $(SYSTEMD_PKGDIR)/etc/systemd/system/getty.target.wants/getty@tty1.service
endif
#	# don't touch /etc and /home
	@rm -v $(SYSTEMD_PKGDIR)/usr/lib/tmpfiles.d/etc.conf
	@rm -v $(SYSTEMD_PKGDIR)/usr/lib/tmpfiles.d/home.conf
#	# the upstream default (graphical.target) wants display-manager.service
	@ln -sf multi-user.target $(SYSTEMD_PKGDIR)/usr/lib/systemd/system/default.target
#	# rpath is only needed for the executables
	@chrpath --delete $(SYSTEMD_PKGDIR)/usr/lib/lib*.so*
	@chrpath --delete $(SYSTEMD_PKGDIR)/usr/lib/systemd/libsystemd-shared-$(SYSTEMD_VERSION).so
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

SYSTEMD_HELPER := \
	systemd \
	systemd-ac-power \
	systemd-cgroups-agent \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_COREDUMP,systemd-coredump,) \
	systemd-fsck \
	systemd-hostnamed \
	systemd-initctl \
	systemd-journald \
	systemd-journal-remote \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_LOCALES,systemd-localed,) \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_LOGIND,systemd-logind,) \
	systemd-modules-load \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_NETWORK,systemd-networkd,) \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_NETWORK,systemd-networkd-wait-online,) \
	systemd-quotacheck \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_DISABLE_RANDOM_SEED,,systemd-random-seed) \
	systemd-remount-fs \
	systemd-reply-password \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_NETWORK,systemd-resolved,) \
	systemd-shutdown \
	systemd-sleep \
	systemd-socket-proxyd \
	systemd-sysctl \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_TIMEDATE,systemd-timedated,) \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_TIMEDATE,systemd-timesyncd,) \
	systemd-update-done \
	$(call ptx/ifdef, PTXCONF_SYSTEMD_VCONSOLE,systemd-vconsole-setup,)

SYSTEMD_UDEV_HELPER-y :=

SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_ATA)	+= ata_id
SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_CDROM)	+= cdrom_id
SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_SCSI)	+= scsi_id
SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_V4L)	+= v4l_id
SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_COLLECT)		+= collect
SYSTEMD_UDEV_HELPER-$(PTXCONF_SYSTEMD_UDEV_MTD_PROBE)		+= mtd_probe

SYSTEMD_UDEV_RULES-y := \
	50-udev-default.rules \
	60-persistent-alsa.rules \
	60-persistent-input.rules \
	60-persistent-storage-tape.rules \
	60-persistent-storage.rules \
	60-block.rules \
	60-drm.rules \
	60-sensor.rules \
	60-serial.rules \
	64-btrfs.rules \
	70-mouse.rules \
	75-net-description.rules \
	78-sound-card.rules \
	80-net-setup-link.rules

SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_LOGIND) += \
	70-power-switch.rules \
	70-uaccess.rules \
	71-seat.rules \
	73-seat-late.rules

SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_VCONSOLE) += \
	90-vconsole.rules

SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_CDROM)	+= 60-cdrom_id.rules
SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_UDEV_HWDB)			+= 60-evdev.rules
SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_UDEV_PERSISTENT_V4L)	+= 60-persistent-v4l.rules
SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_UDEV_MTD_PROBE)		+= 75-probe_mtd.rules
SYSTEMD_UDEV_RULES-$(PTXCONF_SYSTEMD_UDEV_DRIVERS_RULES)	+= 80-drivers.rules

$(STATEDIR)/systemd.targetinstall:
	@$(call targetinfo)

	@$(call install_init,  systemd)
	@$(call install_fixup, systemd,PRIORITY,optional)
	@$(call install_fixup, systemd,SECTION,base)
	@$(call install_fixup, systemd,AUTHOR,"Robert Schwebel <r.schwebel@pengutronix.de>")
	@$(call install_fixup, systemd,DESCRIPTION,missing)

#	#
#	# Some info about the current state of systemd support in ptxdist:
#	#
#	# - we don't care about a user systemd yet
#	#

	@$(call install_lib, systemd, 0, 0, 0644, libsystemd)
	@$(call install_lib, systemd, 0, 0, 0644, systemd/libsystemd-shared-$(SYSTEMD_VERSION))

	@$(call install_lib, systemd, 0, 0, 0644, libnss_myhostname)
	@$(call install_lib, systemd, 0, 0, 0644, libnss_systemd)
ifdef PTXCONF_SYSTEMD_NETWORK
	@$(call install_lib, systemd, 0, 0, 0644, libnss_resolve)
endif

#	# daemon + tools
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemctl)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/journalctl)
ifdef PTXCONF_SYSTEMD_LOGIND
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/loginctl)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-inhibit)
endif
ifdef PTXCONF_SYSTEMD_NETWORK
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/networkctl)
endif
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-escape)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-machine-id-setup)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-notify)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-tmpfiles)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/busctl)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/hostnamectl)
ifdef PTXCONF_SYSTEMD_LOCALES
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/localectl)
endif
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-analyze)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-cat)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-cgls)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-cgtop)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-delta)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-detect-virt)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-mount)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-path)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-run)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-socket-activate)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-stdio-bridge)
	@$(call install_link, systemd, systemd-mount, /usr/bin/systemd-umount)
ifdef PTXCONF_SYSTEMD_TIMEDATE
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/timedatectl)
endif

	@$(call install_tree, systemd, 0, 0, -, /usr/lib/systemd/system-generators/)
	@$(foreach helper, $(SYSTEMD_HELPER), \
		$(call install_copy, systemd, 0, 0, 755, -, \
			/usr/lib/systemd/$(helper));)


ifdef PTXCONF_INITMETHOD_SYSTEMD
	@$(call install_link, systemd, ../lib/systemd/systemd, /usr/sbin/init)
	@$(call install_link, systemd, ../bin/systemctl, /usr/sbin/halt)
	@$(call install_link, systemd, ../bin/systemctl, /usr/sbin/poweroff)
	@$(call install_link, systemd, ../bin/systemctl, /usr/sbin/reboot)
endif

#	# configuration
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/system.conf)
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/journald.conf)
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/journal-remote.conf)
ifdef PTXCONF_SYSTEMD_LOGIND
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/logind.conf)
endif
ifdef PTXCONF_SYSTEMD_TIMEDATE
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/timesyncd.conf)
endif
ifdef PTXCONF_SYSTEMD_NETWORK
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/systemd-resolve)
	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/lib/systemd/resolv.conf)
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/etc/systemd/resolved.conf)
endif
	@$(call install_tree, systemd, 0, 0, -, /etc/systemd/system/)
	@$(call install_tree, systemd, 0, 0, -, /usr/share/dbus-1/system.d/)

	@$(call install_tree, systemd, 0, 0, -, /usr/lib/tmpfiles.d/)
	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/lib/sysctl.d/50-default.conf)

ifdef PTXCONF_SYSTEMD_COREDUMP
	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/lib/sysctl.d/50-coredump.conf)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/coredumpctl)
	@$(call install_alternative, systemd, 0, 0, 0644, /etc/systemd/coredump.conf)
endif

	@$(call install_tree, systemd, 0, 0, -, /usr/share/dbus-1/services/)
	@$(call install_tree, systemd, 0, 0, -, /usr/share/dbus-1/system-services/)
ifdef PTXCONF_SYSTEMD_LOCALES
	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/share/systemd/kbd-model-map)
endif

	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/lib/udev/rules.d/99-systemd.rules)
ifdef PTXCONF_SYSTEMD_NETWORK
	@$(call install_tree, systemd, 0, 0, -, /usr/lib/systemd/network)
	@$(call install_alternative_tree, systemd, 0, 0, /usr/lib/systemd/network)
endif
	@$(call install_alternative, systemd, 0, 0, 0644, \
		/usr/lib/systemd/network/99-default.link)

#	# units
	@$(call install_tree, systemd, 0, 0, -, /usr/lib/systemd/system/)

ifndef PTXCONF_SYSTEMD_VCONSOLE
	@$(call install_alternative, systemd, 0, 0, 0644, /etc/vconsole.conf)
endif

	@$(call install_copy, systemd, 0, 0, 0755, /var/lib/systemd)

#	# systemd expects this directory to exist.
	@$(call install_copy, systemd, 0, 0, 0755, /var/lib/systemd/coredump)
	@$(call install_copy, systemd, 0, 0, 0700, /var/lib/machines)

	@$(call install_alternative, systemd, 0, 0, 0644, /etc/profile.d/systemd.sh)

	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/lib/systemd/systemd-udevd)
	@$(call install_copy, systemd, 0, 0, 0755, -, /usr/bin/udevadm)
	@$(call install_lib, systemd, 0, 0, 0644, libudev)

	@$(foreach helper, $(SYSTEMD_UDEV_HELPER-y), \
		$(call install_copy, systemd, 0, 0, 0755, -, \
			/usr/lib/udev/$(helper));)

	@$(foreach rule, $(SYSTEMD_UDEV_RULES-y), \
		$(call install_copy, systemd, 0, 0, 0644, -, \
			/usr/lib/udev/rules.d/$(rule));)

ifdef PTXCONF_SYSTEMD_UDEV_HWDB
	@$(call install_copy, systemd, 0, 0, 0644, -, /usr/lib/udev/hwdb.bin)
endif

ifdef PTXCONF_SYSTEMD_UDEV_CUST_RULES
	@$(call install_alternative_tree, systemd, 0, 0, /usr/lib/udev/rules.d)
endif

	@$(call install_finish, systemd)

	@$(call touch)

# vim: syntax=make
