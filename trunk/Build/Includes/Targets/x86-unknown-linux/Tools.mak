#######################################################################
#
#    Makefile Variables for the x86-unknown-linux targets
#
#    (c) 2002-2003 Gilles Boccon-Gibod
#    Author: Gilles Boccon-Gibod (bok@bok.net)
#
#######################################################################

##########################################################################
#    tools
##########################################################################
ATX_COMPILE_C   = gcc
ATX_FLAGS_C     = -pedantic
ATX_DEFINES_C   = -D_REENTRANT -D_BSD_SOURCE -D_POSIX_SOURCE
ATX_WARNINGS_C  = -Werror -Wall -W -Wundef -Wmissing-prototypes -Wmissing-declarations -Wno-long-long
ATX_OPTIMIZE_C  = -O3
ATX_AUTODEP_C   = gcc -MM
ATX_LINK_C      = gcc

ATX_ARCHIVE_O   = ld -r -o
ATX_ARCHIVE_A   = ar rs

ATX_COPY_IF_NEW = cp -u
ATX_MAKE_FLAGS  = --no-print-directory

ATX_LIBRARIES_C += -lm

### dmalloc support
ifneq ($(DMALLOC_OPTIONS),)
ATX_DEFINES_C   += -DDMALLOC
ATX_LIBRARIES_C += -ldmalloc
endif
