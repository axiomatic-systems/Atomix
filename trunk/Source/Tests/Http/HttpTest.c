/*****************************************************************
|
|      File: HttpTest.c
|
|      Atomix Tests - Http
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "Atomix.h"

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#define CHECK_RESULT(r, m)                  \
    do {                                    \
        if (ATX_FAILED(r)) {                \
            ATX_Debug("%s (%d)\n", m, r);   \
            exit(1);                        \
        }                                   \
    } while(0)                              \

#if 0
/*----------------------------------------------------------------------
|       ConnectClient
+---------------------------------------------------------------------*/
static ATX_Result
ConnectClient(ATX_ClientSocket* client, 
              char*             hostname, 
              ATX_SocketPort    port,
              ATX_ByteStream*   stream)
{
    ATX_Socket socket;
    ATX_Result result;
    
    ATX_Debug("connecting to %s on port %d\n", hostname, port);

    /* connect client */
    result = ATX_ClientSocket_Connect(client, hostname, port, 10000);
    if (result != ATX_SUCCESS) {
        ATX_Debug("ERROR: connection failed (%d)\n", result);
        return result;
    }
    ATX_Debug("connected\n");

    /* cast to ATX_Socket interface */
    result = ATX_CAST_OBJECT(client, &socket, ATX_Socket);
    if (result != ATX_SUCCESS) {
        ATX_Debug("ERROR: client object does not implement the "
                  "ATX_Socket interface\n");
        return result;
    }

    /* get socket stream */
    result = ATX_Socket_GetStream(&socket, stream);
    if (result != ATX_SUCCESS) {
        ATX_Debug("ERROR: cannot get socket stream\n");
        return result;
    }

    /* check stream */
    if (ATX_OBJECT_IS_NULL(stream)) {
        ATX_Debug("ERROR: stream is NULL\n");
        return ATX_FAILURE;
    }

    return ATX_SUCCESS;
}

#endif

/*----------------------------------------------------------------------
|       main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_HttpClient*   client;
    ATX_HttpRequest*  request;
    ATX_HttpResponse* response;
    ATX_InputStream   response_body = ATX_NULL_OBJECT;
    ATX_Size          response_body_size = 0;
    ATX_Result        result;

    /* create a request */
    result = ATX_HttpRequest_Create(ATX_HTTP_METHOD_GET, "http://zebulon.bok.net/tmp/redirect", &request);
    CHECK_RESULT(result, "ATX_HttpRequest_Create failed");
    
    /* create a client */
    result = ATX_HttpClient_Create(&client);
    CHECK_RESULT(result, "ATX_HttpClient_Create failed");

    /* send the request and get a response */
    result = ATX_HttpClient_SendRequest(client, request, &response);
    CHECK_RESULT(result, "ATX_HttpClient_SendRequest failed");

    /* print the response */
    ATX_Debug("StatusCode = %d\n", ATX_HttpResponse_GetStatusCode(response));
    ATX_Debug("ResonPhrase = %s\n", 
              ATX_String_GetChars(ATX_HttpResponse_GetReasonPhrase(response)));
    ATX_HttpMessage_GetBody((const ATX_HttpMessage*)response, NULL, &response_body_size);
    ATX_Debug("BodySize = %d\n", response_body_size);


    /* cleanup */
    ATX_RELEASE_OBJECT(&response_body);
    ATX_HttpResponse_Destroy(response);
    ATX_HttpRequest_Destroy(request);
    ATX_HttpClient_Destroy(client);

    return 0;
}

