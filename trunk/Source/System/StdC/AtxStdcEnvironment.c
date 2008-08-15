/*****************************************************************
|
|      Atomix - Environment variables: StdC Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdlib.h>

#include "AtxConfig.h"
#include "AtxUtils.h"
#include "AtxResults.h"

/*----------------------------------------------------------------------
|   ATX_GetEnvironment
+---------------------------------------------------------------------*/
ATX_Result 
ATX_GetEnvironment(const char* name, ATX_String* value)
{
    char* env;

    /* default value */
    ATX_String_SetLength(value, 0);

#if defined(ATX_CONFIG_HAVE_GETENV)
    env = getenv(name);
    if (env) {
        ATX_String_Assign(value, env);
        return ATX_SUCCESS;
    } else {
        return ATX_ERROR_NO_SUCH_ITEM;
    }
#elif defined(ATX_CONFIG_HAVE_DUPENV_S)
    if (dupenv_s(&env, NULL, name) != 0) {
        return ATX_FAILURE;
    } else if (env != NULL) {
        ATX_String_Assign(value, env);
        free(env);
        return ATX_SUCCESS;
    } else {
        return ATX_ERROR_NO_SUCH_ITEM;
    }
#else
#error "no getenv or getenv_s available on this platform"
#endif
}
