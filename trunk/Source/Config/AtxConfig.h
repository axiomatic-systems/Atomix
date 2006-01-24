/*****************************************************************
|
|      File: AtxConfig.h
|
|      Atomix - Platform Configuration
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_CONFIG_H_
#define _ATX_CONFIG_H_

/*----------------------------------------------------------------------
|    defaults
+---------------------------------------------------------------------*/
/* assume little endian by default as this is the most common  */
#define ATX_CONFIG_CPU_BYTE_ORDER ATX_CPU_LITTLE_ENDIAN

#define ATX_CONFIG_HAVE_STD_C
#define ATX_CONFIG_HAVE_STDLIB_H
#define ATX_CONFIG_HAVE_STRING_H
#define ATX_CONFIG_HAVE_CTYPE_H
#define ATX_CONFIG_HAVE_MATH_H
#define ATX_CONFIG_HAVE_ASSERT_H

/*----------------------------------------------------------------------
|    CPU byte order
+---------------------------------------------------------------------*/
#define ATX_CPU_BIG_ENDIAN    1
#define ATX_CPU_LITTLE_ENDIAN 2

/*----------------------------------------------------------------------
|    platform specifics
+---------------------------------------------------------------------*/
/* Windows 32 */
#if defined(WIN32)
#if !defined(STRICT)
#define STRICT
#endif

#define vsnprintf _vsnprintf

#endif

/* Microsoft C/C++ Compiler */
#if defined(_MSC_VER)
#define ATX_CONFIG_HAVE_INT64
#define ATX_CONFIG_INT64_TYPE __int64
#endif

/* QNX */
#if defined(__QNX__)
#endif

/*----------------------------------------------------------------------
|    compiler specifics
+---------------------------------------------------------------------*/
/* GCC */
#if defined(__GNUC__)
#define ATX_COMPILER_UNUSED(p) (void)p
#define ATX_CONFIG_HAVE_INT64
#define ATX_CONFIG_INT64_TYPE long long
#else
#define ATX_COMPILER_UNUSED(p) 
#endif

/*----------------------------------------------------------------------
|    standard C runtime
+---------------------------------------------------------------------*/
#if defined(ATX_CONFIG_HAVE_STD_C)
#define ATX_CONFIG_HAVE_MALLOC
#define ATX_CONFIG_HAVE_CALLOC
#define ATX_CONFIG_HAVE_REALLOC
#define ATX_CONFIG_HAVE_FREE
#define ATX_CONFIG_HAVE_MEMCPY
#define ATX_CONFIG_HAVE_MEMMOVE
#define ATX_CONFIG_HAVE_MEMSET
#endif /* ATX_CONFIG_HAS_STD_C */

#if defined(ATX_CONFIG_HAVE_STRING_H)
#define ATX_CONFIG_HAVE_STRCMP
#define ATX_CONFIG_HAVE_STRNCMP
#define ATX_CONFIG_HAVE_STRDUP
#define ATX_CONFIG_HAVE_STRLEN
#define ATX_CONFIG_HAVE_STRCPY
#define ATX_CONFIG_HAVE_STRNCPY
#endif /* ATX_CONFIG_HAVE_STRING_H */

#if defined(ATX_CONFIG_HAVE_CTYPE_H)
#define ATX_CONFIG_HAVE_IS_SPACE
#endif /* ATX_CONFIG_HAVE_CTYPE_H */

#endif /* _ATX_CONFIG_H_ */
