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
        return ATX_SUCCESS;
    } else {
        return ATX_ERROR_NO_SUCH_PROPERTY;
    }
}
