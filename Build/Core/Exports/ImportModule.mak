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
ATX_MODULE_LIBRARIES += libAtxCore.a

##########################################################################
# module targets
##########################################################################
ATX_CORE_BUILD_DIR = $(ATX_BUILD_ROOT)/Core/Targets/$(ATX_TARGET)

.PHONY: Import-Core
Import-Core:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_CORE_BUILD_DIR)
	@$(ATX_COPY_IF_NEW) $(ATX_CORE_BUILD_DIR)/$(ATX_BUILD_CONFIG)/libAtxCore.a .
	$(ATX_MAKE_BANNER_END)

.PHONY: Clean-Core
Clean-Core:
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(ATX_CORE_BUILD_DIR) clean-deps
	$(ATX_MAKE_BANNER_END)
