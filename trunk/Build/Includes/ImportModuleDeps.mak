#######################################################################
#
#    Module Dependencies Processing
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################
ifneq ($(ATX_IMPORT_MODULES),)
ATX_IMPORTED_MODULE_LIBS  := $(foreach module,$(ATX_IMPORT_MODULES),Import-$(module))
ATX_IMPORTED_MODULE_CLEANS := $(foreach module,$(ATX_IMPORT_MODULES),Clean-$(module))
include $(foreach module,$(ATX_IMPORT_MODULES),$(ATX_BUILD_MODULES)/Import$(module).mak)
ATX_IMPORT_MODULES :=
endif
