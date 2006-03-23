/*****************************************************************
|
|   Atomix - System support
|
|   (c) 2001-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_SYSTEM_H_
#define _ATX_SYSTEM_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxTime.h"

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern ATX_Result ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now);
extern ATX_Result ATX_System_Sleep(const ATX_TimeInterval* duration);
extern ATX_Result ATX_System_SleepUntil(const ATX_TimeStamp* when);
extern ATX_Result ATX_System_SetRandomSeed(unsigned int seed);
extern ATX_Int32  ATX_System_GetRandomInteger(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_SYSTEM_H_ */

