/*****************************************************************
|
|   Atomix - Debug Support: Win32 Implementation
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

/*----------------------------------------------------------------------
|   ATX_DebugOutput
+---------------------------------------------------------------------*/
void
ATX_DebugOutput(const char* message)
{
    ATX_ConsoleOutput(message);
}