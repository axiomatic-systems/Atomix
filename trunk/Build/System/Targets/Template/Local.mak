#######################################################################
#
#    Common Target Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
all: libAtxSystem.a

#######################################################################
#    configuration variables
#######################################################################
ATX_TARGET = @TARGET@
ATX_ROOT   = ../../../../..

#######################################################################
#    target specific files
#######################################################################
ATX_SYSTEM_DEBUG_IMPLEMENTATION   = XXXXX
ATX_SYSTEM_SOCKETS_IMPLEMENTATION = XXXXX
ATX_SYSTEM_FILE_IMPLEMENTATION    = XXXXX

#######################################################################
#    includes
#######################################################################
include $(ATX_ROOT)/Build/Includes/TopLevel.mak
include ../../../Makefiles/Module.mak
include $(ATX_BUILD_INCLUDES)/BuildConfig.mak
include $(ATX_BUILD_INCLUDES)/AutoDep.mak
include $(ATX_BUILD_INCLUDES)/Rules.mak
