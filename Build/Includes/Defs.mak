#######################################################################
#
#    Build Definitions
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
# variables
#######################################################################
XXX_CLIENT                = ATX
ATX_BUILD_ROOT            = $(ATX_ROOT)/Build
ATX_BUILD_INCLUDES        = $(ATX_BUILD_ROOT)/Includes
ATX_BUILD_MODULES         = $(ATX_BUILD_INCLUDES)/Modules
ATX_BUILD_TARGET_INCLUDES = $(ATX_BUILD_INCLUDES)/Targets/$(ATX_TARGET)
ATX_BUILD_CONFIG_INCLUDES = $(ATX_BUILD_TARGET_INCLUDES)/BuildConfigs/$(ATX_BUILD_CONFIG)
ATX_SOURCE_ROOT    = $(ATX_ROOT)/Source
ifndef ATX_BUILD_CONFIG
ATX_BUILD_CONFIG = Debug
endif
