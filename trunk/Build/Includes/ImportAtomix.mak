#######################################################################
#
#    Include file for Atomix Clients
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
ifndef XXX_CLIENT
$(error Variable XXX_CLIENT is not defined)
endif

ifndef ATOMIX_ROOT
$(error Variable ATOMIX_ROOT is not defined)
endif

ifeq ($(ATX_ATOMIX_IMPORTED),)
ATX_ATOMIX_IMPORTED := yes

##########################################################################
# client variables
##########################################################################
$(XXX_CLIENT)_MODULE_LIBRARIES += libAtomix.a

##########################################################################
# dependencies
##########################################################################
include $(ATOMIX_ROOT)/Build/Includes/IncludeAtomix.mak

##########################################################################
# module variables
##########################################################################
ATX_BUILD_DIR = $(ATOMIX_ROOT)/Build/Atomix/Targets/$($(XXX_CLIENT)_TARGET)

##########################################################################
# targets
##########################################################################
.PHONY: Import-Atomix
Import-Atomix:
	$($(XXX_CLIENT)_MAKE_BANNER_START)
	$($(XXX_CLIENT)_SUB_MAKE) -C $(ATX_BUILD_DIR) ATX_BUILD_CONFIG=$($(XXX_CLIENT)_BUILD_CONFIG)
	@$($(XXX_CLIENT)_COPY_IF_NEW) $(ATX_BUILD_DIR)/$($(XXX_CLIENT)_BUILD_CONFIG)/libAtomix.a .
	$($(XXX_CLIENT)_MAKE_BANNER_END)

.PHONY: Clean-Atomix
Clean-Atomix:
	$($(XXX_CLIENT)_MAKE_BANNER_START)
	$($(XXX_CLIENT)_SUB_MAKE) -C $(ATX_BUILD_DIR) ATX_BUILD_CONFIG=$($(XXX_CLIENT)_BUILD_CONFIG) clean-deps 
	$($(XXX_CLIENT)_MAKE_BANNER_END)

endif
