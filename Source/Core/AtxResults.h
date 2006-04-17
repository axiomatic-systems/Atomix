/*****************************************************************
|
|      File: AtxResults.h
|
|      Atomix - Result Codes
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
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
        ATX_Debug("@@@ ATX_CHECK failed file %s line %d, result=%d [%s]\n", __FILE__, __LINE__, _x, #_result); \
        return _x;             \
    }                          \
} while(0)
#else
#define ATX_CHECK(_result) do {\
    ATX_Result _x = (_result); \
    if (_x != ATX_SUCCESS) {   \
        return _x;             \
    }                          \
} while(0)
#endif

#if defined(ATX_CONFIG_ENABLE_LOGGING)
#include "AtxLogging.h"
#define ATX_CHECK_LL(_logger, _level, _result) do {                                    \
    ATX_Result _x = (_result);                                                         \
    if (_x != ATX_SUCCESS) {                                                           \
        ATX_LOG_L2(_logger, _level, "ATX_CHECK failed, result=%d [%s]", _x, #_result); \
        return _x;                                                                     \
    }                                                                                  \
} while(0)
#else
#define ATX_CHECK_LL(_logger, _level, _result) ATX_CHECK(_result)
#endif

#define ATX_CHECK_L(_level, _result) ATX_CHECK_LL(_ATX_LocalLogger, _level, _result)
#define ATX_CHECK_SEVERE_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_SEVERE, _result)
#define ATX_CHECK_SEVERE(_result) ATX_CHECK_L(ATX_LOG_LEVEL_SEVERE, _result)
#define ATX_CHECK_WARNING_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_WARNING, _result)
#define ATX_CHECK_WARNING(_result) ATX_CHECK_L(ATX_LOG_LEVEL_WARNING, _result)
#define ATX_CHECK_INFO_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_INFO, _result)
#define ATX_CHECK_INFO(_result) ATX_CHECK_L(ATX_LOG_LEVEL_INFO, _result)
#define ATX_CHECK_FINE_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_FINE, _result)
#define ATX_CHECK_FINE(_result) ATX_CHECK_L(ATX_LOG_LEVEL_FINE, _result)
#define ATX_CHECK_FINER_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_FINER, _result)
#define ATX_CHECK_FINER(_result) ATX_CHECK_L(ATX_LOG_LEVEL_FINER, _result)
#define ATX_CHECK_FINEST_L(_logger, _result) ATX_CHECK_LL(_logger, ATX_LOG_LEVEL_FINEST, _result)
#define ATX_CHECK_FINEST(_result) ATX_CHECK_L(ATX_LOG_LEVEL_FINEST, _result)

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

/* lists error codes */
#define ATX_ERROR_BASE_LISTS            (ATX_ERROR_BASE-800)

/* standard error codes                                  */
/* these are special codes to convey an errno            */
/* the error code is (ATX_ERROR_BASE_ERRNO - errno)      */
/* where errno is the positive integer from errno.h      */
#define ATX_ERROR_BASE_ERRNO            (ATX_ERROR_BASE-2000)
#define ATX_ERROR_ERRNO(e)              (ATX_ERROR_BASE_ERRNO - (e))

#endif /* _ATX_RESULTS_H_ */

