/*****************************************************************
|
|   Atomix - Result Codes
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * ATX Result and Error codes 
 */

#ifndef _ATX_RESULTS_H_
#define _ATX_RESULTS_H_

/*----------------------------------------------------------------------
|    macros      
+---------------------------------------------------------------------*/
#if defined(ATX_DEBUG)
#include "AtxDebug.h"
#define ATX_CHECK(_result) do {\
    ATX_Result _x = (_result); \
    if (_x != ATX_SUCCESS) {   \
        ATX_Debug("%s(%d): @@@ ATX_CHECK failed, result=%d [%s]\n", __FILE__, __LINE__, _x, #_result); \
        return _x;             \
    }                          \
} while(0)
#define ATX_CHECK_LABEL(_result, _label) do {\
    ATX_Result _x = (_result);               \
    if (_x != ATX_SUCCESS) {                 \
        ATX_Debug("%s(%d): @@@ ATX_CHECK failed, result=%d [%s]\n", __FILE__, __LINE__, _x, #_result); \
        goto _label;                         \
    }                                        \
} while(0)
#else
#define ATX_CHECK(_result) do {\
    ATX_Result _x = (_result); \
    if (_x != ATX_SUCCESS) {   \
        return _x;             \
    }                          \
} while(0)
#define ATX_CHECK_LABEL(_result, _label) do {\
    ATX_Result _x = (_result);               \
    if (_x != ATX_SUCCESS) {                 \
        goto _label;                         \
    }                                        \
} while(0)
#endif

#define ATX_FAILED(result)              ((result) != ATX_SUCCESS)
#define ATX_SUCCEEDED(result)           ((result) == ATX_SUCCESS)

/*----------------------------------------------------------------------
|    result codes      
+---------------------------------------------------------------------*/
/** Result indicating that the operation or call succeeded */
#define ATX_SUCCESS                     0

/** Result indicating an unspecififed failure condition */
#define ATX_FAILURE                     (-1)

/* general error codes */
#ifndef ATX_ERROR_BASE               
#define ATX_ERROR_BASE                  (-10000)
#endif

#define ATX_ERROR_BASE_GENERAL          (ATX_ERROR_BASE-0)
#define ATX_ERROR_OUT_OF_MEMORY         (ATX_ERROR_BASE_GENERAL - 0)
#define ATX_ERROR_OUT_OF_RESOURCES      (ATX_ERROR_BASE_GENERAL - 1)
#define ATX_ERROR_INTERNAL              (ATX_ERROR_BASE_GENERAL - 2)
#define ATX_ERROR_INVALID_PARAMETERS    (ATX_ERROR_BASE_GENERAL - 3)
#define ATX_ERROR_INVALID_STATE         (ATX_ERROR_BASE_GENERAL - 4)
#define ATX_ERROR_NOT_IMPLEMENTED       (ATX_ERROR_BASE_GENERAL - 5)
#define ATX_ERROR_OUT_OF_RANGE          (ATX_ERROR_BASE_GENERAL - 6)
#define ATX_ERROR_ACCESS_DENIED         (ATX_ERROR_BASE_GENERAL - 7)
#define ATX_ERROR_INVALID_SYNTAX        (ATX_ERROR_BASE_GENERAL - 8)
#define ATX_ERROR_NOT_SUPPORTED         (ATX_ERROR_BASE_GENERAL -  9)
#define ATX_ERROR_INVALID_FORMAT        (ATX_ERROR_BASE_GENERAL - 10)
#define ATX_ERROR_NOT_ENOUGH_SPACE      (ATX_ERROR_BASE_GENERAL - 11)
#define ATX_ERROR_NO_SUCH_ITEM          (ATX_ERROR_BASE_GENERAL - 12)
#define ATX_ERROR_OVERFLOW              (ATX_ERROR_BASE_GENERAL - 13)

/* device and i/o errors */
#define ATX_ERROR_BASE_DEVICE           (ATX_ERROR_BASE-100)
#define ATX_ERROR_DEVICE_BUSY           (ATX_ERROR_BASE_DEVICE - 0)
#define ATX_ERROR_NO_SUCH_DEVICE        (ATX_ERROR_BASE_DEVICE - 1)
#define ATX_ERROR_OPEN_FAILED           (ATX_ERROR_BASE_DEVICE - 2)
#define ATX_ERROR_NO_MEDIUM             (ATX_ERROR_BASE_DEVICE - 3)

/* object model error codes */
#define ATX_ERROR_BASE_INTERFACES       (ATX_ERROR_BASE-200)

/* properties error codes */
#define ATX_ERROR_BASE_PROPERTIES       (ATX_ERROR_BASE-300)

/* iterator error codes */
#define ATX_ERROR_BASE_ITERATOR         (ATX_ERROR_BASE-400)

/* byte stream error codes */
#define ATX_ERROR_BASE_BYTE_STREAM      (ATX_ERROR_BASE-500)

/* socket error codes */
#define ATX_ERROR_BASE_SOCKETS          (ATX_ERROR_BASE-600)

/* file error codes */
#define ATX_ERROR_BASE_FILE             (ATX_ERROR_BASE-700)

/* standard error codes                                  */
/* these are special codes to convey an errno            */
/* the error code is (ATX_ERROR_BASE_ERRNO - errno)      */
/* where errno is the positive integer from errno.h      */
#define ATX_ERROR_BASE_ERRNO            (ATX_ERROR_BASE-2000)
#define ATX_ERROR_ERRNO(e)              (ATX_ERROR_BASE_ERRNO - (e))

#endif /* _ATX_RESULTS_H_ */

