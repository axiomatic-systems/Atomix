/*****************************************************************
|
|      File: AtxLogging.h
|
|      Atomix - Logging Support
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/
/** @file
* Header file for logging
*/

#ifndef _ATX_LOGGING_H_
#define _ATX_LOGGING_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxTime.h"
#include "AtxString.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct {
    const char*   logger_name;
    int           level;
    const char*   message;
    ATX_TimeStamp timestamp;
    const char*   source_file;
    unsigned int  source_line;
} ATX_LogRecord;

typedef struct ATX_Logger ATX_Logger;
typedef struct ATX_LogHandlerInstance ATX_LogHandlerInstance;
typedef struct ATX_LogHandler ATX_LogHandler;
typedef struct ATX_LogHandlerEntry ATX_LogHandlerEntry;

typedef struct {
    void (*Log)(ATX_LogHandler* self, const ATX_LogRecord* record);
    void (*Destroy)(ATX_LogHandler* self);
} ATX_LogHandlerInterface;

struct ATX_LogHandler {
    ATX_LogHandlerInstance*        instance;
    const ATX_LogHandlerInterface* iface;
};

struct ATX_LogHandlerEntry {
    ATX_LogHandler       handler;
    ATX_LogHandlerEntry* next;
};

struct ATX_Logger {
    ATX_String           name;
    int                  level;
    ATX_Boolean          level_is_inherited;
    ATX_Boolean          forward_to_parent;
    ATX_Logger*          parent;
    ATX_LogHandlerEntry* handlers;
};

typedef struct {
    ATX_Logger* logger;
    const char* name;
} ATX_LoggerReference;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define ATX_LOG_LEVEL_SEVERE  600 
#define ATX_LOG_LEVEL_WARNING 500
#define ATX_LOG_LEVEL_INFO    400
#define ATX_LOG_LEVEL_FINE    300
#define ATX_LOG_LEVEL_FINER   200
#define ATX_LOG_LEVEL_FINEST  100 

#define ATX_LOG_LEVEL_OFF     32767
#define ATX_LOG_LEVEL_ALL     0

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
#define ATX_LOG_GET_LOGGER(_logger)                           \
    if ((_logger).logger == NULL) {                           \
        (_logger).logger = ATX_Log_GetLogger((_logger).name); \
    }

#define ATX_LOG(_level, _msg) ATX_LOG_L(_ATX_LocalLogger, (_level), (_msg))
#define ATX_LOG_1(_level, _msg, _arg1) \
    ATX_LOG_L1(_ATX_LocalLogger, (_level), (_msg), (_arg1)) 
#define ATX_LOG_2(_level, _msg, _arg1, _arg2) \
    ATX_LOG_L2(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2))
#define ATX_LOG_3(_level, _msg, _arg1, _arg2, _arg3) \
    ATX_LOG_L3(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3))
#define ATX_LOG_4(_level, _msg, _arg1, _arg2, _arg3, _arg4) \
    ATX_LOG_L4(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4))
#define ATX_LOG_5(_level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5) \
    ATX_LOG_L5(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5))
#define ATX_LOG_6(_level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6) \
    ATX_LOG_L6(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6))
#define ATX_LOG_7(_level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7) \
    ATX_LOG_L7(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7))
#define ATX_LOG_8(_level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8) \
    ATX_LOG_L8(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7), (_arg8))
#define ATX_LOG_9(_level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9) \
    ATX_LOG_L9(_ATX_LocalLogger, (_level), (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7), (_arg8), (_arg9))


#if defined(ATX_CONFIG_ENABLE_LOGGING)
#define ATX_DEFINE_LOGGER(_logger, _name) \
    static ATX_LoggerReference _logger = { NULL, (_name) };

#define ATX_LOG_CHECK_L(_logger, _level, _result) do {                    \
    ATX_Result _x = (_result);                                            \
    if (_x != ATX_SUCCESS) {                                              \
        ATX_LOG_L1(_logger, _level, "ATX_CHECK failed, result=%d", _x); \
        return _x;                                                        \
    }                                                                     \
} while(0)

#define ATX_LOG_LX(_logger, _level, _argsx)                          \
do {                                                                 \
    ATX_LOG_GET_LOGGER((_logger))                                    \
    if ((_logger).logger && (_level) >= (_logger).logger->level) {   \
        ATX_Logger_Log _argsx;                                       \
    }                                                                \
} while(0)
#define ATX_LOG_L(_logger, _level, _msg) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg)) )
#define ATX_LOG_L1(_logger, _level, _msg, _arg1) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1)) )
#define ATX_LOG_L2(_logger, _level, _msg, _arg1, _arg2) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2)) )
#define ATX_LOG_L3(_logger, _level, _msg, _arg1, _arg2, _arg3) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3)) )
#define ATX_LOG_L4(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4)) )
#define ATX_LOG_L5(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5)) )
#define ATX_LOG_L6(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6)) )
#define ATX_LOG_L7(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7)) )
#define ATX_LOG_L8(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7), (_arg8)) )
#define ATX_LOG_L9(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9) \
    ATX_LOG_LX(_logger, _level, ((_logger).logger, (_level), __FILE__, __LINE__, (_msg), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7), (_arg8), (_arg9)) )

#else /* ATX_CONFIG_ENABLE_LOGGING */
#define ATX_DEFINE_LOGGER(_logger, _name)
#define ATX_LOG_L(_logger, _level, _msg)
#define ATX_LOG_L1(_logger, _level, _msg, _arg1)
#define ATX_LOG_L2(_logger, _level, _msg, _arg1, _arg2)
#define ATX_LOG_L3(_logger, _level, _msg, _arg1, _arg2, _arg3)
#define ATX_LOG_L4(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4)
#define ATX_LOG_L5(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5)
#define ATX_LOG_L6(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6)
#define ATX_LOG_L7(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7)
#define ATX_LOG_L8(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8)
#define ATX_LOG_L9(_logger, _level, _msg, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9)

#define ATX_LOG_CHECK_L(_logger, _level, _result) do { \
    ATX_Result _x = (_result);                         \
    if (_x != ATX_SUCCESS) {                           \
        return _x;                                     \
    }                                                  \
} while(0)

#endif /* ATX_CONFIG_ENABLE_LOGGING */

#define ATX_SET_LOCAL_LOGGER(_name) \
    ATX_DEFINE_LOGGER(_ATX_LocalLogger, (_name))

#define ATX_LOG_CHECK(_level, _result) ATX_LOG_CHECK_L(_ATX_LocalLogger, _level, _result)

#define ATX_CHECK_SEVERE_L(_logger, _result) ATX_LOG_CHECK(_logger, ATX_LOG_LEVEL_SEVERE, _result)
#define ATX_CHECK_SEVERE(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_SEVERE, _result)
#define ATX_CHECK_WARNING_L(_logger, _result) ATX_LOG_CHECK_L(_logger, ATX_LOG_LEVEL_WARNING, _result)
#define ATX_CHECK_WARNING(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_WARNING, _result)
#define ATX_CHECK_INFO_L(_logger, _result) ATX_LOG_CHECK_L(_logger, ATX_LOG_LEVEL_INFO, _result)
#define ATX_CHECK_INFO(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_INFO, _result)
#define ATX_CHECK_FINE_L(_logger, _result) ATX_LOG_CHECK_L(_logger, ATX_LOG_LEVEL_FINE, _result)
#define ATX_CHECK_FINE(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_FINE, _result)
#define ATX_CHECK_FINER_L(_logger, _result) ATX_LOG_CHECK_L(_logger, ATX_LOG_LEVEL_FINER, _result)
#define ATX_CHECK_FINER(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_FINER, _result)
#define ATX_CHECK_FINEST_L(_logger, _result) ATX_LOG_CHECK_L(_logger, ATX_LOG_LEVEL_FINEST, _result)
#define ATX_CHECK_FINEST(_result) ATX_LOG_CHECK(ATX_LOG_LEVEL_FINEST, _result)

/* NOTE: the following are machine-generated, do not edit */
#define ATX_LOG_SEVERE_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_SEVERE, (_msg))
#define ATX_LOG_SEVERE(_msg) ATX_LOG_SEVERE_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_SEVERE_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1))
#define ATX_LOG_SEVERE_1(_msg,_arg1) ATX_LOG_SEVERE_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_SEVERE_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2))
#define ATX_LOG_SEVERE_2(_msg,_arg1,_arg2) ATX_LOG_SEVERE_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_SEVERE_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_SEVERE_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_SEVERE_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_SEVERE_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_SEVERE_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_SEVERE_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_SEVERE_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_SEVERE_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_SEVERE_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_SEVERE_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_SEVERE_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_SEVERE_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_SEVERE_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_SEVERE_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_SEVERE_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_SEVERE_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_SEVERE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_SEVERE_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_SEVERE_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_WARNING_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_WARNING, (_msg))
#define ATX_LOG_WARNING(_msg) ATX_LOG_WARNING_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_WARNING_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1))
#define ATX_LOG_WARNING_1(_msg,_arg1) ATX_LOG_WARNING_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_WARNING_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2))
#define ATX_LOG_WARNING_2(_msg,_arg1,_arg2) ATX_LOG_WARNING_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_WARNING_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_WARNING_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_WARNING_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_WARNING_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_WARNING_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_WARNING_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_WARNING_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_WARNING_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_WARNING_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_WARNING_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_WARNING_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_WARNING_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_WARNING_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_WARNING_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_WARNING_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_WARNING_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_WARNING, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_WARNING_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_WARNING_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_INFO_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_INFO, (_msg))
#define ATX_LOG_INFO(_msg) ATX_LOG_INFO_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_INFO_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1))
#define ATX_LOG_INFO_1(_msg,_arg1) ATX_LOG_INFO_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_INFO_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2))
#define ATX_LOG_INFO_2(_msg,_arg1,_arg2) ATX_LOG_INFO_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_INFO_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_INFO_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_INFO_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_INFO_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_INFO_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_INFO_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_INFO_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_INFO_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_INFO_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_INFO_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_INFO_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_INFO_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_INFO_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_INFO_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_INFO_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_INFO_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_INFO, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_INFO_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_INFO_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINE_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_FINE, (_msg))
#define ATX_LOG_FINE(_msg) ATX_LOG_FINE_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_FINE_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1))
#define ATX_LOG_FINE_1(_msg,_arg1) ATX_LOG_FINE_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_FINE_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINE_2(_msg,_arg1,_arg2) ATX_LOG_FINE_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINE_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINE_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_FINE_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINE_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINE_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_FINE_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINE_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINE_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_FINE_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINE_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINE_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_FINE_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINE_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINE_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_FINE_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINE_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_FINE, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINE_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_FINE_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINER_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_FINER, (_msg))
#define ATX_LOG_FINER(_msg) ATX_LOG_FINER_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_FINER_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1))
#define ATX_LOG_FINER_1(_msg,_arg1) ATX_LOG_FINER_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_FINER_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINER_2(_msg,_arg1,_arg2) ATX_LOG_FINER_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINER_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINER_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_FINER_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINER_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINER_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_FINER_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINER_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINER_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_FINER_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINER_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINER_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_FINER_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINER_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINER_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_FINER_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINER_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_FINER, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINER_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_FINER_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINEST_L(_logger, _msg) ATX_LOG_L((_logger), ATX_LOG_LEVEL_FINEST, (_msg))
#define ATX_LOG_FINEST(_msg) ATX_LOG_FINEST_L(_ATX_LocalLogger, (_msg))
#define ATX_LOG_FINEST_L1(_logger, _msg,_arg1) ATX_LOG_L1((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1))
#define ATX_LOG_FINEST_1(_msg,_arg1) ATX_LOG_FINEST_L1(_ATX_LocalLogger, (_msg),(_arg1))
#define ATX_LOG_FINEST_L2(_logger, _msg,_arg1,_arg2) ATX_LOG_L2((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINEST_2(_msg,_arg1,_arg2) ATX_LOG_FINEST_L2(_ATX_LocalLogger, (_msg),(_arg1),(_arg2))
#define ATX_LOG_FINEST_L3(_logger, _msg,_arg1,_arg2,_arg3) ATX_LOG_L3((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINEST_3(_msg,_arg1,_arg2,_arg3) ATX_LOG_FINEST_L3(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3))
#define ATX_LOG_FINEST_L4(_logger, _msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_L4((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINEST_4(_msg,_arg1,_arg2,_arg3,_arg4) ATX_LOG_FINEST_L4(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4))
#define ATX_LOG_FINEST_L5(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_L5((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINEST_5(_msg,_arg1,_arg2,_arg3,_arg4,_arg5) ATX_LOG_FINEST_L5(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5))
#define ATX_LOG_FINEST_L6(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_L6((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINEST_6(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6) ATX_LOG_FINEST_L6(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6))
#define ATX_LOG_FINEST_L7(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_L7((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINEST_7(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7) ATX_LOG_FINEST_L7(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7))
#define ATX_LOG_FINEST_L8(_logger, _msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_L8((_logger), ATX_LOG_LEVEL_FINEST, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))
#define ATX_LOG_FINEST_8(_msg,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ATX_LOG_FINEST_L8(_ATX_LocalLogger, (_msg),(_arg1),(_arg2),(_arg3),(_arg4),(_arg5),(_arg6),(_arg7),(_arg8))

/*----------------------------------------------------------------------
|   prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern int         ATX_Log_GetLogLevel(const char* name);
extern const char* ATX_Log_GetLogLevelName(int level);
extern ATX_Logger* ATX_Log_GetLogger(const char* name);
extern void ATX_Logger_Log(ATX_Logger*  self, 
                           int          level, 
                           const char*  source_file,
                           unsigned int source_line,
                           const char*  msg, 
                                       ...);
extern ATX_Result ATX_Logger_AddHandler(ATX_Logger* self, ATX_LogHandler* handler);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_LOGGING_H_ */
