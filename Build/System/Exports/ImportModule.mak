#######################################################################
#
#    Exports
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
# exported variables
##########################################################################
ATX_MODULE_LIBRARIES += libAtxSystem.a

##########################################################################
# module targets
##########################################################################
ATX_SYSTEM_BUILD_DIR = $(ATX_BUILD_ROOT)/System/Targets/$(ATX_TARGET)

.PHONY: Import-System
Import-System:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_SYSTEM_BUILD_DIR)
	@$(ATX_COPY_IF_NEW) $(ATX_SYSTEM_BUILD_DIR)/$(ATX_BUILD_CONFIG)/libAtxSystem.a .
	$(ATX_MAKE_BANNER_END)

.PHONY: Clean-System
Clean-System:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_SYSTEM_BUILD_DIR) clean-deps
	$(ATX_MAKE_BANNER_END)
