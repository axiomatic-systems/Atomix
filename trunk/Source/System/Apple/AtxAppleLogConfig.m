/*****************************************************************
|
|      File: AtxAppleLogConfig.c
|
|      Atomix - System Log Configuration Support: Apple Implementation
|
|      (c) 2002-2010 Gilles Boccon-Gibod
|      Author: Edin Hodzic (dino@concisoft.com)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#import <Foundation/Foundation.h>


#import "AtxResults.h"
#import "AtxProperties.h"
#import "AtxLogging.h"

/*----------------------------------------------------------------------
|       ATX_GetSystemLogConfig
+---------------------------------------------------------------------*/
ATX_Result
ATX_GetSystemLogConfig(ATX_String* config)
{
    NSDictionary* env_vars = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"LSEnvironment"];
    NSString* log_config = [env_vars objectForKey:@"ATOMIX_LOG_CONFIG"];
    if (log_config) {
        NSLog(@"ATOMIX_LOG_CONFIG in plist is: %@", log_config);
        ATX_String_Assign(config, (const char*)[log_config UTF8String]);
        return ATX_SUCCESS;
    } else {
        NSLog(@"ATOMIX_LOG_CONFIG not found in 'Info.plist'");
        return ATX_ERROR_NO_SUCH_PROPERTY;
    }
}
