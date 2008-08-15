/*****************************************************************
|
|   Atomix - Console
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/
/** @file
* Header file for console support
*/

#ifndef _ATX_CONSOLE_H_
#define _ATX_CONSOLE_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxResults.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void ATX_ConsoleOutput(const char* message);
extern void ATX_ConsoleOutputF(const char* format, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_CONSOLE_H_ */
