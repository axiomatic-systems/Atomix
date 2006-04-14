/*****************************************************************
|
|      File: AtxStdcDebug.c
|
|      Atomix - Debug Support: StdC Implementation
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>

#include "AtxConfig.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       ATX_Debug
+---------------------------------------------------------------------*/
void
ATX_Debug(const char* format, ...)
{
#if defined(ATX_DEBUG)
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
#else
    ATX_COMPILER_UNUSED(format);
#endif
}
