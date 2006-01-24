#######################################################################
#
#    System Module Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
# module dependencies
#######################################################################
ATX_INCLUDE_MODULES = Core
include $(ATX_BUILD_INCLUDES)/IncludeModuleDeps.mak

#######################################################################
# sources and object files
#######################################################################
ATX_MODULE_SOURCES = \
	Atx$(ATX_SYSTEM_DEBUG_IMPLEMENTATION).c   \
	Atx$(ATX_SYSTEM_SOCKETS_IMPLEMENTATION).c \
	Atx$(ATX_SYSTEM_FILE_IMPLEMENTATION).c

ATX_MODULE_OBJECTS = $(ATX_MODULE_SOURCES:.c=.o)

#######################################################################
# paths
#######################################################################
VPATH += $(ATX_SOURCE_ROOT)/System/Bsd:$(ATX_SOURCE_ROOT)/System/StdC:$(ATX_SOURCE_ROOT)/System/Posix:$(ATX_SOURCE_ROOT)/System/Win32

#######################################################################
# clean
#######################################################################
ATX_LOCAL_FILES_TO_CLEAN = *.d *.o *.a

#######################################################################
# targets
#######################################################################
libAtxSystem.a: $(ATX_MODULE_OBJECTS)
