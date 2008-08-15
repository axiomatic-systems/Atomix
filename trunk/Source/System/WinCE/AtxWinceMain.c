/*****************************************************************
|
|   Atomix - main adapter for WinCE
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <windows.h>

/*----------------------------------------------------------------------
|   imports
+---------------------------------------------------------------------*/
extern int main(int argc, char** argv);

/*----------------------------------------------------------------------
|   _tmain
+---------------------------------------------------------------------*/
int
_tmain(int argc, wchar_t** argv, wchar_t** envp)
{
    char** argv_utf8 = (char**)malloc((1+argc)*sizeof(char*));
    int i;
    int result;

    /* allocate and convert args */
    for (i=0; i<argc; i++) {
        unsigned int arg_length = wcslen(argv[i]);
        argv_utf8[i] = (char*)malloc(4*arg_length+1);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argv_utf8[i], 4*arg_length+1, 0, 0);
    }

    /* terminate the array with a null pointer */
    argv_utf8[argc] = NULL;

    /* call the real main */
    result = main(argc, argv_utf8);

    /* cleanup */
    for (i=0; i<argc; i++) {
        free(argv_utf8[i]);
    }
    free(argv_utf8);

    return result;
}
