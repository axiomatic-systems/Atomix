/*****************************************************************
|
|      File: AtxCocoaConsole.c
|
|      Atomix - System Log Console: Cocoa Implementation
|
|      (c) 2002-2015 Gilles Boccon-Gibod
|      Author: Edin Hodzic (dino@concisoft.com)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#include <Foundation/Foundation.h>

#include "AtxConsole.h"

/*----------------------------------------------------------------------
|       ATX_ConsoleOutput
+---------------------------------------------------------------------*/
void
ATX_ConsoleOutput(const char* message)
{
    printf("%s", message);
    NSLog(@"%s", message);
}
