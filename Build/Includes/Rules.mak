#######################################################################
#
#    Common Makefile Rules
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
#    pattern rules
##########################################################################

### build .o from .c
%.o: %.c
	$(ATX_COMPILE_C) $(ATX_FLAGS_C) $(ATX_DEFINES_C) $(ATX_INCLUDES_C) $(ATX_OPTIMIZE_C) $(ATX_DEBUG_C) $(ATX_PROFILE_C) $(ATX_WARNINGS_C) $(ATX_LOCAL_FLAGS) -c $< -o $@

### build .o from .cpp
%.o: %.cpp
	$(ATX_COMPILE_CPP) $(ATX_FLAGS_CPP) $(ATX_DEFINES_CPP) $(ATX_INCLUDES_CPP) $(ATX_OPTIMIZE_CPP) $(ATX_DEBUG_CPP) $(ATX_PROFILE_CPP) $(ATX_WARNINGS_CPP) $(ATX_LOCAL_FLAGS) -c $< -o $@

### build .a from .o 
%.a:
	$(ATX_ARCHIVE_O) $@ $(filter %.o %.a %.lib,$^)

### make an executable
ATX_MAKE_EXECUTABLE_COMMAND_C = $(ATX_LINK_C) $(ATX_OPTIMIZE_C) $(ATX_DEBUG_C) $(ATX_PROFILE_C) $(filter %.o %.a %.lib,$^) -o $@ $(ATX_LIBRARIES_C)

### make an archive
ATX_MAKE_ARCHIVE_COMMAND = $(ATX_ARCHIVE_A) $@  $(filter %.o %.a %.lib,$^)

### clean
.PHONY: clean
clean:
	@rm -f $(ATX_LOCAL_FILES_TO_CLEAN)

### clean-deps
.PHONY: clean-deps
clean-deps: $(ATX_IMPORTED_MODULE_CLEANS) clean

##########################################################################
#    utils
##########################################################################
ATX_COLOR_SET_1   = "[33;1m"
ATX_COLOR_SET_2   = "[36;1m"
ATX_COLOR_RESET = "[0m"

ATX_MAKE_BANNER_START = @echo $(ATX_COLOR_SET_1)================ making \ $(ATX_COLOR_RESET) $(ATX_COLOR_SET_2) $(XXX_CLIENT)::$@ [$(ATX_BUILD_CONFIG)] $(ATX_COLOR_RESET) $(ATX_COLOR_SET_1) =============== $(ATX_COLOR_RESET)

ATX_MAKE_BANNER_END =  @echo $(ATX_COLOR_SET_1)================ done with $(ATX_COLOR_RESET)$(ATX_COLOR_SET_2)$(XXX_CLIENT)::$@ $(ATX_COLOR_RESET) $(ATX_COLOR_SET_1) ================= $(ATX_COLOR_RESET)

ATX_MAKE_BANNER = $(ATX_MAKE_BANNER_START)

ATX_SUB_MAKE = @$(MAKE) $(ATX_MAKE_FLAGS) 
