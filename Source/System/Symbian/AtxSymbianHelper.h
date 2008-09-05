/*****************************************************************
|
|      File: AtxSymbianHelper.c
|
|      Atomix - Helpers: SymbianOS Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef ATXSYMBIANRANDOM_H_
#define ATXSYMBIANRANDOM_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"

/*----------------------------------------------------------------------
|       declarations
+---------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------
|       Random Functions
+---------------------------------------------------------------------*/
unsigned long    ATX_Symbian_GetRandomBytes(unsigned char *buf, unsigned long len);

ATX_Int32       ATX_Heap_Available();

/*----------------------------------------------------------------------
|       File Functions
+---------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*ATXSYMBIANRANDOM_H_*/
