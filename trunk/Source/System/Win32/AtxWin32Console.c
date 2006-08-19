/*****************************************************************
|
|   Atomix - Console Support: Win32 Implementation
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>

#include "AtxConfig.h"
#include "AtxConsole.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_ConsoleOutput
+---------------------------------------------------------------------*/
void
ATX_ConsoleOutput(const char* message)
{
    OutputDebugString(message);
    printf("%s", message);
}

