#######################################################################
#
#    Core Module Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
# sources and object files
#######################################################################
ATX_MODULE_SOURCES = AtxInterfaces.c AtxList.c AtxStreams.c AtxUtils.c \
	AtxProperties.c AtxRingBuffer.c AtxSockets.c AtxDataBuffer.c
ATX_MODULE_OBJECTS = $(ATX_MODULE_SOURCES:.c=.o)
ATX_INCLUDES_C    += -I$(ATX_SOURCE_ROOT)/Core -I$(ATX_SOURCE_ROOT)/Config

#######################################################################
# paths
#######################################################################
VPATH += $(ATX_SOURCE_ROOT)/Core

#######################################################################
# clean
#######################################################################
ATX_LOCAL_FILES_TO_CLEAN = *.d *.o *.a

#######################################################################
# targets
#######################################################################
libAtxCore.a: $(ATX_MODULE_OBJECTS) $(ATX_MODULE_LIBRARIES)

