/*****************************************************************
|
|      File: AtxBsdSockets.h
|
|      Atomix - Sockets: BSD Implementation
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#if defined(WIN32)
#define STRICT
#include <windows.h>
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#endif /* defined(WIN32) */

#include "AtxConfig.h"
#include "AtxInterfaces.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"
#include "AtxTypes.h"
#include "AtxErrors.h"
#include "AtxStreams.h"
#include "AtxSockets.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/
#define ATX_TCP_SERVER_SOCKET_DEFAULT_LISTEN_COUNT  20

/*----------------------------------------------------------------------
|       WinSock adaptation layer
+---------------------------------------------------------------------*/
#if defined(WIN32)
#define EINPROGRESS  WSAEWOULDBLOCK
#define ECONNREFUSED WSAECONNREFUSED
#define ETIMEDOUT    WSAETIMEDOUT
#define EAGAIN       WSAEWOULDBLOCK
#define EINTR		 WSAEINTR
#define ATX_BSD_INVALID_SOCKET INVALID_SOCKET
#define ATX_BSD_SOCKET_ERROR SOCKET_ERROR
typedef int    ssize_t;
typedef int    socklen_t;
typedef char*  SocketBuffer;
typedef const char*  SocketConstBuffer;
typedef char*  SocketOption;
typedef SOCKET SocketFd;
#define GetSocketError() WSAGetLastError()
static ATX_Boolean WinsockInitialized = ATX_FALSE;
#else /* defined(WIN32) */
typedef void*  SocketBuffer;
typedef const void*  SocketConstBuffer;
typedef void*  SocketOption;
typedef int    SocketFd;
#define GetSocketError() errno
#define closesocket  close
#define ioctlsocket ioctl
#define ATX_BSD_INVALID_SOCKET (-1)
#define ATX_BSD_SOCKET_ERROR   (-1)
#endif /* defined(WIN32) */

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal reference_count;
    SocketFd     fd;
} BsdSocketFdWrapper;

typedef struct {
    ATX_Cardinal        reference_count;
    BsdSocketFdWrapper* socket_ref;
} BsdSocketStream;

typedef struct {
    BsdSocketFdWrapper* socket_ref;
    ATX_SocketInfo      info;
} BsdSocket;

typedef struct {
    BsdSocket    base;
    unsigned int max_clients;
} BsdTcpServerSocket;

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocketStream)
static const ATX_InputStreamInterface BsdSocketStream_ATX_InputStreamInterface;
static const ATX_OutputStreamInterface BsdSocketStream_ATX_OutputStreamInterface;

ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocket)
static const ATX_SocketInterface BsdSocket_ATX_SocketInterface;

ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpClientSocket)
static const ATX_SocketInterface BsdTcpClientSocket_ATX_SocketInterface;

ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpServerSocket)
static const ATX_SocketInterface BsdTcpServerSocket_ATX_SocketInterface;
static const ATX_ServerSocketInterface BsdTcpServerSocket_ATX_ServerSocketInterface;

ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdUdpSocket)
static const ATX_SocketInterface BsdUdpSocket_ATX_SocketInterface;
static const ATX_DatagramSocketInterface BsdUdpSocket_ATX_DatagramSocketInterface;

/*----------------------------------------------------------------------
|       BsdSockets_Init
+---------------------------------------------------------------------*/
static ATX_Result
BsdSockets_Init(void)
{
#if defined(WIN32)
	if (WinsockInitialized == ATX_FALSE) {
		WORD    wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD( 1, 1 );
		if (WSAStartup( wVersionRequested, &wsaData ) != 0) {
			return ATX_FAILURE;
		}
		WinsockInitialized = ATX_TRUE;
	}
#endif
	return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       SocketAddressToInetAddress
+---------------------------------------------------------------------*/
static void
SocketAddressToInetAddress(const ATX_SocketAddress* socket_address, 
                           struct sockaddr_in*      inet_address)
{
    /* initialize the structure */
    int i;
    for (i=0; i<8; i++) inet_address->sin_zero[i]=0;

    /* setup the structure */
    inet_address->sin_family = AF_INET;
    inet_address->sin_port = htons(socket_address->port);
    inet_address->sin_addr.s_addr = htonl(ATX_IpAddress_AsLong(&socket_address->ip_address));
}

/*----------------------------------------------------------------------
|       InetAddressToSocketAddress
+---------------------------------------------------------------------*/
static void
InetAddressToSocketAddress(const struct sockaddr_in* inet_address,
                           ATX_SocketAddress*        socket_address)
{
    socket_address->port = ntohs(inet_address->sin_port);
    ATX_IpAddress_SetFromLong(&socket_address->ip_address, 
                              ntohl(inet_address->sin_addr.s_addr));
}

/*----------------------------------------------------------------------
|       ResolveName
+---------------------------------------------------------------------*/
ATX_Result
ATX_IpAddress_ResolveName(ATX_IpAddress* address,
                          const char*    name, 
                          ATX_Timeout    timeout)
{
    /* make sure the TCP/IP stack is initialized */
    ATX_CHECK(BsdSockets_Init());

    /* get rid of compiler warnings */
    timeout = timeout;

    /* check parameters */
    if (name == NULL || name[0] == '\0') return ATX_ERROR_HOST_UNKNOWN;

    /* handle numerical addrs */
    {
        ATX_IpAddress numerical_address;
        if (ATX_SUCCEEDED(ATX_IpAddress_Parse(&numerical_address, name))) {
            /* the name is a numerical IP addr */
            ATX_IpAddress_Copy(address, &numerical_address);
            return ATX_SUCCESS;
        }
    }

    /* do a name lookup */
    {
        struct hostent *host_entry;
        host_entry = gethostbyname(name);
        if (host_entry == NULL ||
            host_entry->h_addrtype != AF_INET) {
            return ATX_ERROR_HOST_UNKNOWN;
        }
        ATX_CopyMemory(address, host_entry->h_addr, 4);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketFdWrapper_Create
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocketFdWrapper_Create(SocketFd fd, BsdSocketFdWrapper** wrapper)
{
    /* allocate a new object */
    (*wrapper) = ATX_AllocateZeroMemory(sizeof(BsdSocketFdWrapper));
    if (*wrapper == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*wrapper)->fd = fd;
    (*wrapper)->reference_count = 1;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketFdWrapper_Destroy
+---------------------------------------------------------------------*/
static void
BsdSocketFdWrapper_Destroy(BsdSocketFdWrapper* wrapper)
{
    closesocket(wrapper->fd);
    ATX_FreeMemory((void*)wrapper);
}

/*----------------------------------------------------------------------
|       BsdSocketFdWrapper_AddReference
+---------------------------------------------------------------------*/
static void
BsdSocketFdWrapper_AddReference(BsdSocketFdWrapper* wrapper)
{
    ++wrapper->reference_count;
}

/*----------------------------------------------------------------------
|       BsdSocketFdWrapper_Release
+---------------------------------------------------------------------*/
static void
BsdSocketFdWrapper_Release(BsdSocketFdWrapper* wrapper)
{
    if (wrapper == NULL) return;
    if (--wrapper->reference_count == 0) {
        BsdSocketFdWrapper_Destroy(wrapper);
    }
}

/*----------------------------------------------------------------------
|       BsdSocketStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocketStream_Create(BsdSocketFdWrapper* socket_ref, BsdSocketStream** stream)
{
    /* create a new object */
    (*stream) = (BsdSocketStream*)ATX_AllocateMemory(sizeof(BsdSocketStream));
    if (*stream == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*stream)->reference_count = 1;
    (*stream)->socket_ref = socket_ref;

    /* keep a reference */
    BsdSocketFdWrapper_AddReference(socket_ref);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketInputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocketInputStream_Create(BsdSocketFdWrapper* socket_ref, 
                            ATX_InputStream*    stream)
{ 
    BsdSocketStream* socket_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = BsdSocketStream_Create(socket_ref, &socket_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_InputStreamInstance*)socket_stream;
    ATX_INTERFACE(stream) = &BsdSocketStream_ATX_InputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketOuputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocketOutputStream_Create(BsdSocketFdWrapper* socket_ref, 
                             ATX_OutputStream*   stream)
{ 
    BsdSocketStream* socket_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = BsdSocketStream_Create(socket_ref, &socket_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_OutputStreamInstance*)socket_stream;
    ATX_INTERFACE(stream) = &BsdSocketStream_ATX_OutputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocketStream_Destroy(BsdSocketStream* stream)
{
    BsdSocketFdWrapper_Release(stream->socket_ref);
    ATX_FreeMemory((void*)stream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketStream_Read(ATX_InputStreamInstance* instance,
                     ATX_Any                  buffer, 
                     ATX_Size                 bytes_to_read, 
                     ATX_Size*                bytes_read)
{
    BsdSocketStream* stream = (BsdSocketStream*)instance;
    ssize_t          nb_read;
    int              watchdog = 5000;

    do {
        nb_read = recv(stream->socket_ref->fd, 
                       buffer, 
                       (ssize_t)bytes_to_read, 
                       0);
        if (nb_read > 0) {
            if (bytes_read) *bytes_read = nb_read;
            return ATX_SUCCESS;
        } else {
            if (bytes_read) *bytes_read = 0;
            if (nb_read == 0) {
                return ATX_ERROR_EOS;
            } else {
                /* on linux, EAGAIN can be returned for UDP sockets */
                /* when the checksum fails                          */
                if (GetSocketError() == EAGAIN) {
                    return ATX_SUCCESS;
                }

                /* dont return an error if we get interrupted */
                if (GetSocketError() != EINTR) {
                    return ATX_FAILURE;
                } 
            }
        }
    } while (watchdog--);

    if (bytes_read) *bytes_read = 0;
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       BsdSocketStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketStream_Write(ATX_OutputStreamInstance* instance,
                      ATX_AnyConst              buffer, 
                      ATX_Size                  bytes_to_write, 
                      ATX_Size*                 bytes_written)
{
    BsdSocketStream* stream = (BsdSocketStream*)instance;
    ssize_t          nb_written;

    nb_written = send(stream->socket_ref->fd, 
                      (SocketConstBuffer)buffer, 
                      (ssize_t)bytes_to_write, 
                      0);

    if (nb_written > 0) {
        if (bytes_written) *bytes_written = nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        if (nb_written == 0) {
            return ATX_ERROR_DISCONNECTED;
        } else {
            return ATX_FAILURE;
        }
    }
}

/*----------------------------------------------------------------------
|       BsdSocketInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketInputStream_Seek(ATX_InputStreamInstance* instance, 
                          ATX_Offset               where)
{
    /* can't seek in socket streams */
    ATX_COMPILER_UNUSED(instance);
    ATX_COMPILER_UNUSED(where);
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       BsdSocketInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketInputStream_Tell(ATX_InputStreamInstance* instance, 
                          ATX_Offset*              where)
{
    ATX_COMPILER_UNUSED(instance);

    if (where) *where = 0;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketInputStream_GetSize(ATX_InputStreamInstance* instance, 
                             ATX_Size*                size)
{
    ATX_COMPILER_UNUSED(instance);

    if (size) *size = 0;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketInputStream_GetAvailable(ATX_InputStreamInstance* instance, 
                                  ATX_Size*                available)
{
    BsdSocketStream* stream = (BsdSocketStream*)instance;
    unsigned long ready = 0;
    int io_result = ioctlsocket(stream->socket_ref->fd, FIONREAD, &ready); 
    if (io_result == ATX_BSD_SOCKET_ERROR) {
        *available = 0;
        return ATX_FAILURE;
    } else {
        *available = ready;
        return ATX_SUCCESS;
    }
}

/*----------------------------------------------------------------------
|       BsdSocketOutputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketOutputStream_Seek(ATX_OutputStreamInstance* instance, 
                           ATX_Offset                where)
{
    ATX_COMPILER_UNUSED(instance);
    ATX_COMPILER_UNUSED(where);

    /* can't seek in socket streams */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       BsdSocketOutputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketOutputStream_Tell(ATX_OutputStreamInstance* instance, 
                           ATX_Offset*               where)
{
    ATX_COMPILER_UNUSED(instance);

    if (where) *where = 0;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocketOutputStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocketOutputStream_Flush(ATX_OutputStreamInstance* instance)
{
    ATX_COMPILER_UNUSED(instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_InputStream interface
+---------------------------------------------------------------------*/
static const ATX_InputStreamInterface
BsdSocketStream_ATX_InputStreamInterface = {
    BsdSocketStream_GetInterface,
    BsdSocketStream_Read,
    BsdSocketInputStream_Seek,
    BsdSocketInputStream_Tell,
    BsdSocketInputStream_GetSize,
    BsdSocketInputStream_GetAvailable
};

/*----------------------------------------------------------------------
|       ATX_OutputStream interface
+---------------------------------------------------------------------*/
static const ATX_OutputStreamInterface
BsdSocketStream_ATX_OutputStreamInterface = {
    BsdSocketStream_GetInterface,
    BsdSocketStream_Write,
    BsdSocketOutputStream_Seek,
    BsdSocketOutputStream_Tell,
    BsdSocketOutputStream_Flush
};

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_REFERENCEABLE_INTERFACE(BsdSocketStream, reference_count)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocketStream) 
ATX_INTERFACE_MAP_ADD(BsdSocketStream, ATX_InputStream)
ATX_INTERFACE_MAP_ADD(BsdSocketStream, ATX_OutputStream)
ATX_INTERFACE_MAP_ADD(BsdSocketStream, ATX_Referenceable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocketStream)

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
static ATX_Result BsdSocket_RefreshInfo(BsdSocket* socket);

/*----------------------------------------------------------------------
|       BsdSocket_Construct
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_Construct(BsdSocket* socket, SocketFd fd)
{ 
    ATX_Result result;

    /* create a reference to the fd */
    result = BsdSocketFdWrapper_Create(fd, &socket->socket_ref);
    if (ATX_FAILED(result)) return result;

    /* get initial info */
    BsdSocket_RefreshInfo(socket);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocket_Destruct
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_Destruct(BsdSocket* socket)
{
    BsdSocketFdWrapper_Release(socket->socket_ref);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocket_Create
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_Create(SocketFd fd, ATX_Socket* object)
{ 
    BsdSocket* socket;

    /* allocate new object */
    socket = (BsdSocket*)ATX_AllocateZeroMemory(sizeof(BsdSocket));
    if (socket == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    BsdSocket_Construct(socket, fd);

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_SocketInstance*)socket;
    ATX_INTERFACE(object) = &BsdSocket_ATX_SocketInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocket_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_Destroy(ATX_DestroyableInstance* instance)
{
    BsdSocket* socket = (BsdSocket*)instance;
    BsdSocket_Destruct(socket);
    ATX_FreeMemory((void*)instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocket_RefreshInfo
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_RefreshInfo(BsdSocket* socket)
{
    struct sockaddr_in inet_address;
    socklen_t          name_length = sizeof(inet_address);

    /* check that we have a socket */
    if (socket->socket_ref == NULL) return ATX_ERROR_INVALID_STATE;

    /* get the local socket addr */
    if (getsockname(socket->socket_ref->fd, 
                    (struct sockaddr*)&inet_address, 
                    &name_length) == 0) {
        ATX_IpAddress_SetFromLong(&socket->info.local_address.ip_address,
                                  ntohl(inet_address.sin_addr.s_addr));
        socket->info.local_address.port = ntohs(inet_address.sin_port);
    }   

    /* get the peer socket addr */
    if (getpeername(socket->socket_ref->fd,
                    (struct sockaddr*)&inet_address, 
                    &name_length) == 0) {
        ATX_IpAddress_SetFromLong(&socket->info.remote_address.ip_address,
                                  ntohl(inet_address.sin_addr.s_addr));
        socket->info.remote_address.port = ntohs(inet_address.sin_port);
    }   

    return ATX_SUCCESS;
}

#if defined(WIN32)
/*----------------------------------------------------------------------
|       BsdSocketFd_SetBlockingMode
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_SetBlockingMode(BsdSocket* socket, ATX_Boolean blocking)
{
    unsigned long args = (blocking == ATX_TRUE) ? 0 : 1;
    if (ioctlsocket(socket->socket_ref->fd, FIONBIO, &args)) {
        return ATX_FAILURE;
    }
    return ATX_SUCCESS;
}
#else
/*----------------------------------------------------------------------
|       BsdSocketFd_SetBlockingMode
+---------------------------------------------------------------------*/
static ATX_Result
BsdSocket_SetBlockingMode(BsdSocket* socket, ATX_Boolean blocking)
{
    int flags = fcntl(socket->socket_ref->fd, F_GETFL, 0);
    if (blocking == ATX_TRUE) {
        flags ^= O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }
    if (fcntl(socket->socket_ref->fd, F_SETFL, flags)) {
        return ATX_FAILURE;
    }
    return ATX_SUCCESS;
}
#endif

/*----------------------------------------------------------------------
|       BsdSocket_Bind
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocket_Bind(ATX_SocketInstance* instance, const ATX_SocketAddress* address)
{
    BsdSocket* socket = (BsdSocket*)instance;

    /* convert the address */
    struct sockaddr_in inet_address;
    SocketAddressToInetAddress(address, &inet_address);
    
    /* bind the socket */
    if (bind(socket->socket_ref->fd, 
             (struct sockaddr*)&inet_address, 
             sizeof(inet_address)) < 0) {
        return ATX_ERROR_BIND_FAILED;
    }

    /* refresh socket info */
    BsdSocket_RefreshInfo(socket);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdSocket_Connect
+---------------------------------------------------------------------*/
ATX_METHOD 
BsdSocket_Connect(ATX_SocketInstance*      instance, 
                  const ATX_SocketAddress* address, 
                  ATX_Timeout              timeout)
{
    ATX_COMPILER_UNUSED(instance);
    ATX_COMPILER_UNUSED(address);
    ATX_COMPILER_UNUSED(timeout);

    /* this is unsupported unless overridden in a derived class */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       BsdSocket_GetInputStream
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocket_GetInputStream(ATX_SocketInstance* instance, ATX_InputStream* stream)
{
    BsdSocket* socket = (BsdSocket*)instance;

    /* check that we have a socket */
    if (socket->socket_ref == NULL) return ATX_ERROR_INVALID_STATE;

    /* create a stream */
    return BsdSocketInputStream_Create(socket->socket_ref, stream);
}

/*----------------------------------------------------------------------
|       BsdSocket_GetOutputStream
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocket_GetOutputStream(ATX_SocketInstance* instance, ATX_OutputStream* stream)
{
    BsdSocket* socket = (BsdSocket*)instance;

    /* check that we have a socket */
    if (socket->socket_ref == NULL) return ATX_ERROR_INVALID_STATE;

    /* create a stream */
    return BsdSocketOutputStream_Create(socket->socket_ref, stream);
}

/*----------------------------------------------------------------------
|       BsdSocket_GetInfo
+---------------------------------------------------------------------*/
ATX_METHOD
BsdSocket_GetInfo(ATX_SocketInstance* instance, ATX_SocketInfo* info)
{
    BsdSocket* socket = (BsdSocket*)instance;

    /* return the cached info */
    *info = socket->info;
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_Socket interface
+---------------------------------------------------------------------*/
static const ATX_SocketInterface
BsdSocket_ATX_SocketInterface = {
    BsdSocket_GetInterface,
    BsdSocket_Bind,
    BsdSocket_Connect,
    BsdSocket_GetInputStream,
    BsdSocket_GetOutputStream,
    BsdSocket_GetInfo
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(BsdSocket)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocket) 
ATX_INTERFACE_MAP_ADD(BsdSocket, ATX_Socket)
ATX_INTERFACE_MAP_ADD(BsdSocket, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdSocket)

/*----------------------------------------------------------------------
|       ATX_UdpSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_UdpSocket_Create(ATX_DatagramSocket* object)
{ 
    BsdSocket* sock;

    /* make sure the TCP/IP stack is initialized */
    ATX_CHECK(BsdSockets_Init());

    /* allocate new object */
    sock = (BsdSocket*)ATX_AllocateZeroMemory(sizeof(BsdSocket));
    if (socket == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    BsdSocket_Construct(sock, socket(AF_INET, SOCK_DGRAM, 0));

    /* set default socket options */
    {
        int option = 1;
        setsockopt(sock->socket_ref->fd, 
                   SOL_SOCKET, 
                   SO_REUSEADDR, 
                   (SocketOption)&option, 
                   sizeof(option));
    }

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_DatagramSocketInstance*)sock;
    ATX_INTERFACE(object) = &BsdUdpSocket_ATX_DatagramSocketInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdUdpSocket_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
BsdUdpSocket_Destroy(ATX_DestroyableInstance* instance)
{
    BsdSocket* socket = (BsdSocket*)instance;

    BsdSocket_Destruct(socket);
    ATX_FreeMemory((void*)socket);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdUdpSocket_Connect
+---------------------------------------------------------------------*/
ATX_METHOD
BsdUdpSocket_Connect(ATX_SocketInstance*      instance,
                     const ATX_SocketAddress* address,
                     ATX_Timeout              timeout)
{
    BsdSocket*         socket = (BsdSocket*)instance;
    struct sockaddr_in inet_address;
    int                io_result;
    
    /* avoid compiler warnings */
    timeout = timeout;

    /* setup an address structure */
    SocketAddressToInetAddress(address, &inet_address);

    /* connect so that we can have some addr bound to the socket */
    io_result = connect(socket->socket_ref->fd, 
                        (struct sockaddr *)&inet_address, 
                        sizeof(inet_address));
    if (io_result == ATX_BSD_SOCKET_ERROR) { 
        return ATX_FAILURE;
    }
    
    /* set default socket options */
    {
        int option = 1;
        setsockopt(socket->socket_ref->fd, 
                SOL_SOCKET, 
                SO_BROADCAST, 
                (SocketOption)&option, 
                sizeof(option));
    }

    /* refresh socket info */
    BsdSocket_RefreshInfo(socket);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdUdpSocket_Send
+---------------------------------------------------------------------*/
ATX_METHOD 
BsdUdpSocket_Send(ATX_DatagramSocketInstance* instance,
                  const ATX_DataBuffer*       packet, 
                  const ATX_SocketAddress*    address) 
{
    BsdSocket* socket = (BsdSocket*)instance;
    int        io_result;

    /* get the packet buffer */
    const ATX_Byte* buffer        = ATX_DataBuffer_GetData(packet);
    ssize_t         buffer_length = ATX_DataBuffer_GetDataSize(packet);

    /* send the packet buffer */
    if (address != NULL) {
        /* send to the specified socket */

        /* setup an address structure */
        struct sockaddr_in inet_address;
        SocketAddressToInetAddress(address, &inet_address);
        io_result = sendto(socket->socket_ref->fd, 
                           (SocketConstBuffer)buffer, 
                           buffer_length, 
                           0, 
                           (struct sockaddr *)&inet_address, 
                           sizeof(inet_address));
    } else {
        /* send to whichever addr the socket is connected */
        io_result = send(socket->socket_ref->fd, 
                         (SocketConstBuffer)buffer, 
                         buffer_length,
                         0);
    }

    /* check the result */
    if (io_result == ATX_BSD_SOCKET_ERROR) {
        return ATX_FAILURE;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdUdpSocket_Receive
+---------------------------------------------------------------------*/
ATX_METHOD 
BsdUdpSocket_Receive(ATX_DatagramSocketInstance* instance,
                     ATX_DataBuffer*             packet, 
                     ATX_SocketAddress*          address)
{
    BsdSocket* socket = (BsdSocket*)instance;
    int        io_result;

    /* get the packet buffer */
    ATX_Byte* buffer        = ATX_DataBuffer_UseData(packet);
    ssize_t   buffer_length = ATX_DataBuffer_GetDataSize(packet);;

    /* check that we have some space to receive */
    if (buffer_length == 0) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* receive a packet */
    if (address != NULL) {
        struct sockaddr_in inet_address;
        socklen_t          inet_address_length = sizeof(inet_address);
        io_result = recvfrom(socket->socket_ref->fd, 
                             (SocketBuffer)buffer, 
                             buffer_length, 
                             0, 
                             (struct sockaddr *)&inet_address, 
                             &inet_address_length);

        /* convert the address format */
        if (io_result != ATX_BSD_SOCKET_ERROR) {
            if (inet_address_length == sizeof(inet_address)) {
                InetAddressToSocketAddress(&inet_address, address);
            }
            ATX_DataBuffer_SetDataSize(packet, io_result);
        } else {
            ATX_DataBuffer_SetDataSize(packet, 0);
        }
    } else {
        io_result = recv(socket->socket_ref->fd,
                         (SocketBuffer)buffer,
                         buffer_length,
                         0);
    }

    /* check the result */
    if (io_result == ATX_BSD_SOCKET_ERROR) {
        return ATX_FAILURE;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_Socket interface
+---------------------------------------------------------------------*/
static const ATX_SocketInterface
BsdUdpSocket_ATX_SocketInterface = {
    BsdUdpSocket_GetInterface,
    BsdSocket_Bind,
    BsdUdpSocket_Connect,
    BsdSocket_GetInputStream,
    BsdSocket_GetOutputStream,
    BsdSocket_GetInfo
};

/*----------------------------------------------------------------------
|       ATX_DatagramSocket interface
+---------------------------------------------------------------------*/
static const ATX_DatagramSocketInterface
BsdUdpSocket_ATX_DatagramSocketInterface = {
    BsdUdpSocket_GetInterface,
    BsdUdpSocket_Send,
    BsdUdpSocket_Receive
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(BsdUdpSocket)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdUdpSocket) 
ATX_INTERFACE_MAP_ADD(BsdUdpSocket, ATX_Socket)
ATX_INTERFACE_MAP_ADD(BsdUdpSocket, ATX_DatagramSocket)
ATX_INTERFACE_MAP_ADD(BsdUdpSocket, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdUdpSocket)

/*----------------------------------------------------------------------
|       ATX_TcpClientSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_TcpClientSocket_Create(ATX_Socket* object)
{ 
    BsdSocket* sock;

    /* make sure the TCP/IP stack is initialized */
    ATX_CHECK(BsdSockets_Init());

    /* allocate new object */
    sock = (BsdSocket*)ATX_AllocateZeroMemory(sizeof(BsdSocket));
    if (sock == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    BsdSocket_Construct(sock, socket(AF_INET, SOCK_STREAM, 0));

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_SocketInstance*)sock;
    ATX_INTERFACE(object) = &BsdTcpClientSocket_ATX_SocketInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       TcpClientSocket_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
BsdTcpClientSocket_Destroy(ATX_DestroyableInstance* instance)
{
    return BsdSocket_Destroy(instance); 
}

/*----------------------------------------------------------------------
|       BsdTcpClientSocket_Connect
+---------------------------------------------------------------------*/
ATX_METHOD
BsdTcpClientSocket_Connect(ATX_SocketInstance*      instance,
                           const ATX_SocketAddress* address,
                           ATX_Timeout              timeout)
{
    BsdSocket*          socket = (BsdSocket*)instance;
    struct sockaddr_in  inet_address;
    SocketFd            socket_fd = socket->socket_ref->fd;
    int                 io_result;
    fd_set              read_set;
    fd_set              write_set;
    fd_set              except_set;
    struct timeval      timeout_value;

    /* set the socket to nonblocking so that we can timeout on connect */
    if (BsdSocket_SetBlockingMode(socket, ATX_FALSE)) {
        return ATX_FAILURE;
    }

    /* convert the address */
    SocketAddressToInetAddress(address, &inet_address);

    /* initiate connection */
    io_result = connect(socket_fd, 
                        (struct sockaddr *)&inet_address, 
                        sizeof(inet_address));
    if (io_result == 0) {
        /* immediate connection */
        BsdSocket_SetBlockingMode(socket, ATX_TRUE);

        /* get socket info */
        BsdSocket_RefreshInfo(socket);

        return ATX_SUCCESS;
    }
    if (io_result == ATX_BSD_SOCKET_ERROR && GetSocketError() != EINPROGRESS) {
        /* error */
        return ATX_FAILURE;
    }

    /* wait for connection to succeed or fail */
    FD_ZERO(&read_set);
    FD_SET(socket_fd, &read_set);
    FD_ZERO(&write_set);
    FD_SET(socket_fd, &write_set);
    FD_ZERO(&except_set);
    FD_SET(socket_fd, &except_set);

    if (timeout != ATX_SOCKET_TIMEOUT_INFINITE) {
        timeout_value.tv_sec = timeout/1000;
        timeout_value.tv_usec = 1000*(timeout-1000*(timeout/1000));
    }

    io_result = select((int)(socket_fd+1), &read_set, &write_set, &except_set, 
                       timeout == ATX_SOCKET_TIMEOUT_INFINITE ? 
                       NULL : &timeout_value);
    if (io_result == ATX_BSD_SOCKET_ERROR) {
        /* select error */
        return ATX_ERROR_SELECT_FAILED;
    } else if (io_result == 0) {
        /* timeout */
        return ATX_ERROR_TIMEOUT;
    }
    if (FD_ISSET(socket_fd, &read_set) || 
        FD_ISSET(socket_fd, &write_set) ||
        FD_ISSET(socket_fd, &except_set)) {
        int       error;
        socklen_t length = sizeof(error);

        /* get error status from socket */
        if (getsockopt(socket_fd, 
                       SOL_SOCKET, 
                       SO_ERROR, 
                       (SocketOption)&error, 
                       &length) == ATX_BSD_SOCKET_ERROR) {
            return ATX_ERROR_CONNECTION_FAILED;
        }
        if (error) {
            if (error == ECONNREFUSED) {
                return ATX_ERROR_CONNECTION_REFUSED;
            } else if (error == ETIMEDOUT) {
                return ATX_ERROR_TIMEOUT;
            } else {
                return ATX_ERROR_CONNECTION_FAILED;
            }
        }
    }
    
    /* put the socket back in blocking mode */
    BsdSocket_SetBlockingMode(socket, ATX_TRUE);

    /* get socket info */
    BsdSocket_RefreshInfo(socket);

    /* done */
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_Socket interface
+---------------------------------------------------------------------*/
static const ATX_SocketInterface
BsdTcpClientSocket_ATX_SocketInterface = {
    BsdTcpClientSocket_GetInterface,
    BsdSocket_Bind,
    BsdTcpClientSocket_Connect,
    BsdSocket_GetInputStream,
    BsdSocket_GetOutputStream,
    BsdSocket_GetInfo
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(BsdTcpClientSocket)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpClientSocket) 
ATX_INTERFACE_MAP_ADD(BsdTcpClientSocket, ATX_Socket)
ATX_INTERFACE_MAP_ADD(BsdTcpClientSocket, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpClientSocket)

/*----------------------------------------------------------------------
|       ATX_TcpServerSocket_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_TcpServerSocket_Create(ATX_ServerSocket* object)
{ 
    BsdTcpServerSocket* server;

    /* make sure the TCP/IP stack is initialized */
    ATX_CHECK(BsdSockets_Init());

    /* allocate new object */
    server = (BsdTcpServerSocket*)ATX_AllocateZeroMemory(sizeof(BsdTcpServerSocket));
    if (server == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    BsdSocket_Construct(&server->base, socket(AF_INET, SOCK_STREAM, 0));
    server->max_clients = 0;

    /* set socket options */
    { 
        int option = 1;
        setsockopt(server->base.socket_ref->fd, 
                SOL_SOCKET, 
                SO_REUSEADDR, 
                (SocketOption)&option, 
                sizeof(option));
    }

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_ServerSocketInstance*)server;
    ATX_INTERFACE(object) = &BsdTcpServerSocket_ATX_ServerSocketInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       TcpServerSocket_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
BsdTcpServerSocket_Destroy(ATX_DestroyableInstance* instance)
{
    return BsdSocket_Destroy(instance); 
}

/*----------------------------------------------------------------------
|       BsdTcpServerSocket_Listen
+---------------------------------------------------------------------*/
ATX_METHOD
BsdTcpServerSocket_Listen(ATX_ServerSocketInstance* instance, unsigned int max_clients)
{
    BsdTcpServerSocket* socket = (BsdTcpServerSocket*)instance;

    /* listen for connections */
    if (listen(socket->base.socket_ref->fd, max_clients) < 0) {
        socket->max_clients = 0;
        return ATX_ERROR_LISTEN_FAILED;
    }   
    socket->max_clients = max_clients;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       BsdTcpServerSocket_WaitForNewClient
+---------------------------------------------------------------------*/
ATX_METHOD
BsdTcpServerSocket_WaitForNewClient(ATX_ServerSocketInstance* instance,
                                    ATX_Socket*               client)
{
    BsdTcpServerSocket* server = (BsdTcpServerSocket*)instance;
    struct sockaddr_in  inet_address;
    socklen_t           namelen = sizeof(inet_address);
    SocketFd            socket_fd;
    ATX_Result          result;

    /* check that we are listening for clients */
    if (server->max_clients == 0) {
        BsdTcpServerSocket_Listen(instance, ATX_TCP_SERVER_SOCKET_DEFAULT_LISTEN_COUNT);
    }

    /* wait for incoming connection */
    socket_fd = accept(server->base.socket_ref->fd, 
                       (struct sockaddr*)&inet_address, 
                       &namelen); 
    if (socket_fd == ATX_BSD_INVALID_SOCKET) {
        ATX_CLEAR_OBJECT(client);
        return ATX_ERROR_ACCEPT_FAILED;
    }

    /* create a new client socket to wrap this file descriptor */
    result = BsdSocket_Create(server->base.socket_ref->fd, client);
    if (result != ATX_SUCCESS) return result;
    
    /* done */
    return ATX_SUCCESS;    
}

/*----------------------------------------------------------------------
|       ATX_Socket interface
+---------------------------------------------------------------------*/
static const ATX_SocketInterface
BsdTcpServerSocket_ATX_SocketInterface = {
    BsdTcpServerSocket_GetInterface,
    BsdSocket_Bind,
    BsdSocket_Connect,
    BsdSocket_GetInputStream,
    BsdSocket_GetOutputStream,
    BsdSocket_GetInfo
};

/*----------------------------------------------------------------------
|       ATX_ServerSocket interface
+---------------------------------------------------------------------*/
static const ATX_ServerSocketInterface
BsdTcpServerSocket_ATX_ServerSocketInterface = {
    BsdTcpServerSocket_GetInterface,
    BsdTcpServerSocket_Listen,
    BsdTcpServerSocket_WaitForNewClient
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(BsdTcpServerSocket)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpServerSocket) 
ATX_INTERFACE_MAP_ADD(BsdTcpServerSocket, ATX_Socket)
ATX_INTERFACE_MAP_ADD(BsdTcpServerSocket, ATX_ServerSocket)
ATX_INTERFACE_MAP_ADD(BsdTcpServerSocket, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(BsdTcpServerSocket)










