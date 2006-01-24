#######################################################################
#
#    Module Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
# module dependencies
#######################################################################
ATX_IMPORT_MODULES = Atomix
include $(ATX_BUILD_INCLUDES)/ImportModuleDeps.mak

#######################################################################
# sources and object files
#######################################################################
ATX_MODULE_SOURCES = PropertiesTest.c
ATX_MODULE_OBJECTS = $(ATX_MODULE_SOURCES:.c=.o)

#######################################################################
# paths
#######################################################################
VPATH += $(ATX_SOURCE_ROOT)/Tests/Properties

#######################################################################
# clean
#######################################################################
ATX_LOCAL_FILES_TO_CLEAN = *.d *.o *.a PropertiesTest

#######################################################################
# targets
#######################################################################
PropertiesTest: $(ATX_IMPORTED_MODULE_LIBS) $(ATX_MODULE_OBJECTS) $(ATX_MODULE_LIBRARIES)
	$(ATX_MAKE_BANNER_START)
	$(ATX_MAKE_EXECUTABLE_COMMAND_C)
	$(ATX_MAKE_BANNER_END)
