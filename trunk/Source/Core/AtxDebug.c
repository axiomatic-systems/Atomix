/*****************************************************************
|
|   Atomix - Debug Support
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxDebug.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_DebugOutputFunction
+---------------------------------------------------------------------*/
static void
ATX_DebugOutputFunction(void* parameter, const char* message)
{
    ATX_COMPILER_UNUSED(parameter);
    ATX_DebugOutput(message);
}

/*----------------------------------------------------------------------
|   ATX_Debug
+---------------------------------------------------------------------*/
void
ATX_Debug(const char* format, ...)
{
#if defined(ATX_DEBUG)
    va_list args;
    va_start(args, format);
    ATX_FormatOutput(ATX_DebugOutputFunction, NULL, format, args);
    va_end(args);
#else
    ATX_COMPILER_UNUSED(format);
#endif
}
