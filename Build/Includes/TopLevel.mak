#######################################################################
#
#    Top Level Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

######################################################################
#    checks 
#####################################################################
ifndef ATX_ROOT
$(error Variable ATX_ROOT is not set)
endif

ifndef ATX_TARGET
$(error Variable ATX_TARGET is not set)
endif

ifndef ATX_BUILD_CONFIG
$(error Variable ATX_BUILD_CONFIG is not set)
endif

######################################################################
#    includes
#####################################################################
include $(ATX_ROOT)/Build/Includes/Defs.mak
include $(ATX_BUILD_INCLUDES)/Exports.mak
include $(ATX_BUILD_TARGET_INCLUDES)/Tools.mak
-include $(ATX_BUILD_CONFIG_INCLUDES)/Config.mak
