/*****************************************************************
|
|      File: AtxWin32Debug.c
|
|      Atomix - Debug Support: Win32 Implementation
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

#include "AtxConfig.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/
#define ATX_DEBUG_LOCAL_BUFFER_SIZE 1024
#define ATX_DEBUG_BUFFER_INCREMENT  4096
#define ATX_DEBUG_BUFFER_MAX_SIZE   65536

#if defined(ATX_DEBUG)
/*----------------------------------------------------------------------
|       ATX_Print
+---------------------------------------------------------------------*/
static void
ATX_Print(const char* message)
{
    OutputDebugString(message);
    printf("%s", message);
}
#endif

/*----------------------------------------------------------------------
|       ATX_Debug
+---------------------------------------------------------------------*/
void
ATX_Debug(const char* format, ...)
{
#if defined(ATX_DEBUG)
    char         local_buffer[ATX_DEBUG_LOCAL_BUFFER_SIZE];
    unsigned int buffer_size = ATX_DEBUG_LOCAL_BUFFER_SIZE;
    char*        buffer = local_buffer;
    va_list      args;

    va_start(args, format);

    for(;;) {
        int result;

        /* try to format the message (it might not fit) */
#if (_MSC_VER >= 1400)
        /* use the secure function for VC 8 and above */
        result = _vsnprintf_s(buffer, buffer_size, _TRUNCATE, format, args);
#else
        result = _vsnprintf(buffer, buffer_size-1, format, args);
#endif
        buffer[buffer_size-1] = 0; /* force a NULL termination */
        if (result >= 0) break;

        /* the buffer was too small, try something bigger */
        buffer_size = (buffer_size+ATX_DEBUG_BUFFER_INCREMENT)*2;
        if (buffer_size > ATX_DEBUG_BUFFER_MAX_SIZE) break;
        if (buffer != local_buffer) ATX_FreeMemory((void*)buffer);
        buffer = ATX_AllocateMemory(buffer_size);
        if (buffer == NULL) return;
    }

    ATX_Print(buffer);
    if (buffer != local_buffer) ATX_FreeMemory((void*)buffer);
   
    va_end(args);
#else
    ATX_COMPILER_UNUSED(format);
#endif
}
