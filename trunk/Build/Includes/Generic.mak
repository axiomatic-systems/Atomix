#######################################################################
#
#    Generic Makefiles Rules
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

#######################################################################
#  check configuration variables
#######################################################################
ifndef ATX_TARGET
ifdef MAKECMDGOALS
$(error "ATX_TARGET variable is not set")
endif
endif

ifndef ATX_BUILD_CONFIG
# default build configuration
ATX_BUILD_CONFIG=Debug
endif

#######################################################################
#  target templates
#######################################################################
ATX_ALL_BUILDS = $(ATX_SUBDIR_BUILDS) $(ATX_SUBTARGET_BUILDS)
All: $(ATX_ALL_BUILDS)
Clean-All: $(foreach goal,$(ATX_ALL_BUILDS),Clean-$(goal))

ATX_SUBDIR_CLEANS = $(foreach dir,$(ATX_SUBDIR_BUILDS),Clean-$(dir))
ATX_SUBTARGET_CLEANS = $(foreach dir,$(ATX_SUBTARGET_BUILDS),Clean-$(dir))

.PHONY: $(ATX_SUBDIR_BUILDS)
$(ATX_SUBDIR_BUILDS): 
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $@ All
	$(ATX_MAKE_BANNER_END)

.PHONY: $(ATX_SUBTARGET_BUILDS)
$(ATX_SUBTARGET_BUILDS): 
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $@/Targets/$(ATX_TARGET)
	$(ATX_MAKE_BANNER_END)

.PHONY: $(ATX_SUBDIR_CLEANS)
$(ATX_SUBDIR_CLEANS): 
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(subst Clean-,,$@) Clean-All
	$(ATX_MAKE_BANNER_END)

.PHONY: $(ATX_SUBTARGET_CLEANS)
$(ATX_SUBTARGET_CLEANS): 
	$(ATX_MAKE_BANNER_START)
	$(ATX_SUB_MAKE) -C $(subst Clean-,,$@)/Targets/$(ATX_TARGET) clean-deps
	$(ATX_MAKE_BANNER_END)
