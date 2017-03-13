/*****************************************************************
|
|   Atomix - System, Null/Stub Implementation
|
| Copyright (c) 2002-2016, Axiomatic Systems, LLC.
| All rights reserved.
|
| Redistribution and use in source and binary forms, with or without
| modification, are permitted provided that the following conditions are met:
|     * Redistributions of source code must retain the above copyright
|       notice, this list of conditions and the following disclaimer.
|     * Redistributions in binary form must reproduce the above copyright
|       notice, this list of conditions and the following disclaimer in the
|       documentation and/or other materials provided with the distribution.
|     * Neither the name of Axiomatic Systems nor the
|       names of its contributors may be used to endorse or promote products
|       derived from this software without specific prior written permission.
|
| THIS SOFTWARE IS PROVIDED BY AXIOMATIC SYSTEMS ''AS IS'' AND ANY
| EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
| WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
| DISCLAIMED. IN NO EVENT SHALL AXIOMATIC SYSTEMS BE LIABLE FOR ANY
| DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
| (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
| ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
| (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
| SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxResults.h"
#include "AtxSystem.h"

/*----------------------------------------------------------------------
|   ATX_System_GetCurrentTimeStamp
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_GetCurrentTimeStamp(ATX_TimeStamp* now)
{
    now->seconds     = 0;
    now->nanoseconds = 0;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_Sleep
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_Sleep(const ATX_TimeInterval* duration)
{
    ATX_COMPILER_UNUSED(duration);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_SleepUntil
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_SleepUntil(const ATX_TimeStamp* when)
{
    ATX_COMPILER_UNUSED(when);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System_SetRandomSeed
+---------------------------------------------------------------------*/
ATX_Result
ATX_System_SetRandomSeed(unsigned int seed)
{
    ATX_COMPILER_UNUSED(seed);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_System::ATX_System
+---------------------------------------------------------------------*/
ATX_UInt32
ATX_System_GetRandomInteger()
{
    return 0;
}
