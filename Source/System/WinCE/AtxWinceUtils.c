/*****************************************************************
|
|   Atomix - Environment Support: WinCE Implementation
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <windows.h>

#include "AtxConfig.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"
#include "AtxUtils.h"

/*----------------------------------------------------------------------
|   ATX_GetEnvironment
+---------------------------------------------------------------------*/
char*
ATX_GetEnvironment(const char* name)
{
    return NULL;
}

/*----------------------------------------------------------------------
|   _tmain
+---------------------------------------------------------------------*/
extern int main(int argc, char** argv);

int
_tmain(int argc, wchar_t** argv, wchar_t** envp)
{
    char** argv_utf8 = (char**)ATX_AllocateMemory(argc*sizeof(char*));
    int i;
    int result;

    /* allocate and convert args */
    for (i=0; i<argc; i++) {
        unsigned int arg_length = wcslen(argv[i]);
        argv_utf8[i] = (char*)ATX_AllocateMemory(4*arg_length+1);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argv_utf8[i], 4*arg_length+1, 0, 0);
    }

    result = main(argc, argv_utf8);


    /* cleanup */
    for (i=0; i<argc; i++) {
        ATX_FreeMemory(argv_utf8[i]);
    }
    ATX_FreeMemory(argv_utf8);

    return result;
}
