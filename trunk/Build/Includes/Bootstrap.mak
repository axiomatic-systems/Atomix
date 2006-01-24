#######################################################################
#
#    Bootstrap Makefile
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

ifndef ATX_BUILD_CONFIG
ATX_BUILD_CONFIG=Debug
endif
 
export ATX_ROOT
export ATX_TARGET
export ATX_BUILD_CONFIG
 
ifndef MAKECMDGOALS
MAKECMDGOALS = $(ATX_DEFAULT_GOAL)
endif
 
$(MAKECMDGOALS):
	@[ -d $(ATX_BUILD_CONFIG) ] || mkdir $(ATX_BUILD_CONFIG)
	@$(MAKE) --no-print-directory -C $(ATX_BUILD_CONFIG) -f $(ATX_ROOT)/Build/Includes/Common.mak  $(MAKECMDGOALS)

