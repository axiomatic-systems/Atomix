/*****************************************************************
|
|      File: AtxSymbianUtils.cpp
|
|      Atomix - Helpers: SymbianOS Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "e32cmn.h"

#include "AtxSymbianUtils.h"
#include "AtxResults.h"
#include "AtxTypes.h"
#include "AtxString.h"
#include "AtxUtils.h"


/*----------------------------------------------------------------------
|       ATX_Symbian_C_PathToUtf16
+---------------------------------------------------------------------*/
ATX_Result
ATX_Symbian_C_PathToUtf16(const char* filename, RBuf& symbian_string)
{
    TUint len = strlen(filename);
    TText* text_name = new TText [len];
    for (unsigned int i=0; i < len; ++i) {
        if (filename[i] == '/')
            text_name[i] = '\\';
        else
            text_name[i] = filename[i];
    }

    symbian_string.Assign(text_name, len, len); // RBuf takes ownership of text_name

    return ATX_SUCCESS;
}


/*----------------------------------------------------------------------
|       ATX_Symbian_CompareToCString
+---------------------------------------------------------------------*/
ATX_Int32
ATX_Symbian_CompareToCString(const TDesC& symbian_str, const char* cstring)
{
    const int sym_len = symbian_str.Length();
    const int c_len = ATX_StringLength(cstring);
    if (sym_len != c_len) {
        return sym_len - c_len;
    }

    for (int i = 0; i < sym_len; ++i) {
        if (symbian_str[i] > cstring[i]) {
            return 1;
        }
        else if (symbian_str[i] < cstring[i]) {
            return -1;
        }
    }

    return 0;
}


/*----------------------------------------------------------------------
|       ATX_Symbian_CStringToSymbian
+---------------------------------------------------------------------*/
ATX_Result
ATX_Symbian_CStringToSymbian(const char* cstring, RBuf& symb_str)
{
    int len = ATX_StringLength(cstring);
    TText* mystr = new TText [len];

    for (int i = 0; i < len; ++i) {
        mystr[i] = cstring[i];
    }

    symb_str.Close();
    symb_str.Assign(mystr, len, len); // RBuf takes ownership of text_name

    return ATX_SUCCESS;
}


/*----------------------------------------------------------------------
|       ATX_String_Assign_FromSymbianString
+---------------------------------------------------------------------*/
ATX_Result
ATX_String_Assign_FromSymbianString(ATX_String* str, const TDesC& symbian_str)
{
    /* Clear the string */
    ATX_String_Assign(str, "");

    ATX_String_Reserve(str, symbian_str.Length() + 1);
    char* buf = ATX_String_UseChars(str);

    for (int i = 0; i < symbian_str.Length(); ++i) {
        TUint16 val = symbian_str[i];
        if (val & 0xff00) {
            /* String contents are actually 16bit, we can't convert that. */
            return ATX_FAILURE;
        }
        buf[i] = (TUint8) val;
    }

    buf[symbian_str.Length()] = '\0';
    return ATX_SUCCESS;
}


