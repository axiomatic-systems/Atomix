/*****************************************************************
|
|      File: AtxAndroidLogConfig.c
|
|      Atomix - System Log Configuration Support: Android Implementation
|
|      (c) 2002-2010 Gilles Boccon-Gibod
|      Author: Edin Hodzic (dino@concisoft.com)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <sys/system_properties.h>
#include <android/log.h>
#include <dlfcn.h> 

#include "AtxResults.h"
#include "AtxProperties.h"
#include "AtxLogging.h"

/*----------------------------------------------------------------------
|   android_property_get
+---------------------------------------------------------------------*/
static int android_property_get(const char* name, char* value) {
    static int (*__real_system_property_get)(const char*, char*) = NULL;
    if (__real_system_property_get == NULL) {
        void* handle = dlopen("libc.so", 0);
        if (!handle) {
             __android_log_print(ANDROID_LOG_DEBUG, "Atomix", "Cannot dlopen libc.so: %s", dlerror());
             return 0;
        }
        __real_system_property_get = (int (*)(const char*, char*))(
                                        dlsym(handle, "__system_property_get"));
        if (!__real_system_property_get) {
             __android_log_print(ANDROID_LOG_DEBUG, "Atomix", "Cannot resolve __system_property_get(): %s", dlerror());
             return 0;
        }
    }
    return (*__real_system_property_get)(name, value);
}

/*----------------------------------------------------------------------
|       ATX_GetSystemLogConfig
+---------------------------------------------------------------------*/
ATX_Result
ATX_GetSystemLogConfig(ATX_String* config)
{
    char android_atx_config[PROP_VALUE_MAX];
    android_atx_config[0] = 0;
    int prop_len = android_property_get("persist.atomix_log_config", 
                                         android_atx_config);
    if (prop_len > 0) {
        ATX_String_AssignN(config, android_atx_config, prop_len);
        __android_log_print(ANDROID_LOG_DEBUG, "Atomix", "Atomix Log Config = %s\n", ATX_String_GetChars(config));
        return ATX_SUCCESS;
    } else {
        __android_log_write(ANDROID_LOG_DEBUG, "Atomix", "Atomix Log Config = DEFAULT\n");
        return ATX_ERROR_NO_SUCH_PROPERTY;
    }
}
