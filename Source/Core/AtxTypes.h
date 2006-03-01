/*****************************************************************
|
|      File: AtxDefs.h
|
|      Atomix - General Types
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_TYPES_H_
#define _ATX_TYPES_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxDefs.h"

/*----------------------------------------------------------------------
|       generic types
+---------------------------------------------------------------------*/
typedef unsigned long  ATX_UInt32;
typedef long           ATX_Int32;
typedef unsigned short ATX_UInt16;
typedef short          ATX_Int16;
typedef unsigned char  ATX_UInt8;
typedef signed char    ATX_Int8;
typedef int            ATX_Int;
typedef float          ATX_Float;

/*----------------------------------------------------------------------
|       types
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
 * Signed integer value represening a function or method result (return value)
 *
 * When a function or method call succeeds, the return value is always 
 * ATX_SUCCESS unless otherwise documented. Error conditions are always
 * negative values, defined in AtxErrors.h
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
typedef ATX_UInt32 ATX_Size;

/**
 * A signed integer used to represent an offset from a base value of position.
 */
typedef ATX_Int32 ATX_Offset;

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
#else
typedef struct {
    ATX_UInt32 high;
    ATX_UInt32 low;
} ATX_Int64;
#endif

/*----------------------------------------------------------------------
|       functions
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
