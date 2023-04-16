# -*-makefile-*-
#
# Copyright (C) 2023 by Artur Wiebe <artur@4wiebe.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
HOST_PACKAGES-$(PTXCONF_HOST_PYTHON3_SETUPTOOLS_RUST) += host-python3-setuptools-rust

#
# Paths and names
#
HOST_PYTHON3_SETUPTOOLS_RUST_VERSION		:= 1.5.2
HOST_PYTHON3_SETUPTOOLS_RUST_MD5		:= fd3412ca77ffd6e06e2e83d0e9636084
HOST_PYTHON3_SETUPTOOLS_RUST			:= setuptools-rust-$(HOST_PYTHON3_SETUPTOOLS_RUST_VERSION)
HOST_PYTHON3_SETUPTOOLS_RUST_SUFFIX		:= tar.gz
HOST_PYTHON3_SETUPTOOLS_RUST_URL		:= $(call ptx/mirror-pypi, setuptools-rust, $(HOST_PYTHON3_SETUPTOOLS_RUST).$(HOST_PYTHON3_SETUPTOOLS_RUST_SUFFIX))
HOST_PYTHON3_SETUPTOOLS_RUST_SOURCE		:= $(SRCDIR)/$(HOST_PYTHON3_SETUPTOOLS_RUST).$(HOST_PYTHON3_SETUPTOOLS_RUST_SUFFIX)
HOST_PYTHON3_SETUPTOOLS_RUST_DIR		:= $(HOST_BUILDDIR)/$(HOST_PYTHON3_SETUPTOOLS_RUST)
HOST_PYTHON3_SETUPTOOLS_RUST_LICENSE		:= MIT
HOST_PYTHON3_SETUPTOOLS_RUST_LICENSE_FILES	:= file://LICENSE;md5=011cd92e702dd9e6b1a26157b6fd53f5

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

HOST_PYTHON3_SETUPTOOLS_RUST_CONF_TOOL	:= python3

# vim: syntax=make
