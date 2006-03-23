/*****************************************************************
|
|      Atomix - System, Posix Implementation
|
|      (c) 2001-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxSystem.h"
#include "AtxResults.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       ATX_System_GetCurrentTimeStamp
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now)
{
    struct timeval now_tv;

    /* get current time from system */
    if (gettimeofday(&now_tv, NULL)) {
        now->seconds     = 0;
        now->nanoseconds = 0;
        return ATX_FAILURE;
    }

    /* convert format */
    now->seconds     = now_tv.tv_sec;
    now->nanoseconds = now_tv.tv_usec * 1000;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_System_Sleep
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_Sleep(const ATX_TimeInterval* duration)
{
    unsigned long usecs = 1000000*duration->seconds + duration->nanoseconds/1000;
    usleep(usecs);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_System_SleepUntil
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_SleepUntil(const ATX_TimeStamp* when)
{
    ATX_TimeStamp now;
    ATX_System_GetCurrentTimeStamp(&now);
    if (ATX_TimeStamp_IsLater(*when, now)) {
        ATX_TimeInterval duration;
        ATX_TimeStamp_Sub(duration, *when, now);
        return ATX_System_Sleep(&duration);
    } else {
        return ATX_SUCCESS;
    }
}

/*----------------------------------------------------------------------
|       ATX_System_SetRandomSeed
+---------------------------------------------------------------------*/
ATX_Result  
ATX_System_SetRandomSeed(unsigned int seed)
{
    srand(seed);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_System_GetRandomInteger
+---------------------------------------------------------------------*/
ATX_Int32 
ATX_System_GetRandomInteger(void)
{
    static ATX_Boolean seeded = ATX_FALSE;
    if (!seeded) {
        ATX_TimeStamp now;
        ATX_System_GetCurrentTimeStamp(&now);
        srand(now.nanoseconds);
        seeded = ATX_TRUE;
    }

    return rand();
}

