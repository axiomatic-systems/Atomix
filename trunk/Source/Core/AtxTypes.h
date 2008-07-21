/*****************************************************************
|
|   Atomix - General Types
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_TYPES_H_
#define _ATX_TYPES_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxDefs.h"

/*----------------------------------------------------------------------
|   generic types
+---------------------------------------------------------------------*/
typedef unsigned int   ATX_UInt32;
typedef int            ATX_Int32;
typedef unsigned short ATX_UInt16;
typedef short          ATX_Int16;
typedef unsigned char  ATX_UInt8;
typedef signed char    ATX_Int8;
typedef float          ATX_Float;

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
/**
 * Boolean type used for variables that can be true (ATX_TRUE) or false
 * (ATX_FALSE)
 */
typedef enum {
    ATX_FALSE = 0, 
    ATX_TRUE  = 1
} ATX_Boolean;

/**
 * Signed integer value representing a function or method result (return value)
 *
 * When a function or method call succeeds, the return value is always 
 * ATX_SUCCESS unless otherwise documented. Error conditions are always
 * negative values, defined in AtxResults.h
 */
typedef int ATX_Result;

/**
 * Type used to represent a bit pattern signifying a combination of flags
 * that can be on or off. Bits set to 1 indicate that the corresponding flag
 * is on, bits set to 0 indicate that the corresponding flag is off. The
 * position and meaning of flags is specific to each method, function, variable
 * or data structure that uses this type, and the corresponding header file
 * will give symbolic constants to represent individual flag bits.
 */
typedef ATX_UInt32 ATX_Flags;

/**
 * An unsigned integer used to represent a bit mask.
 */
typedef ATX_UInt32 ATX_Mask;

/**
 * An unsigned integer used to represent a measurable quantity (eg. the 
 * size of a file)
 */
typedef unsigned long ATX_Size;

/**
 * A signed integer used to represent an offset from a base value.
 */
typedef signed long ATX_Offset;

/**
* A signed integer used to represent a positive offset from a base value.
*/
typedef unsigned long ATX_Position;

/**
 * An address as a generic pointer, that can be dereferenced as a byte address.
 */
typedef unsigned char* ATX_Address;

/**
 * An unsigned integer used to represent the difference between a matximum 
 * value and a minimum value.
 */
typedef ATX_UInt32 ATX_Range;

/**
 * An unsigned integer used to represent a quantity that can be counted (such
 * as an number of elements in a list).
 */
typedef ATX_UInt32 ATX_Cardinal;

/**
 * An unsigned integer that represents a position in a sequence (such as an
 * index into a list of elements).
 */
typedef ATX_UInt32  ATX_Ordinal;

/**
 * An unsigned integer used to represent a version Id. Version Ids are 
 * monotonic, so that a larger integer means a more recent version.
 */
typedef ATX_UInt32  ATX_VersionId;

/**
 * A const pointer to a NULL-terminated character array, used to represent 
 * strings. Strings that cannot be represented by characters in the ASCII set
 * are encoded as UTF-8 unless otherwise specified.
 */
typedef const char* ATX_CString;
typedef char*       ATX_CStringBuffer;

/**
 * Pointer to void, used to represent pointers to arbitrary untyped data 
 * buffers.
 */
typedef void*       ATX_Any;
typedef const void* ATX_AnyConst;

/**
 * 8-bit Byte
 */
typedef ATX_UInt8 ATX_Byte;

/**
 * Pointer to a byte buffer
 */
typedef ATX_UInt8* ATX_ByteBuffer;

/**
 * Timeout in milliseconds
 */
typedef long       ATX_Timeout;

/**
 * 64 bit integers
 */
#if defined(ATX_CONFIG_HAVE_INT64)
typedef ATX_CONFIG_INT64_TYPE ATX_Int64;
typedef unsigned ATX_CONFIG_INT64_TYPE ATX_UInt64;
#else 
#error ATX_CONFIG_INT64_TYPE not defined
#endif

/**
 * Integer that matches the native machine word size.
 * This is 32-bit in 32-bit mode, and 64-bit in 64-bit mode
 * (This type is necessary, because not all 64-bit systems
 * have 64-bit longs)
 */
typedef ATX_CONFIG_INT_32_64_TYPE ATX_Int3264;
typedef unsigned ATX_CONFIG_INT_32_64_TYPE ATX_UInt3264;

#if !defined(ATX_INT_MIN)
#if defined(ATX_CONFIG_HAVE_INT_MIN)
#define ATX_INT_MIN INT_MIN
#endif
#endif

#if !defined(ATX_INT_MAX)
#if defined(ATX_CONFIG_HAVE_INT_MAX)
#define ATX_INT_MAX INT_MAX
#endif
#endif

#if !defined(ATX_UINT_MAX)
#if defined(ATX_CONFIG_HAVE_UINT_MAX)
#define ATX_UINT_MAX UINT_MAX
#endif
#endif

#if !defined(ATX_LONG_MIN)
#if defined(ATX_CONFIG_HAVE_LONG_MIN)
#define ATX_LONG_MIN LONG_MIN
#endif
#endif

#if !defined(ATX_LONG_MAX)
#if defined(ATX_CONFIG_HAVE_LONG_MAX)
#define ATX_LONG_MAX LONG_MAX
#endif
#endif

#if !defined(ATX_ULONG_MAX)
#if defined(ATX_CONFIG_HAVE_ULONG_MAX)
#define ATX_ULONG_MAX ULONG_MAX
#endif
#endif

#if !defined(ATX_INT64_MIN)
#if defined(ATX_CONFIG_HAVE_LONG_LONG_MIN)
#define ATX_INT64_MIN LONG_LONG_MIN
#endif
#endif

#if !defined(ATX_INT64_MAX)
#if defined(ATX_CONFIG_HAVE_LONG_LONG_MAX)
#define ATX_INT64_MAX LONG_LONG_MAX
#endif
#endif

#if !defined(ATX_UINT64_MAX)
#if defined(ATX_CONFIG_HAVE_ULONG_LONG_MAX)
#define ATX_UINT64_MAX ULONG_LONG_MAX
#endif
#endif

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
#if defined(ATX_CONFIG_HAVE_INT64)
#define ATX_Int64_Zero(i) do {(i)=0;} while(0)
#define ATX_Int64_Add_Int64(a,b) ((a)+=(b))
#define ATX_Int64_Add_Int32(a,b) ((a)+=(ATX_CONFIG_INT64_TYPE)(b))
#define ATX_Int64_Sub_Int64(a,b) ((a)-=(b))
#define ATX_Int64_Sub_Int32(a,b) ((a)-=(ATX_CONFIG_INT64_TYPE)(b))
#define ATX_Int64_Mul_Int64(a,b) ((a)*=(b))
#define ATX_Int64_Mul_Int32(a,b) ((a)*=(ATX_CONFIG_INT64_TYPE)(b))
#define ATX_Int64_Div_Int64(a,b) ((a)/=(b))
#define ATX_Int64_Div_Int32(a,b) ((a)/=(ATX_CONFIG_INT64_TYPE)(b))
#define ATX_Int64_Set_Int32(a,b) ((a) = (ATX_CONFIG_INT64_TYPE)(b))
#define ATX_Int64_Get_Int32(i) ((ATX_Int32)(i))
#define ATX_Int64_Equal(a,b) ((a) == (b))
#else
/* no INT64 */
#endif

#endif /* _ATX_TYPES_H_ */
