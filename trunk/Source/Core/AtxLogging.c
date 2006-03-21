/*****************************************************************
|
|      File: AtxLogging.c
|
|      Atomix - Logging Support
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/
/** @file
* Implementation file for logging
*/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <stdarg.h>
#include "AtxConfig.h"
#include "AtxDebug.h"
#include "AtxTypes.h"
#include "AtxUtils.h"
#include "AtxResults.h"
#include "AtxLogging.h"
#include "AtxSystem.h"
#include "AtxString.h"
#include "AtxList.h"
#include "AtxDataBuffer.h"
#include "AtxFile.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_String key;
    ATX_String value;
} ATX_LogConfigEntry;

typedef struct {
    ATX_List*   config;
    ATX_List*   loggers;
    ATX_Logger* root;
    ATX_Boolean initialized;
} ATX_LogManager;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define ATX_LOG_HEAP_BUFFER_INCREMENT 4096
#define ATX_LOG_STACK_BUFFER_MAX_SIZE 512
#define ATX_LOG_HEAP_BUFFER_MAX_SIZE  65536

#define ATX_LOG_CONFIG_ENV "ATOMIX_LOG_CONFIG"
/*#define ATX_LOG_DEFAULT_CONFIG_SOURCE "file:atomix-logging.properties" */
#define ATX_LOG_DEFAULT_CONFIG_SOURCE "plist:atomix.level=FINE;atomix.test.handlers=FooHandler,FileHandler"

/*----------------------------------------------------------------------
|   globals
+---------------------------------------------------------------------*/
static ATX_LogManager LogManager;

/*----------------------------------------------------------------------
|   forward references
+---------------------------------------------------------------------*/
static ATX_Logger* ATX_Logger_Create(const char* name);
static ATX_Result ATX_Logger_Destroy(ATX_Logger* self);

/*----------------------------------------------------------------------
|   GetStrippedString
+---------------------------------------------------------------------*/
static ATX_String
GetStrippedString(const char* str, ATX_Size max_length)
{
    unsigned int str_length = 0;
    const char*  start = str;

    while (((ATX_Size)(str-start)<max_length) && (*str == ' ' || *str == '\t')) ++str;
    while (str[str_length] != ' '  && 
           str[str_length] != '\t' && 
           (ATX_Size)(str+str_length-start)<max_length) {
        ++str_length;
    }
    return ATX_String_CreateFromSubString(str, 0, str_length);
}

/*----------------------------------------------------------------------
|   ATX_FileHandler_Log
+---------------------------------------------------------------------*/
static void
ATX_FileHandler_Log(ATX_LogHandler* self, const ATX_LogRecord* record)
{
    ATX_COMPILER_UNUSED(self);
    ATX_Debug("ATX_DefaultHandler_Log: logger=%s, level=%d, file=%s, line=%d, message=%s\n", 
        record->logger_name, 
        record->level, 
        record->source_file,
        record->source_line,
        record->message);
}

/*----------------------------------------------------------------------
|   ATX_FileHandler_Destroy
+---------------------------------------------------------------------*/
static void
ATX_FileHandler_Destroy(ATX_LogHandler* self)
{
    ATX_COMPILER_UNUSED(self);
}

/*----------------------------------------------------------------------
|   ATX_FileHandler_Interface
+---------------------------------------------------------------------*/
static const ATX_LogHandlerInterface 
ATX_FileHandler_Interface = {
    ATX_FileHandler_Log,
    ATX_FileHandler_Destroy
};

/*----------------------------------------------------------------------
|   ATX_FileHandler_Create
+---------------------------------------------------------------------*/
static ATX_Result
ATX_FileHandler_Create(ATX_LogHandler* handler)
{
    handler->instance = NULL;
    handler->iface    = &ATX_FileHandler_Interface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_LogHandler_Create
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogHandler_Create(const char* name, ATX_LogHandler* handler)
{
    if (ATX_StringsEqual(name, "FileHandler")) {
        return ATX_FileHandler_Create(handler);
    }

    return ATX_ERROR_NO_SUCH_CLASS;
}

/*----------------------------------------------------------------------
|   ATX_Log_GetLogLevel
+---------------------------------------------------------------------*/
int 
ATX_Log_GetLogLevel(const char* name)
{
    if (       ATX_StringsEqual(name, "SEVERE")) {
        return ATX_LOG_LEVEL_SEVERE;
    } else if (ATX_StringsEqual(name, "WARNING")) {
        return ATX_LOG_LEVEL_WARNING;
    } else if (ATX_StringsEqual(name, "INFO")) {
        return ATX_LOG_LEVEL_INFO;
    } else if (ATX_StringsEqual(name, "FINE")) {
        return ATX_LOG_LEVEL_FINE;
    } else if (ATX_StringsEqual(name, "FINER")) {
        return ATX_LOG_LEVEL_FINER;
    } else if (ATX_StringsEqual(name, "FINEST")) {
        return ATX_LOG_LEVEL_FINEST;
    } else if (ATX_StringsEqual(name, "NONE")) {
        return ATX_LOG_LEVEL_NONE;
    } else {
        return -1;
    }
}

/*----------------------------------------------------------------------
|   ATX_LogManager_ClearConfig
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogManager_ClearConfig() 
{
    ATX_ListItem* item = ATX_List_GetFirstItem(LogManager.config);
    while (item) {
        ATX_LogConfigEntry* entry = (ATX_LogConfigEntry*)ATX_ListItem_GetData(item);
        ATX_String_Destruct(&entry->key);
        ATX_String_Destruct(&entry->value);
        ATX_FreeMemory((void*)entry);
        item = ATX_ListItem_GetNext(item);
    }
    ATX_List_Clear(LogManager.config);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_ParseConfig
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogManager_ParseConfig(const char* config,
                           ATX_Size    config_size) 
{
    const char* cursor = config;
    const char* line = config;
    const char* separator = NULL;

    /* parse all entries */
    while (cursor <= config+config_size) {
        /* separators are newlines, ';' or end of buffer */
        if (*cursor == '\n' || 
            *cursor == '\r' || 
            *cursor == ';' || 
            cursor == config+config_size) {
            /* newline or end of buffer */
            if (separator && line[0] != '#') {
                /* we have a property */
                ATX_LogConfigEntry* entry = ATX_AllocateMemory(sizeof(ATX_LogConfigEntry));
                if (entry == NULL) return ATX_ERROR_OUT_OF_MEMORY;

                entry->key   = GetStrippedString(line, (ATX_Size)(separator-line));
                entry->value = GetStrippedString(separator+1, (ATX_Size)(cursor-separator-1));
                ATX_List_AddData(LogManager.config, (void*)entry);
                ATX_Debug("config:: %s=%s\n", 
                          ATX_String_GetChars(&entry->key),
                          ATX_String_GetChars(&entry->value));
            }
            line = cursor+1;
            separator = NULL;
        } else if (*cursor == '=' && separator == NULL) {
            separator = cursor;
        }
        cursor++;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_ParseConfigFile
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogManager_ParseConfigFile(const char* filename) 
{
    ATX_Result result;

    /* load the file */
    ATX_DataBuffer* buffer = NULL;
    ATX_CHECK(ATX_LoadFile(filename, &buffer));

    /* parse the config */
    result = ATX_LogManager_ParseConfig((const char*)ATX_DataBuffer_GetData(buffer),
                                        ATX_DataBuffer_GetDataSize(buffer));

    /* destroy the buffer */
    ATX_DataBuffer_Destroy(buffer);

    return result;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_ParseConfigSource
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogManager_ParseConfigSource(ATX_String* source) 
{
    if (ATX_String_StartsWith(source, "file:")) {
        /* file source */
        ATX_LogManager_ParseConfigFile(ATX_String_GetChars(source)+5);
    } else if (ATX_String_StartsWith(source, "plist:")) {
        ATX_LogManager_ParseConfig(ATX_String_GetChars(source)+6,
                                   ATX_String_GetLength(source)-6);
    } else {
        return ATX_ERROR_INVALID_SYNTAX;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_Terminate
+---------------------------------------------------------------------*/
static void
ATX_LogManager_Terminate(void)
{
    /* destroy everything we've created */
    ATX_LogManager_ClearConfig();
    ATX_List_Destroy(LogManager.config);

    {
        ATX_ListItem* item = ATX_List_GetFirstItem(LogManager.loggers);
        ATX_Logger* logger = (ATX_Logger*)ATX_ListItem_GetData(item);
        ATX_Logger_Destroy(logger);
        item = ATX_ListItem_GetNext(item);
    }
    ATX_List_Destroy(LogManager.loggers);
    ATX_Logger_Destroy(LogManager.root);
}

/*----------------------------------------------------------------------
|   ATX_LogManager_GetConfigValue
+---------------------------------------------------------------------*/
static ATX_String*
ATX_LogManager_GetConfigValue(const char* prefix, const char* suffix)
{
    ATX_ListItem* item = ATX_List_GetFirstItem(LogManager.config);
    ATX_Size      prefix_length = prefix?ATX_StringLength(prefix):0;
    ATX_Size      suffix_length = suffix?ATX_StringLength(suffix):0;
    ATX_Size      key_length    = prefix_length+suffix_length;
    while (item) {
        ATX_LogConfigEntry* entry = (ATX_LogConfigEntry*)ATX_ListItem_GetData(item);
        if (ATX_String_GetLength(&entry->key) == key_length &&
            (prefix == NULL || ATX_String_StartsWith(&entry->key, prefix)) &&
            (suffix == NULL || ATX_String_EndsWith(&entry->key, suffix  )) ) {
            return &entry->value;
        }
        item = ATX_ListItem_GetNext(item);
    }

    /* not found */
    return NULL;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_HaveLoggerConfig
+---------------------------------------------------------------------*/
static ATX_Boolean
ATX_LogManager_HaveLoggerConfig(const char* name)
{
    ATX_ListItem* item = ATX_List_GetFirstItem(LogManager.config);
    ATX_Size      name_length = ATX_StringLength(name);
    while (item) {
        ATX_LogConfigEntry* entry = (ATX_LogConfigEntry*)ATX_ListItem_GetData(item);
        if (ATX_String_StartsWith(&entry->key, name)) {
            const char* suffix = ATX_String_GetChars(&entry->key)+name_length;
            if (ATX_StringsEqual(suffix, ".level") ||
                ATX_StringsEqual(suffix, ".handlers") ||
                ATX_StringsEqual(suffix, ".forward")) {
                return ATX_TRUE;
            }
        }
        item = ATX_ListItem_GetNext(item);
    }

    /* no config found */
    return ATX_FALSE;

}

/*----------------------------------------------------------------------
|   ATX_LogManager_ConfigureLogger
+---------------------------------------------------------------------*/
static ATX_Result
ATX_LogManager_ConfigureLogger(ATX_Logger* logger)
{
    ATX_Debug("ATX_LogManager_ConfigureLogger:: logger=%s\n", 
              ATX_String_GetChars(&logger->name));

    /* configure the level */
    {
        ATX_String* level_value = ATX_LogManager_GetConfigValue(
            ATX_String_GetChars(&logger->name),".level");
        if (level_value) {
            long value;
            /* try a symbolic name */
            value = ATX_Log_GetLogLevel(ATX_String_GetChars(level_value));
            if (value < 0) {
                /* try a numeric value */
                if (ATX_FAILED(ATX_String_ToInteger(level_value, &value, ATX_FALSE))) {
                    value = -1;
                }
            }
            if (value >= 0) {
                logger->level = value;
                logger->level_is_inherited = ATX_FALSE;
            }
        }
    }

    /* configure the handlers */
    {
        ATX_String* handlers = ATX_LogManager_GetConfigValue(
            ATX_String_GetChars(&logger->name),".handlers");
        if (handlers) {
            const char*    handlers_list = ATX_String_GetChars(handlers);
            const char*    cursor = handlers_list;
            const char*    name_start = handlers_list;
            ATX_String     handler_name = ATX_EMPTY_STRING;
            ATX_LogHandler handler;
            for (;;) {
                if (*cursor == '\0' || *cursor == ',') {
                    if (cursor != name_start) {
                        ATX_String_AssignN(&handler_name, name_start, (ATX_Size)(cursor-name_start));
                        ATX_String_TrimWhitespace(&handler_name);
                        
                        /* create a handler */
                        if (ATX_SUCCEEDED(
                            ATX_LogHandler_Create(ATX_String_GetChars(&handler_name),
                                                  &handler))) {
                            ATX_Logger_AddHandler(logger, &handler);
                        }

                    }
                    if (*cursor == '\0') break;
                    name_start = cursor+1;
                }
                ++cursor;
            }
            ATX_String_Destruct(&handler_name);
        }
    }

    /* configure the forwarding */
    {
        ATX_String* forward = ATX_LogManager_GetConfigValue(
            ATX_String_GetChars(&logger->name),".forward");
        if (forward) {
            if (ATX_String_Compare(forward, "false", ATX_TRUE) == 0||
                ATX_String_Compare(forward, "0", ATX_FALSE) == 0) {
                logger->forward_to_parent = ATX_FALSE;
            }
        }
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_LogManager_Init
+---------------------------------------------------------------------*/
static void
ATX_LogManager_Init(void) 
{
    char* config_sources;

    /* register a function to be called when the program exits */
    ATX_AtExit(ATX_LogManager_Terminate);

    /* create a logger list */
    ATX_List_Create(&LogManager.loggers);

    /* create a config */
    ATX_List_Create(&LogManager.config);

    /* decide what the configuration sources are */
    config_sources = ATX_GetEnvironment(ATX_LOG_CONFIG_ENV);
    if (config_sources == NULL) config_sources = ATX_LOG_DEFAULT_CONFIG_SOURCE;

    /* load all configs */
    {
        ATX_String config_source = ATX_EMPTY_STRING;
        const char* cursor = config_sources; 
        const char* source = config_sources;
        for (;;) {
            if (*cursor == '\0' || *cursor == '|') {
                if (cursor != source) {
                    ATX_String_AssignN(&config_source, source, (ATX_Size)(cursor-source));
                    ATX_String_TrimWhitespace(&config_source);
                    ATX_LogManager_ParseConfigSource(&config_source);
                }
                if (*cursor == '\0') break;
            }
            cursor++;
        }
        ATX_String_Destruct(&config_source);
    }

    /* create the root logger */
    LogManager.root = ATX_Logger_Create("");
    ATX_LogManager_ConfigureLogger(LogManager.root);

    /* we are now initialized */
    LogManager.initialized = ATX_TRUE;
}

/*----------------------------------------------------------------------
|   ATX_Logger_Create
+---------------------------------------------------------------------*/
static ATX_Logger*
ATX_Logger_Create(const char* name)
{
    /* create a new logger */
    ATX_Logger* logger = 
        (ATX_Logger*)ATX_AllocateZeroMemory(sizeof(ATX_Logger));
    if (logger == NULL) return NULL;

    /* setup the logger */
    logger->level              = ATX_LOG_LEVEL_NONE;
    logger->level_is_inherited = ATX_TRUE;
    logger->name               = ATX_String_Create(name);
    logger->forward_to_parent  = ATX_FALSE;
    logger->parent             = NULL;

    return logger;
}

/*----------------------------------------------------------------------
|   ATX_Logger_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
ATX_Logger_Destroy(ATX_Logger* self)
{
    /* destroy all handlers */
    ATX_LogHandlerEntry* entry = self->handlers;
    while (entry) {
        entry->handler.iface->Destroy(&entry->handler);
        entry = entry->next;
    }
    
    /* destruct other members */
    ATX_String_Destruct(&self->name);

    /* free the object memory */
    ATX_FreeMemory((void*)self);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Logger_Log
+---------------------------------------------------------------------*/
void
ATX_Logger_Log(ATX_Logger*  self, 
               int          level, 
               const char*  source_file,
               unsigned int source_line,
               const char*  msg, 
                            ...)
{
    char     buffer[ATX_LOG_STACK_BUFFER_MAX_SIZE];
    ATX_Size buffer_size = sizeof(buffer);
    char*    message = buffer;
    int      result;
    va_list  args;

    va_start(args, msg);

    for(;;) {
        /* try to format the message (it might not fit) */
        result = ATX_FormatStringVN(message, buffer_size-1, msg, args);
        message[buffer_size-1] = 0; /* force a NULL termination */
        if (result >= 0) {
            /* the message is formatted, publish it to the handlers */
            ATX_LogHandlerEntry* entry = self->handlers;
            ATX_LogRecord        record;
            
            /* setup the log record */
            record.logger_name = ATX_String_GetChars(&self->name),
            record.level       = level;
            record.message     = message;
            record.source_file = source_file;
            record.source_line = source_line;
            ATX_System_GetCurrentTimeStamp(&record.timestamp);

            /* call all handlers */
            while (entry) {
                entry->handler.iface->Log(&entry->handler, &record);
                entry = entry->next;
            }
            break;
        }

        /* the buffer was too small, try something bigger */
        buffer_size = (buffer_size+ATX_LOG_HEAP_BUFFER_INCREMENT)*2;
        if (buffer_size > ATX_LOG_HEAP_BUFFER_MAX_SIZE) break;
        if (message != buffer) ATX_FreeMemory((void*)message);
        message = ATX_AllocateMemory(buffer_size);
        if (message == NULL) return;
    }

    /* free anything we may have allocated */
    if (message != buffer) ATX_FreeMemory((void*)message);

    va_end(args);
}

/*----------------------------------------------------------------------
|   ATX_Logger_AddHandler
+---------------------------------------------------------------------*/
ATX_Result
ATX_Logger_AddHandler(ATX_Logger* self, ATX_LogHandler* handler)
{
    ATX_LogHandlerEntry* entry;

    /* check parameters */
    if (handler == NULL) return ATX_ERROR_INVALID_PARAMETERS;

    /* allocate a new entry */
    entry = (ATX_LogHandlerEntry*)ATX_AllocateMemory(sizeof(ATX_LogHandlerEntry));
    if (entry == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* setup the entry */
    entry->handler = *handler;
    
    /* attach the new entry at the beginning of the list */
    entry->next = self->handlers;
    self->handlers = entry;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Logger_SetParent
+---------------------------------------------------------------------*/
ATX_Result
ATX_Logger_SetParent(ATX_Logger* self, ATX_Logger* parent)
{
    ATX_Logger* logger = self;

    /* set our new parent */
    self->parent = parent;

    /* find the first ancestor with its own log level */
    if (logger->level_is_inherited && logger->parent) {
        logger = logger->parent;
    }
    if (logger != self) self->level = logger->level;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_Log_FindLogger
+---------------------------------------------------------------------*/
ATX_Logger*
ATX_Log_FindLogger(const char* name)
{
    ATX_ListItem* item = ATX_List_GetFirstItem(LogManager.loggers);
    while (item) {
        ATX_Logger* logger = (ATX_Logger*)ATX_ListItem_GetData(item);
        if (ATX_StringsEqual(ATX_String_GetChars(&logger->name), name)) {
            return logger;
        }
        item = ATX_ListItem_GetNext(item);
    }

    return NULL;
}

/*----------------------------------------------------------------------
|   ATX_Log_GetLogger
+---------------------------------------------------------------------*/
ATX_Logger*
ATX_Log_GetLogger(const char* name)
{
    ATX_Logger* logger;

    /* check that the manager is initialized */
    if (!LogManager.initialized) {
        /* init the manager */
        ATX_LogManager_Init();
        ATX_ASSERT(LogManager.initialized);
    }

    /* check if this logger is already configured */
    logger = ATX_Log_FindLogger(name);
    if (logger) return logger;

    /* create a new logger */
    logger = ATX_Logger_Create(name);
    if (logger == NULL) return NULL;

    /* configure the logger */
    ATX_LogManager_ConfigureLogger(logger);

    /* find which parent to attach to */
    {
        ATX_Logger* parent = LogManager.root;
        ATX_String  parent_name = ATX_String_Create(name);
        for (;;) {
            ATX_Logger* candidate_parent;

            /* find the last dot */
            int dot = ATX_String_ReverseFindChar(&parent_name, '.');
            if (dot < 0) break;
            ATX_String_SetLength(&parent_name, dot);
            
            /* see if the parent exists */
            candidate_parent = ATX_Log_FindLogger(ATX_String_GetChars(&parent_name));
            if (candidate_parent) {
                parent = candidate_parent;
                break;
            }

            /* this parent name does not exist, see if we need to create it */
            if (ATX_LogManager_HaveLoggerConfig(ATX_String_GetChars(&parent_name))) {
                parent = ATX_Log_GetLogger(ATX_String_GetChars(&parent_name));
                break;
            }
        }
        ATX_String_Destruct(&parent_name);

        /* attach to the parent */
        ATX_Logger_SetParent(logger, parent);
    }

    /* add this logger to the list */
    ATX_List_AddData(LogManager.loggers, (void*)logger);

    return logger;
}