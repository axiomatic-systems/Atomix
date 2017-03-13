/*****************************************************************
|
|   Atomix - Sockets: Null/Stub Implementation
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
#include "AtxConfig.h"
#include "AtxSockets.h"

/*----------------------------------------------------------------------
|   ResolveName
+---------------------------------------------------------------------*/
ATX_Result
ATX_IpAddress_ResolveName(ATX_IpAddress* address,
                          const char*    name,
                          ATX_Timeout    timeout)
{
    ATX_COMPILER_UNUSED(address);
    ATX_COMPILER_UNUSED(name);
    ATX_COMPILER_UNUSED(timeout);

    return ATX_ERROR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------------
|   ATX_UdpSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_UdpSocket_Create(ATX_DatagramSocket** object)
{
    ATX_COMPILER_UNUSED(object);

    return ATX_ERROR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------------
|   ATX_TcpClientSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_TcpClientSocket_Create(ATX_Socket** object)
{
    ATX_COMPILER_UNUSED(object);

    return ATX_ERROR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------------
|   ATX_TcpServerSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_TcpServerSocket_Create(ATX_ServerSocket** object)
{
    ATX_COMPILER_UNUSED(object);

    return ATX_ERROR_NOT_IMPLEMENTED;
}
