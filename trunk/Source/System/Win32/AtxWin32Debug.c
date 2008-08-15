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
#include <windows.h>
#include <stdio.h>

#include "AtxConfig.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_DebugOutput
+---------------------------------------------------------------------*/
void
ATX_DebugOutput(const char* message)
{
    OutputDebugString(message);
    printf("%s", message);
}
