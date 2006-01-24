#######################################################################
#
#    Module Dependencies Processing
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
ifneq ($(ATX_INCLUDE_MODULES),)
include $(foreach module,$(ATX_INCLUDE_MODULES),$(ATX_BUILD_MODULES)/Include$(module).mak)
ATX_INCLUDE_MODULES :=
endif

