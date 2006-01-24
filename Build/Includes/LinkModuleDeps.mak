#######################################################################
#
#    Module Dependencies Processing
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
ifneq ($(ATX_LINK_MODULES),)
include $(foreach module,$(ATX_LINK_MODULES),$(ATX_BUILD_MODULES)/Link$(module).mak)
ATX_LINK_MODULES :=
endif

