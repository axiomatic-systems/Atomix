/*****************************************************************
|
|      File: AtxUtils.h
|
|      Atomix - Runtime Utilities
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_UTILS_H_
#define _ATX_UTILS_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxTypes.h"

#if defined(ATX_CONFIG_HAVE_STDLIB_H)
#include <stdlib.h>
#endif /* ATX_CONFIG_HAVE_STDLIB_H */

#if defined(ATX_CONFIG_HAVE_STRING_H)
#include <string.h>
#endif /* ATX_CONFIG_HAVE_STRING_H */

#if defined(ATX_CONFIG_HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(DMALLOC)
#include <dmalloc.h>
#endif

/*----------------------------------------------------------------------
|    macros
+---------------------------------------------------------------------*/
#define ATX_ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

/*----------------------------------------------------------------------
|    string macros
+---------------------------------------------------------------------*/
#define ATX_SET_CSTRING(s, n)           \
do {                                    \
    if ((s) != (ATX_CString)(0)) {      \
        ATX_FreeMemory((void*)(s));     \
    }                                   \
    if ((n) != (ATX_CString)(0)) {      \
        (s) = ATX_DuplicateString(n);   \
    }                                   \
} while (0)

#define ATX_DESTROY_CSTRING(s)          \
do {                                    \
    if ((s) != (ATX_CString)(0)) {      \
        ATX_FreeMemory((void*)(s));     \
        (s) = 0;                        \
    }                                   \
} while (0)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----------------------------------------------------------------------
|    conversion utilities
+---------------------------------------------------------------------*/
extern ATX_Result 
ATX_ParseFloat(const char* str, float* result, ATX_Boolean relaxed);

extern ATX_Result 
ATX_ParseInteger(const char* str, long* result, ATX_Boolean relaxed);

/*----------------------------------------------------------------------
|    byte IO
+---------------------------------------------------------------------*/
extern void ATX_BytesFromInt32Be(unsigned char* buffer, ATX_UInt32 value);
extern void ATX_BytesFromInt16Be(unsigned char* buffer, ATX_UInt16 value);
extern ATX_UInt32 ATX_BytesToInt32Be(const unsigned char* buffer);
extern ATX_UInt16 ATX_BytesToInt16Be(const unsigned char* buffer);

extern void ATX_BytesFromInt32Le(unsigned char* buffer, ATX_UInt32 value);
extern void ATX_BytesFromInt16Le(unsigned char* buffer, ATX_UInt16 value);
extern ATX_UInt32 ATX_BytesToInt32Le(const unsigned char* buffer);
extern ATX_UInt16 ATX_BytesToInt16Le(const unsigned char* buffer);

/*----------------------------------------------------------------------
|    C Runtime
+---------------------------------------------------------------------*/
#if defined(ATX_CONFIG_HAVE_MALLOC)
#define ATX_AllocateMemory malloc
#else
extern void* ATX_AllocateMemory(unsigned int);
#endif

#if defined(ATX_CONFIG_HAVE_CALLOC)
#define ATX_AllocateZeroMemory(x) calloc(1,(x))
#else
extern void* ATX_AllocateZeroMemory(unsigned int);
#endif

#if defined(ATX_CONFIG_HAVE_FREE)
#define ATX_FreeMemory free
#else
extern void ATX_FreeMemory(void* pointer);
#endif

#if defined(ATX_CONFIG_HAVE_MEMCPY)
#define ATX_CopyMemory memcpy
#else
extern void ATX_CopyMemory(void* dest, const void* src, ATX_Size size);
#endif

#if defined(ATX_CONFIG_HAVE_MEMMOVE)
#define ATX_MoveMemory memmove
#else
extern void ATX_MoveMemory(void* dest, const void* src, ATX_Size size);
#endif

#if defined(ATX_CONFIG_HAVE_MEMSET)
#define ATX_SetMemory memset
#else
extern void ATX_SetMemory(void* dest, int c, ATX_Size size);
#endif

#if defined(ATX_CONFIG_HAVE_STRCPY)
#define ATX_CopyString(dst, src) strcpy((dst), (src))
#else
extern char* ATX_CopyString(char* dst, const char* src);
#endif

#if defined(ATX_CONFIG_HAVE_STRNCPY)
#define ATX_CopyStringN(dst, src, n) strncpy((dst), (src), n)
#else
extern int ATX_CopyStringN(char* dst, const char* src, unsigned long n);
#endif

#if defined(ATX_CONFIG_HAVE_STRCMP)
#define ATX_StringsEqual(s1, s2) (strcmp((s1), (s2)) == 0)
#else
extern int ATX_StringsEqual(const char* s1, const char* s2);
#endif

#if defined(ATX_CONFIG_HAVE_STRNCMP)
#define ATX_StringsEqualN(s1, s2, n) (strncmp((s1), (s2), (n)) == 0)
#else
extern int ATX_StringsEqualN(const char* s1, const char* s2, unsigned long size);
#endif

#if defined(ATX_CONFIG_HAVE_STRDUP)
#define ATX_DuplicateString(s) ATX_strdup(s)
#else
extern char* ATX_DuplicateString(const char* s);
#endif

#if defined(ATX_CONFIG_HAVE_STRLEN)
#define ATX_StringLength(s) (ATX_Size)strlen(s)
#else
extern unsigned long ATX_StringLength(const char* s);
#endif

#if defined(ATX_CONFIG_HAVE_IS_SPACE)
#define ATX_IsSpace(c) isspace(c)
#else
extern int ATX_IsSpace(int c);
#endif

#if defined(ATX_CONFIG_HAVE_MEMCMP)
#define ATX_MemoryEqual(s1, s2, n) (memcmp((s1), (s2), (n)) == 0) 
#else 
extern int ATX_MemoryEqual(const void* s1, const void* s2, unsigned long n); 
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_UTILS_H_ */
