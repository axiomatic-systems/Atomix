/*****************************************************************
|
|   Atomix - Platform Configuration
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
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
#define ATX_CONFIG_HAVE_STDIO_H
#define ATX_CONFIG_HAVE_STDARG_H
#define ATX_CONFIG_HAVE_CTYPE_H
#define ATX_CONFIG_HAVE_MATH_H
#define ATX_CONFIG_HAVE_ASSERT_H
#define ATX_CONFIG_HAVE_LIMITS_H

/*----------------------------------------------------------------------
|    CPU byte order
+---------------------------------------------------------------------*/
#define ATX_CPU_BIG_ENDIAN    1
#define ATX_CPU_LITTLE_ENDIAN 2

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
#define ATX_CONFIG_HAVE_MEMCMP
#define ATX_CONFIG_HAVE_ATEXIT
#define ATX_CONFIG_HAVE_GETENV
#endif /* ATX_CONFIG_HAS_STD_C */

#if defined(ATX_CONFIG_HAVE_STRING_H)
#define ATX_CONFIG_HAVE_STRCMP
#define ATX_CONFIG_HAVE_STRNCMP
#define ATX_CONFIG_HAVE_STRCHR
#define ATX_CONFIG_HAVE_STRDUP
#define ATX_CONFIG_HAVE_STRLEN
#define ATX_CONFIG_HAVE_STRCPY
#define ATX_CONFIG_HAVE_STRNCPY
#endif /* ATX_CONFIG_HAVE_STRING_H */

#if defined(ATX_CONFIG_HAVE_STDIO_H)
#define ATX_CONFIG_HAVE_SNPRINTF
#define ATX_CONFIG_HAVE_VSNPRINTF
#endif /* ATX_CONFIG_HAVE_STDIO_H_ */

#if defined(ATX_CONFIG_HAVE_CTYPE_H)
#define ATX_CONFIG_HAVE_IS_SPACE
#define ATX_CONFIG_HAVE_IS_ALNUM
#endif /* ATX_CONFIG_HAVE_CTYPE_H */

#if defined(ATX_CONFIG_HAVE_LIMITS_H)
#define ATX_CONFIG_HAVE_INT_MIN
#define ATX_CONFIG_HAVE_INT_MAX
#define ATX_CONFIG_HAVE_UINT_MAX
#define ATX_CONFIG_HAVE_LONG_MIN
#define ATX_CONFIG_HAVE_LONG_MAX
#define ATX_CONFIG_HAVE_ULONG_MAX
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

/* Microsoft C Compiler */
#if defined(_MSC_VER)
#define ATX_CONFIG_HAVE_INT64
#define ATX_CONFIG_INT64_TYPE __int64
#define ATX_strdup     _strdup
#if (_MSC_VER >= 1400)
#define ATX_vsnprintf(s,c,f,a) _vsnprintf_s(s,c,_TRUNCATE,f,a)
#define ATX_strncpy(d,s,c)     strncpy_s(d,c,s,_TRUNCATE)
#else
#define ATX_vsnprintf  _vsnprintf
#define ATX_strncpy    _strncpy
#endif
#define ATX_snprintf   _snprintf
#if (_MSC_VER >= 1300)
typedef __w64 long ATX_PointerLong;
#define ATX_POINTER_TO_LONG(_p) ((long)(ATX_PointerLong) (_p) )
#undef ATX_CONFIG_HAVE_STRCPY
#endif
#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#endif
#endif

/*----------------------------------------------------------------------
|   defaults
+---------------------------------------------------------------------*/
#ifndef ATX_POINTER_TO_LONG
#define ATX_POINTER_TO_LONG(_p) ((long)(_p))
#endif

/*----------------------------------------------------------------------
|    defaults
+---------------------------------------------------------------------*/
/* some compilers (ex: MSVC 8) deprecate those, so we rename them */
#if !defined(ATX_strdup)
#define ATX_strdup strdup
#endif
#if !defined(ATX_snprintf)
#define ATX_snprintf snprintf
#endif
#if !defined(ATX_snprintf)
#define ATX_snprintf snprintf
#endif
#if !defined(ATX_vsnprintf)
#define ATX_vsnprintf vsnprintf
#endif

#endif /* _ATX_CONFIG_H_ */
