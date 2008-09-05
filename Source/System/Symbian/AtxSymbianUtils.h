/*****************************************************************
|
|      File: AtxSymbianUtils.h
|
|      Atomix - Utils: SymbianOS Helpers
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATXSYMBIANUTILS_H_
#define _ATXSYMBIANUTILS_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "e32cmn.h"
#include "AtxTypes.h"
#include "AtxString.h"


#ifdef __cplusplus
extern "C" {
#endif

ATX_Result      ATX_Symbian_C_PathToUtf16(const char* filename, RBuf& symbian_string);
ATX_Result      ATX_Symbian_CStringToSymbian(const char* cstring, RBuf& symbian_string);
ATX_Int32       ATX_Symbian_CompareToCString(const TDesC& symbian_str, const char* cstring);

ATX_Result      ATX_String_Assign_FromSymbianString(ATX_String* str, const TDesC& symbian_str);

#ifdef __cplusplus
}
#endif

#endif /*_ATXSYMBIANUTILS_H_*/
