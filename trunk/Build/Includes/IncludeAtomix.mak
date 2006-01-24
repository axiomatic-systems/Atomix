#######################################################################
#
#    Include file for Atomix Clients
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
ifndef XXX_CLIENT
$(error Variable XXX_CLIENT is not defined)
endif

ifndef ATOMIX_ROOT
$(error Variable ATOMIX_ROOT is not defined)
endif

##########################################################################
# client variables
##########################################################################
ifeq ($(ATX_ATOMIX_INCLUDED),)
ATX_ATOMIX_INCLUDED := yes

ATOMIX_INCLUDES = -I$(ATOMIX_ROOT)/Source/Atomix -I$(ATOMIX_ROOT)/Source/Core -I$(ATOMIX_ROOT)/Source/Config
$(XXX_CLIENT)_INCLUDES_C   += $(ATOMIX_INCLUDES)
$(XXX_CLIENT)_INCLUDES_CPP += $(ATOMIX_INCLUDES)

endif
