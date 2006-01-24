#######################################################################
#
#    Exports for Linking Clients
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
# includes
##########################################################################
include $(ATX_BUILD_ROOT)/Atomix/Exports/IncludeModule.mak

##########################################################################
# libraries
##########################################################################
ATX_MODULE_LIBRARIES += libAtomix.a

##########################################################################
# module variables
##########################################################################
ATX_BUILD_DIR = $(ATX_ROOT)/Build/Atomix/Targets/$(ATX_TARGET)

##########################################################################
# targets
##########################################################################
.PHONY: Import-Atomix
Import-Atomix:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_BUILD_DIR)
	@$(ATX_COPY_IF_NEW) $(ATX_BUILD_DIR)/$(ATX_BUILD_CONFIG)/libAtomix.a .
	$(ATX_MAKE_BANNER_END)

.PHONY: Clean-Atomix
Clean-Atomix:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_BUILD_DIR) clean-deps
	$(ATX_MAKE_BANNER_END)


