#
# We provide this package
#
IMAGE_PACKAGES-$(PTXCONF_IMAGE_SYSTEM) += image-system

#
# Paths and names
#
IMAGE_SYSTEM			:= image-system
IMAGE_SYSTEM_DIR		:= $(BUILDDIR)/$(IMAGE_SYSTEM)
IMAGE_SYSTEM_IMAGE		:= $(IMAGEDIR)/system.img
IMAGE_SYSTEM_CONFIG		:= image-system.config

# ----------------------------------------------------------------------------
# Image
# ----------------------------------------------------------------------------

$(IMAGE_SYSTEM_IMAGE):
	@$(call targetinfo)
	
	@$(call image/genimage, IMAGE_SYSTEM)
	
	rm -f $(IMAGEDIR)/boot.vfat
	rm -f $(IMAGEDIR)/init.vfat
	
	xz -T0 -zf $(IMAGE_SYSTEM_IMAGE)
	
	@$(call finish)

# vim: syntax=make
