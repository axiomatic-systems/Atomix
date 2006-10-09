#######################################################################
#
#    Makefile Variables for the arm-unknown-linux targets
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
#    tools
##########################################################################
ATX_CROSS_PREFIX=arm-linux-
ATX_COMPILE_C   = $(ATX_CROSS_PREFIX)gcc
ATX_FLAGS_C     = -ansi -pedantic
ATX_DEFINES_C   = -D_REENTRANT -D_BSD_SOURCE -D_POSIX_SOURCE
ATX_WARNINGS_C  = -Werror -Wall -W -Wundef -Wmissing-prototypes -Wmissing-declarations -Wno-long-long
ATX_AUTODEP_C   = $(ATX_CROSS_PREFIX)gcc -MM
ATX_LINK_C      = $(ATX_CROSS_PREFIX)gcc

ATX_ARCHIVE_O   = $(ATX_CROSS_PREFIX)ld -r -o
ATX_ARCHIVE_A   = $(ATX_CROSS_PREFIX)ar rs

ATX_COPY_IF_NEW = cp -u
ATX_MAKE_FLAGS  = --no-print-directory

### dmalloc support
ifneq ($(DMALLOC_OPTIONS),)
ATX_DEFINES_C   += -DDMALLOC
ATX_LIBRARIES_C += -ldmalloc
endif
