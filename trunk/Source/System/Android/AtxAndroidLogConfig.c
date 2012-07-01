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

#include "AtxResults.h"
#include "AtxProperties.h"
#include "AtxLogging.h"

/*----------------------------------------------------------------------
|       ATX_GetSystemLogConfig
+---------------------------------------------------------------------*/
ATX_Result
ATX_GetSystemLogConfig(ATX_String* config)
{
    char android_atx_config[PROP_VALUE_MAX];
    android_atx_config[0] = 0;
    int prop_len = __system_property_get("persist.atomix_log_config", 
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
