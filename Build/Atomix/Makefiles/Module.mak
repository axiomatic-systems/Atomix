#######################################################################
#
#    Atomix Module Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
# module dependencies
#######################################################################
ATX_IMPORT_MODULES = Core System
include $(ATX_BUILD_INCLUDES)/ImportModuleDeps.mak

#######################################################################
# clean
#######################################################################
ATX_LOCAL_FILES_TO_CLEAN = *.d *.o *.a

#######################################################################
# targets
#######################################################################
ATX_SDK_DIR         = SDK
ATX_SDK_LIB_DIR     = SDK/lib
ATX_SDK_INCLUDE_DIR = SDK/include

.PHONY: dirs
dirs:
	-@mkdir -p $(ATX_SDK_DIR)
	-@mkdir -p $(ATX_SDK_LIB_DIR)
	-@mkdir -p $(ATX_SDK_INCLUDE_DIR)

libAtomix.a: $(ATX_IMPORTED_MODULE_LIBS) $(ATX_MODULE_OBJECTS) $(ATX_MODULE_LIBRARIES)
	$(ATX_MAKE_BANNER_START)
	$(ATX_MAKE_ARCHIVE_COMMAND)
	$(ATX_MAKE_BANNER_END)

.PHONY: SDK
SDK: dirs libAtomix.a
	$(ATX_MAKE_BANNER_START)
	@cp libAtomix.a $(ATX_SDK_LIB_DIR)
	@cp $(ATX_SOURCE_ROOT)/Core/*.h $(ATX_SDK_INCLUDE_DIR)
	@cp $(ATX_SOURCE_ROOT)/Config/*.h $(ATX_SDK_INCLUDE_DIR)
	@cp $(ATX_SOURCE_ROOT)/Atomix/Atomix.h $(ATX_SDK_INCLUDE_DIR)
	$(ATX_MAKE_BANNER_END)
