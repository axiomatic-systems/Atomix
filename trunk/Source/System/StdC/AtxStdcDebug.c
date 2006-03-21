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
|       constants
+---------------------------------------------------------------------*/
#define ATX_DEBUG_MAX_BUFFER 512

#if defined(ATX_DEBUG)
/*----------------------------------------------------------------------
|       ATX_Print
+---------------------------------------------------------------------*/
static void
ATX_Print(const char* message)
{
    printf("%s", message);
}
#endif

/*----------------------------------------------------------------------
|       ATX_Debug
+---------------------------------------------------------------------*/
void
ATX_Debug(const char* format, ...)
{
#if defined(ATX_DEBUG)
    char buffer[ATX_DEBUG_MAX_BUFFER];

    va_list args;

    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);
    buffer[ATX_DEBUG_MAX_BUFFER-1] = 0;
    ATX_Print(buffer);

    va_end(args);
#else
    ATX_COMPILER_UNUSED(format);
#endif
}
