#######################################################################
#
#    Build Definitions for Automatic Dependencies
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
# rules
##########################################################################

### autodependency for .cpp files
ifneq ($(ATX_AUTODEP_STDOUT),)
%.d: %.cpp
	$(ATX_AUTODEP_CPP) $(ATX_DEFINES_CPP) $(ATX_INCLUDES_CPP) $< > $@
else
%.d: %.cpp
	$(ATX_AUTODEP_CPP) $(ATX_DEFINES_CPP) $(ATX_INCLUDES_CPP) $< -o $@
endif

### autodependency for .c files
ifneq ($(ATX_AUTODEP_STDOUT),)
%.d: %.c
	$(ATX_AUTODEP_C) $(ATX_DEFINES_C) $(ATX_INCLUDES_C) $< > $@
else
%.d: %.c
	$(ATX_AUTODEP_C) $(ATX_DEFINES_C) $(ATX_INCLUDES_C) $< -o $@
endif

##########################################################################
# auto dependencies
##########################################################################
ATX_MODULE_DEPENDENCIES := $(patsubst %.c,%.d,$(ATX_MODULE_SOURCES))
ATX_MODULE_DEPENDENCIES := $(patsubst %.cpp,%.d,$(ATX_MODULE_DEPENDENCIES))

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), clean-deps)
ifdef ATX_MODULE_DEPENDENCIES
include $(ATX_MODULE_DEPENDENCIES)
endif
ifdef ATX_MODULE_LOCAL_RULES
include $(ATX_MODULE_LOCAL_RULES)
endif
endif
endif
