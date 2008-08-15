/*****************************************************************
|
|   Atomix - System, PS3 Implementation
|
|   (c) 2001-2006 Gilles Boccon-Gibod (bok@bok.net)
|   Author: Gilles Boccon-Gibod
|
****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/timer.h>
#include <sys/sys_time.h>
#include <unistd.h>

#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxSystem.h"
#include "AtxResults.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|   singleton
+---------------------------------------------------------------------*/
static ATX_Boolean ATX_System_RandomGeneratorSeeded = ATX_FALSE;

/*----------------------------------------------------------------------
|   ATX_System_GetCurrentTimeStamp
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now)
{
    sys_time_sec_t sec;
    sys_time_nsec_t nsec;

    int result = sys_time_get_current_time(&sec, &nsec);
    if (result != CELL_OK){
        now->seconds     = 0;
        now->nanoseconds = 0;   
        return ATX_FAILURE;
    }

    /* convert format */
    now->seconds     = sec;
    now->nanoseconds = nsec;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_Sleep
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_Sleep(const ATX_TimeInterval* duration)
{
    unsigned long usecs = 1000000*duration->seconds + duration->nanoseconds/1000;
    sys_timer_usleep(usecs);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_System_SleepUntil
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
|   ATX_System_SetRandomSeed
+---------------------------------------------------------------------*/
ATX_Result  
ATX_System_SetRandomSeed(unsigned int seed)
{
    srand(seed);
    ATX_System_RandomGeneratorSeeded = ATX_TRUE;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System::ATX_System
+---------------------------------------------------------------------*/
ATX_UInt32 
ATX_System_GetRandomInteger()
{
    if (!ATX_System_RandomGeneratorSeeded) {
        ATX_TimeStamp now;
        ATX_System_GetCurrentTimeStamp(&now);
        ATX_System_SetRandomSeed(now.nanoseconds);
    }

    return rand();
}


