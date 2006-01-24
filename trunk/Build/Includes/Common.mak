#######################################################################
#
#    Common Target Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
#    includes
#######################################################################
include $(ATX_ROOT)/Build/Includes/TopLevel.mak
include $(ATX_BUILD_INCLUDES)/BuildConfig.mak
-include ../Local.mak
include ../../../Makefiles/Module.mak
include $(ATX_BUILD_INCLUDES)/Rules.mak
include $(ATX_BUILD_INCLUDES)/AutoDep.mak
