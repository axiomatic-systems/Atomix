/*****************************************************************
|
|      File: AtxErrors.h
|
|      Atomix - Error Constants
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * ATX Result and Error codes 
 */

#ifndef _ATX_ERRORS_H_
#define _ATX_ERRORS_H_

/*----------------------------------------------------------------------
|    error codes      
+---------------------------------------------------------------------*/
/** Result indicating that the operation or call succeeded */
#define ATX_SUCCESS                     0
/** Result indicating an unspecififed failure condition */
#define ATX_FAILURE                     (-1)

#define ATX_FAILED(result)              ((result) != ATX_SUCCESS)
#define ATX_SUCCEEDED(result)           ((result) == ATX_SUCCESS)

#define ATX_CHECK(_x) do {          \
    ATX_Result _result = (_x);      \
    if (_result != ATX_SUCCESS) {   \
        return _result;             \
    }                               \
} while(0)

/* general error codes */
#define ATX_ERROR_BASE_GENERAL          (-100)
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

/* device and i/o errors */
#define ATX_ERROR_BASE_DEVICE           (-200)
#define ATX_ERROR_DEVICE_BUSY           (ATX_ERROR_BASE_DEVICE - 0)
#define ATX_ERROR_NO_SUCH_DEVICE        (ATX_ERROR_BASE_DEVICE - 1)
#define ATX_ERROR_OPEN_FAILED           (ATX_ERROR_BASE_DEVICE - 2)
#define ATX_ERROR_NO_MEDIUM             (ATX_ERROR_BASE_DEVICE - 3)

/* object model error codes */
#define ATX_ERROR_BASE_INTERFACES       (-300)

/* properties error codes */
#define ATX_ERROR_BASE_PROPERTIES       (-400)

/* iterator error codes */
#define ATX_ERROR_BASE_ITERATOR         (-500)

/* byte stream error codes */
#define ATX_ERROR_BASE_BYTE_STREAM      (-600)

/* socket error codes */
#define ATX_ERROR_BASE_SOCKETS          (-700)

/* file error codes */
#define ATX_ERROR_BASE_FILE             (-800)

/* lists error codes */
#define ATX_ERROR_BASE_LISTS            (-900)

/* standard error codes                                  */
/* these are special codes to convey an errno            */
/* the error code is (ATX_ERROR_BASE_ERRNO - errno)      */
/* where errno is the positive integer from errno.h      */
#define ATX_ERROR_BASE_ERRNO            (-10000)
#define ATX_ERROR_ERRNO(e)              (ATX_ERROR_BASE_ERRNO - (e))

#endif /* _ATX_ERRORS_H_ */

