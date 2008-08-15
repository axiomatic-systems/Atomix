/*****************************************************************
|
|   Atomix - Console
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxConsole.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_ConsoleOutputFunction
+---------------------------------------------------------------------*/
static void
ATX_ConsoleOutputFunction(void* parameter, const char* message)
{
    ATX_COMPILER_UNUSED(parameter);
    ATX_ConsoleOutput(message);
}

/*----------------------------------------------------------------------
|   ATX_ConsoleOutputF
+---------------------------------------------------------------------*/
void 
ATX_ConsoleOutputF(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    ATX_FormatOutput(ATX_ConsoleOutputFunction, NULL, format, args);

    va_end(args);
}

