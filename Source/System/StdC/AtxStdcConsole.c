/*****************************************************************
|
|      Atomix - Console Support: StdC Implementation
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
#include "AtxConsole.h"

/*----------------------------------------------------------------------
|       ATX_ConsoleOutput
+---------------------------------------------------------------------*/
void
ATX_ConsoleOutput(const char* message)
{
    printf("%s", message);
}
