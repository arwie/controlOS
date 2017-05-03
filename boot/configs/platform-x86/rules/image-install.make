#
# We provide this package
#
IMAGE_PACKAGES-$(PTXCONF_IMAGE_INSTALL) += image-install

#
# Paths and names
#
IMAGE_INSTALL			:= image-install
IMAGE_INSTALL_DIR		:= $(BUILDDIR)/$(IMAGE_INSTALL)
IMAGE_INSTALL_IMAGE		:= $(IMAGEDIR)/install.img
IMAGE_INSTALL_CONFIG	:= image-install.config

# ----------------------------------------------------------------------------
# Image
# ----------------------------------------------------------------------------

$(IMAGE_INSTALL_IMAGE):
	@$(call targetinfo)

	@$(call image/genimage, IMAGE_INSTALL)

	rm -f $(IMAGEDIR)/install.vfat

	xz -T0 -zf $(IMAGE_INSTALL_IMAGE)

	@$(call finish)

# vim: syntax=make
