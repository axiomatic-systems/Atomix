/*****************************************************************
|
|      File: AtxHttp.h
|
|      Atomix - HTTP
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_HTTP_H_
#define _ATX_HTTP_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxInterfaces.h"
#include "AtxProperties.h"
#include "AtxStreams.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct ATX_HttpClient   ATX_HttpClient;
typedef struct ATX_HttpMessage  ATX_HttpMessage;
typedef struct ATX_HttpRequest  ATX_HttpRequest;
typedef struct ATX_HttpResponse ATX_HttpResponse;

/*----------------------------------------------------------------------
|    constants
+---------------------------------------------------------------------*/
#define ATX_HTTP_METHOD_GET     "GET"
#define ATX_HTTP_METHOD_HEAD    "HEAD"
#define ATX_HTTP_METHOD_POST    "POST"

#define ATX_HTTP_CLIENT_OPTION_FOLLOW_REDIRECT "FollowRedirect"

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern ATX_Result 
ATX_HttpMessage_Create(ATX_HttpMessage** message);

extern ATX_Result
ATX_HttpMessage_Destroy(ATX_HttpMessage* message);

extern ATX_Result
ATX_HttpMessage_SetHeader(ATX_HttpMessage* message,
                          ATX_CString      name, 
                          ATX_CString      value);

extern const ATX_String*
ATX_HttpMessage_GetHeader(const ATX_HttpMessage* message,
                          ATX_CString            name);

extern ATX_Result
ATX_HttpMessage_SetProtocol(ATX_HttpMessage* message,
                            ATX_CString      protocol);

extern const ATX_String*
ATX_HttpMessage_GetProtocol(const ATX_HttpMessage* message);

extern ATX_Result
ATX_HttpMessage_SetBody(ATX_HttpMessage* message,
                        ATX_InputStream* stream,
                        ATX_Size         content_length);

extern ATX_Result
ATX_HttpMessage_GetBody(const ATX_HttpMessage* message,
                        ATX_InputStream*       stream,
                        ATX_Size*              content_length);

extern ATX_Result
ATX_HttpRequest_Create(ATX_CString       method, 
                       ATX_CString       uri,
                       ATX_HttpRequest** request);

extern ATX_Result
ATX_HttpRequest_Destroy(ATX_HttpRequest* request);

extern const ATX_String*
ATX_HttpRequest_GetMethod(const ATX_HttpRequest* request);

extern ATX_Result
ATX_HttpRequest_SetMethod(ATX_HttpRequest* request,
                          ATX_CString      method);

extern ATX_Result
ATX_HttpRequest_Emit(const ATX_HttpRequest* request,
                     ATX_OutputStream*      stream);

extern ATX_Result
ATX_HttpResponse_CreateFromStream(ATX_InputStream*   stream,
                                  ATX_HttpResponse** request);

extern ATX_Result
ATX_HttpResponse_Destroy(ATX_HttpResponse* response);

extern ATX_Result
ATX_HttpResponse_Emit(const ATX_HttpResponse* response, 
                      ATX_OutputStream*       stream);

extern ATX_UInt32
ATX_HttpResponse_GetStatusCode(const ATX_HttpResponse* response);

extern const ATX_String*
ATX_HttpResponse_GetReasonPhrase(const ATX_HttpResponse* response);

extern ATX_Result
ATX_HttpClient_Create(ATX_HttpClient** client);

extern ATX_Result
ATX_HttpClient_Destroy(ATX_HttpClient* client);

extern ATX_Result
ATX_HttpClient_SetOptionBool(ATX_HttpClient* client, 
                             ATX_CString     option,
                             ATX_Boolean     value);

extern ATX_Result
ATX_HttpClient_GetOption(const ATX_HttpClient* client, 
                             ATX_CString       option,
                             ATX_Boolean*      value);

extern ATX_Result 
ATX_HttpClient_SendRequest(ATX_HttpClient*    client,
                           ATX_HttpRequest*   request,
                           ATX_HttpResponse** response);

#ifdef __cplusplus
}
#endif

#endif /* _ATX_HTTP_H_ */
