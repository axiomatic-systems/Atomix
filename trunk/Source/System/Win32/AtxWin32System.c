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
#if !defined(_WIN32_WCE)
#include <sys/timeb.h>
#endif

#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxSystem.h"
#include "AtxResults.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|   singleton
+---------------------------------------------------------------------*/
static ATX_Boolean ATX_System_RandomGeneratorSeeded = ATX_FALSE;

#if defined(_WIN32_WCE)
/*----------------------------------------------------------------------
|   ATX_System_GetCurrentTimeStamp
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now)
{
    SYSTEMTIME stime;
    FILETIME   ftime;
    __int64    time64;
    GetSystemTime(&stime);
    SystemTimeToFileTime(&stime, &ftime);

    /* convert to 64-bits 100-nanoseconds value */
    time64 = (((unsigned __int64)ftime.dwHighDateTime)<<32) | ((unsigned __int64)ftime.dwLowDateTime);
    time64 -= 116444736000000000; /* convert from the Windows epoch (Jan. 1, 1601) to the 
                                   * Unix epoch (Jan. 1, 1970) */
    
    now->seconds = (ATX_Int32)(time64/10000000);
    now->nanoseconds = 100*(ATX_Int32)(time64-((unsigned __int64)now->seconds*10000000));

    return ATX_SUCCESS;
}
#else
/*----------------------------------------------------------------------
|   ATX_System_GetCurrentTimeStamp
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now)
{
    struct _timeb time_stamp;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    _ftime_s(&time_stamp);
#else
    _ftime(&time_stamp);
#endif
    now->seconds     = (long)time_stamp.time;
    now->nanoseconds = (long)time_stamp.millitm*1000000;

    return ATX_SUCCESS;
}
#endif

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

