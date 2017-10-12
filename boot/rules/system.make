# -*-makefile-*-
#
# Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#
# We provide this package
#
PACKAGES-$(PTXCONF_SYSTEM) += system

SYSTEM_VERSION	:= 0.1

# ----------------------------------------------------------------------------
# Get
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.get:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.extract:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.prepare:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.compile:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.install:
#	@$(call targetinfo)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/system.targetinstall:
	@$(call targetinfo)
	@$(call install_init, system)
	@$(call install_fixup,system,PRIORITY,optional)
	@$(call install_fixup,system,SECTION,base)
	@$(call install_fixup,system,AUTHOR,"Artur Wiebe")
	@$(call install_fixup,system,DESCRIPTION,missing)

	@$(call install_copy,        system, 0, 0, 0755, /mnt/install)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/boot)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/init)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/root)
	@$(call install_copy,        system, 0, 0, 0755, /mnt/data)

	@$(call install_alternative, system, 0, 0, 0644, /etc/install.conf)
	
	@$(call install_finish,system)
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/system.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, SYSTEM)

# vim: syntax=make
