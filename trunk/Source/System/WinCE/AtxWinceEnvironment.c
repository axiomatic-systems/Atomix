/*****************************************************************
|
|      Atomix - Environment variables: Windows CE Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <windows.h>

#include "AtxConfig.h"
#include "AtxUtils.h"
#include "AtxResults.h"

/*----------------------------------------------------------------------
|   ATX_GetEnvironment
+---------------------------------------------------------------------*/
ATX_Result 
ATX_GetEnvironment(const char* name, ATX_String* value)
{
    HKEY       key = NULL; 
    DWORD      type;
    WCHAR*     name_w;
    DWORD      name_length;
    DWORD      value_length;
    ATX_Result result;

    /* default value */
    ATX_String_SetLength(value, 0);

    /* convert name to unicode */
    name_length = ATX_StringLength(name);
    name_w = (WCHAR*)ATX_AllocateMemory(sizeof(WCHAR)*(name_length+1));
    MultiByteToWideChar(CP_UTF8, 0, name, -1, name_w, name_length+1);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, 
                     _T("Software\\Axiomatic\\Atomix\\Environment"), 
                     0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) { 
        if (RegQueryValueEx(key, name_w, 0, &type, (PBYTE)NULL, &value_length ) == ERROR_SUCCESS) { 
            /* convert to UTF-8 */

            WCHAR* value_w = ATX_AllocateMemory(sizeof(WCHAR)*(value_length+1));
            int    value_size = 4*value_length+1;
            ATX_String_Reserve(value, value_size);

            if (RegQueryValueEx(key, name_w, 0, &type, (PBYTE)value_w, &value_length ) == ERROR_SUCCESS) {
                value_size = WideCharToMultiByte(CP_UTF8, 0, value_w, value_length, ATX_String_UseChars(value), value_size, NULL, FALSE);
                ATX_String_SetLength(value, value_size);
            }

            ATX_FreeMemory(value_w);
            result = ATX_SUCCESS;
        }
    }

    ATX_FreeMemory(name_w);

    return result;
}
