/*****************************************************************
|
|      Atomix - Debug Support: StdC Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdio.h>

#include "AtxConfig.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       ATX_DebugOutput
+---------------------------------------------------------------------*/
void
ATX_DebugOutput(const char* message)
{
    printf("%s", message);
}
