/*****************************************************************
|
|   Atomix - Null/Stub Threads
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
|   includes
+---------------------------------------------------------------------*/
#include "AtxThreads.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_GetCurrentThreadId
+---------------------------------------------------------------------*/
ATX_ThreadId
ATX_GetCurrentThreadId(void)
{
    return 0;
}

/*----------------------------------------------------------------------
|   ATX_Mutex_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_Mutex_Create(ATX_Mutex** mutex)
{
    *mutex = ATX_AllocateMemory(1);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Mutex_LockAutoCreate
+---------------------------------------------------------------------*/
ATX_Result
ATX_Mutex_LockAutoCreate(ATX_Mutex** mutex)
{
    *mutex = ATX_AllocateMemory(1);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Mutex_Lock
+---------------------------------------------------------------------*/
ATX_Result
ATX_Mutex_Lock(ATX_Mutex* mutex)
{
    ATX_COMPILER_UNUSED(mutex);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Mutex_Unlock
+---------------------------------------------------------------------*/
ATX_Result
ATX_Mutex_Unlock(ATX_Mutex* mutex)
{
    ATX_COMPILER_UNUSED(mutex);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Mutex_Destroy
+---------------------------------------------------------------------*/
ATX_Result
ATX_Mutex_Destroy(ATX_Mutex* mutex)
{
    ATX_FreeMemory(mutex);

    return ATX_SUCCESS;
}
