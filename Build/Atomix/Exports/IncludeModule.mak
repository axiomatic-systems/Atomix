#######################################################################
#
#    Module Exports
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

ifeq ($(ATX_ATOMIX_MODULE_INCLUDED),)
ATX_ATOMIX_MODULE_INCLUDED=yes

##########################################################################
# include dependencies
##########################################################################
ATX_INCLUDE_MODULES := Core System
include $(ATX_BUILD_INCLUDES)/IncludeModuleDeps.mak

##########################################################################
# exported variables
##########################################################################
ATX_ATOMIX_INCLUDES = -I$(ATX_SOURCE_ROOT)/Atomix
ATX_INCLUDES_C   += $(ATX_ATOMIX_INCLUDES)
ATX_INCLUDES_CPP += $(ATX_ATOMIX_INCLUDES)

endif
