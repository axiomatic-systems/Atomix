/*****************************************************************
|
|   Atomix - System, Win32 Implementation
|
|   (c) 2001-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <windows.h>
#include <sys/timeb.h>

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
    struct _timeb time_stamp;

#if (_MSC_VER >= 1400)
    _ftime_s(&time_stamp);
#else
    _ftime(&time_stamp);
#endif
    now->seconds     = (long)time_stamp.time;
    now->nanoseconds = (long)time_stamp.millitm*1000000;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_Sleep
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_Sleep(const ATX_TimeInterval* duration)
{
    DWORD milliseconds = 1000*duration->seconds + duration->nanoseconds/1000000;
    Sleep(milliseconds);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_SleepUntil
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

