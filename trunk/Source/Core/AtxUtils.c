/*****************************************************************
|
|   Atomix - Utils
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxUtils.h"
#include "AtxResults.h"
#include "AtxConfig.h"
#if defined(ATX_CONFIG_HAVE_MATH_H)
#include <math.h>
#endif
#if defined(ATX_CONFIG_HAVE_LIMITS_H)
#include <limits.h>
#endif

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define ATX_FORMAT_LOCAL_BUFFER_SIZE 1024
#define ATX_FORMAT_BUFFER_INCREMENT  4096
#define ATX_FORMAT_BUFFER_MAX_SIZE   65536

/*----------------------------------------------------------------------
|    ATX_BytesFromInt32Be
+---------------------------------------------------------------------*/
void 
ATX_BytesFromInt32Be(unsigned char* buffer, ATX_UInt32 value)
{
    buffer[0] = (unsigned char)(value>>24) & 0xFF;
    buffer[1] = (unsigned char)(value>>16) & 0xFF;
    buffer[2] = (unsigned char)(value>> 8) & 0xFF;
    buffer[3] = (unsigned char)(value    ) & 0xFF;
}

/*----------------------------------------------------------------------
|    ATX_BytesFromInt16Be
+---------------------------------------------------------------------*/
void 
ATX_BytesFromInt16Be(unsigned char* buffer, ATX_UInt16 value)
{
    buffer[0] = (unsigned char)((value>> 8) & 0xFF);
    buffer[1] = (unsigned char)((value    ) & 0xFF);
}

/*----------------------------------------------------------------------
|    ATX_BytesToInt32Be
+---------------------------------------------------------------------*/
ATX_UInt32 
ATX_BytesToInt32Be(const unsigned char* buffer)
{
    return 
        ( ((ATX_UInt32)buffer[0])<<24 ) |
        ( ((ATX_UInt32)buffer[1])<<16 ) |
        ( ((ATX_UInt32)buffer[2])<<8  ) |
        ( ((ATX_UInt32)buffer[3])     );
}

/*----------------------------------------------------------------------
|    ATX_BytesToInt16Be
+---------------------------------------------------------------------*/
ATX_UInt16 
ATX_BytesToInt16Be(const unsigned char* buffer)
{
    return 
        ( ((unsigned short)buffer[0])<<8  ) |
        ( ((unsigned short)buffer[1])     );
}

/*----------------------------------------------------------------------
|    ATX_BytesFromInt32Le
+---------------------------------------------------------------------*/
void 
ATX_BytesFromInt32Le(unsigned char* buffer, ATX_UInt32 value)
{
    buffer[3] = (unsigned char)(value>>24) & 0xFF;
    buffer[2] = (unsigned char)(value>>16) & 0xFF;
    buffer[1] = (unsigned char)(value>> 8) & 0xFF;
    buffer[0] = (unsigned char)(value    ) & 0xFF;
}

/*----------------------------------------------------------------------
|    ATX_BytesFromInt16Le
+---------------------------------------------------------------------*/
extern void 
ATX_BytesFromInt16Le(unsigned char* buffer, ATX_UInt16 value)
{
    buffer[1] = (unsigned char)((value>> 8) & 0xFF);
    buffer[0] = (unsigned char)((value    ) & 0xFF);
}

/*----------------------------------------------------------------------
|    ATX_BytesToInt32Le
+---------------------------------------------------------------------*/
extern ATX_UInt32 
ATX_BytesToInt32Le(const unsigned char* buffer)
{
    return 
        ( ((ATX_UInt32)buffer[3])<<24 ) |
        ( ((ATX_UInt32)buffer[2])<<16 ) |
        ( ((ATX_UInt32)buffer[1])<<8  ) |
        ( ((ATX_UInt32)buffer[0])     );
}

/*----------------------------------------------------------------------
|    ATX_BytesToInt16Le
+---------------------------------------------------------------------*/
extern ATX_UInt16 
ATX_BytesToInt16Le(const unsigned char* buffer)
{
    return 
        ( ((unsigned short)buffer[1])<<8  ) |
        ( ((unsigned short)buffer[0])     );
}

/*----------------------------------------------------------------------
|    ATX_ParseFloat
+---------------------------------------------------------------------*/
ATX_Result 
ATX_ParseFloat(const char* str, float* result, ATX_Boolean relaxed)
{
    ATX_Boolean  after_radix = ATX_FALSE;
    ATX_Boolean  negative = ATX_FALSE;
    ATX_Boolean  empty = ATX_TRUE;
    float        value = 0.0f;
    float        decimal = 10.0f;
    char         c;

    /* safe default value */
    *result = 0.0f;

    /* check params */
    if (str == NULL || *str == '\0') {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* ignore leading whitespace */
    if (relaxed) {
        while (ATX_IsSpace(*str)) {
            str++;
        }
    }
    if (str == NULL || *str == '\0') {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* check for sign */
    if (*str == '-') {
        /* negative number */
        negative = ATX_TRUE; 
        str++;
    } else if (*str == '+') {
        /* skip the + sign */
        str++;
    }

    while ((c = *str++)) {
        if (c == '.') {
            if (after_radix || (*str < '0' || *str > '9')) {
                return ATX_ERROR_INVALID_PARAMETERS;
            } else {
                after_radix = ATX_TRUE;
            }
        } else if (c >= '0' && c <= '9') {
            empty = ATX_FALSE;
            if (after_radix) {
                value += (float)(c-'0')/decimal;
                decimal *= 10.0f;
            } else {
                value = 10.0f*value + (float)(c-'0');
            }
        } else if (c == 'e' || c == 'E') {
            /* exponent */
            if (*str == '+' || *str == '-' || (*str >= '0' && *str <= '9')) {
                long exponent = 0;
                if (ATX_SUCCEEDED(ATX_ParseInteger(str, &exponent, relaxed))) {
                    value *= (float)pow(10.0f, (float)exponent);
                    break;
                } else {
                    return ATX_ERROR_INVALID_PARAMETERS;
                }
            } else {
                return ATX_ERROR_INVALID_PARAMETERS;
            }
        } else {
            if (relaxed) {
                break;
            } else {
                return ATX_ERROR_INVALID_PARAMETERS;
            }
        } 
    }

    /* check that the value was non empty */
    if (empty) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* return the result */
    *result = negative ? -value : value;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_ParseInteger
+---------------------------------------------------------------------*/
ATX_Result 
ATX_ParseInteger(const char* str, long* result, ATX_Boolean relaxed)
{
    ATX_Boolean negative = ATX_FALSE;
    ATX_Boolean empty    = ATX_TRUE;
    long        value    = 0;
    long        max      = ATX_INT_MAX/10;
    char        c;

    /* safe default value */
    *result = 0;

    /* check params */
    if (str == NULL || *str == '\0') {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* ignore leading whitespace */
    if (relaxed) {
        while (ATX_IsSpace(*str)) {
            str++;
        }
    }
    if (*str == '\0') return ATX_ERROR_INVALID_PARAMETERS;

    /* check for sign */
    if (*str == '-') {
        /* negative number */
        negative = ATX_TRUE; 
        str++;
    } else if (*str == '+') {
        /* skip the + sign */
        str++;
    }

    /* adjust the max for overflows when the value is negative */
    if (negative && ((ATX_INT_MAX%10) == 9)) ++max;

    while ((c = *str++)) {
        if (c >= '0' && c <= '9') {
            if (value < 0 || value > max) return ATX_ERROR_OVERFLOW;
            value = 10*value + (c-'0');
            if (value < 0 && (!negative || value != ATX_INT_MIN)) return ATX_ERROR_OVERFLOW;
            empty = ATX_FALSE;
        } else {
            if (relaxed) {
                break;
            } else {
                return ATX_ERROR_INVALID_PARAMETERS;
            }
        } 
    }

    /* check that the value was non empty */
    if (empty) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* return the result */
    if (negative) {
        *result = -value;
    } else {
        *result = value;
    }
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_ParseIntegerU
+---------------------------------------------------------------------*/
ATX_Result 
ATX_ParseIntegerU(const char* str, unsigned long* result, ATX_Boolean relaxed)
{
    ATX_Boolean   empty = ATX_TRUE;
    unsigned long value = 0;
    unsigned long max   = ATX_UINT_MAX/10;
    char          c;

    /* safe default value */
    *result = 0;

    /* check params */
    if (str == NULL || *str == '\0') {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* ignore leading whitespace */
    if (relaxed) {
        while (ATX_IsSpace(*str)) {
            str++;
        }
    }
    if (*str == '\0') return ATX_ERROR_INVALID_PARAMETERS;

    while ((c = *str++)) {
        if (c >= '0' && c <= '9') {
            unsigned long new_value;
            if (value > max)  return ATX_ERROR_OVERFLOW;
            new_value = 10*value + (c-'0');
            if (new_value < value) return ATX_ERROR_OVERFLOW;
            value = new_value;
            empty = ATX_FALSE;
        } else {
            if (relaxed) {
                break;
            } else {
                return ATX_ERROR_INVALID_PARAMETERS;
            }
        } 
    }

    /* check that the value was non empty */
    if (empty) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* return the result */
    *result = value;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_ParseInteger32
+---------------------------------------------------------------------*/
ATX_Result 
ATX_ParseInteger32(const char* str, ATX_Int32* value, ATX_Boolean relaxed)
{
    long value_l;
    ATX_Result result = ATX_ParseInteger(str, &value_l, relaxed);
    *value = 0;
    if (ATX_SUCCEEDED(result)) {
        if (value_l < ATX_INT_MIN || value_l > ATX_INT_MAX) {
            return ATX_ERROR_OVERFLOW;
        }
        *value = (ATX_Int32)value_l;
    }
    return result;
}

/*----------------------------------------------------------------------
|    ATX_ParseInteger32U
+---------------------------------------------------------------------*/
ATX_Result 
ATX_ParseInteger32U(const char* str, ATX_UInt32* value, ATX_Boolean relaxed)
{
    unsigned long value_l;
    ATX_Result result = ATX_ParseIntegerU(str, &value_l, relaxed);
    *value = 0;
    if (ATX_SUCCEEDED(result)) {
        if (value_l > ATX_UINT_MAX) return ATX_ERROR_OVERFLOW;
        *value = (ATX_UInt32)value_l;
    }
    return result;
}

/*----------------------------------------------------------------------
|   ATX_FloatToString
+---------------------------------------------------------------------*/
ATX_Result
ATX_FloatToString(float value, char* buffer, ATX_Size buffer_size)
{
    char  s[256];
    char* c = s;

    /* check arguments */
    if (buffer_size < 4) return ATX_ERROR_OUT_OF_RANGE;

    /* deal with the sign */
    if (value < 0.0f) {
        value = -value;
        *c++ = '-';
    }

    if (value == 0.0f) {
        *c++ = '0';
    } else {
        float limit;
        do {
            ATX_Int32 integer_part;
            limit = 1.0f;
            while (value > limit*1E9f) {
                limit *= 1E9f;
            }
            /* convert the top of the integer part */
            integer_part = (ATX_Int32)(value/limit);
            ATX_IntegerToString(integer_part, c, (ATX_Size)(sizeof(s)-(c-&s[0])));
            while (*c != '\0') { ++c; }
            value -= limit*(float)integer_part;
        } while (limit > 1.0f);
    }

    /* emit the fractional part */
    if (value >= 1.0f) {
        *buffer = '\0';
        return ATX_ERROR_INTERNAL;
    }
    *c++ = '.';
    if (value <= 1E-6) {
        *c++ = '0';
        *c++ = '\0';
    } else {
        ATX_Int32 factional_part = (ATX_Int32)(value*1E6);
        do {
            int digit = factional_part/100000;
            factional_part = 10*(factional_part-(digit*100000));
            *c++ = '0'+digit;
        } while (factional_part);
        *c++ = '\0';
    }

    /* copy the string */
    if (ATX_StringLength(s)+1 > buffer_size) {
        return ATX_ERROR_OUT_OF_RANGE;
    } else {
        ATX_CopyString(buffer, s);
        return ATX_SUCCESS;
    }
}

/*----------------------------------------------------------------------
|   ATX_IntegerToString
+---------------------------------------------------------------------*/
ATX_Result
ATX_IntegerToString(long value, char* buffer, ATX_Size buffer_size)
{
    char s[32];
    char* c = &s[31];
    ATX_Boolean negative;
    *c-- = '\0';

    /* default value */
    if (buffer == NULL || buffer_size == 0) return ATX_ERROR_INVALID_PARAMETERS;
    buffer[0] = '\0';

    /* handle the sign */
    negative = ATX_FALSE;
    if (value < 0) {
        negative = ATX_TRUE;
        value = -value;
    }

    /* process the digits */
    do {
        int digit = value%10;
        *c-- = '0'+digit;
        value /= 10;
    } while(value);

    if (negative) {
        *c = '-';
    } else {
        ++c;
    }

    /* check that the string fits */
    if ((ATX_Size)(&s[31]-c)+1 > buffer_size) return ATX_ERROR_OUT_OF_RANGE;

    /* copy the string */
    ATX_CopyString(buffer, c);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_IntegerToStringU
+---------------------------------------------------------------------*/
ATX_Result
ATX_IntegerToStringU(unsigned long value, char* buffer, ATX_Size buffer_size)
{
    char s[32];
    char* c = &s[31];
    *c-- = '\0';

    /* default value */
    if (buffer == NULL || buffer_size == 0) return ATX_ERROR_INVALID_PARAMETERS;
    buffer[0] = '\0';

    /* process the digits */
    do {
        unsigned int digit = value%10;
        *c-- = '0'+digit;
        value /= 10;
    } while(value);
    ++c;

    /* check that the string fits */
    if ((ATX_Size)(&s[31]-c)+1 > buffer_size) return ATX_ERROR_OUT_OF_RANGE;

    /* copy the string */
    ATX_CopyString(buffer, c);

    return ATX_SUCCESS;
}

#if !defined(ATX_CONFIG_HAVE_STRCPY)
/*----------------------------------------------------------------------
|    ATX_CopyString
+---------------------------------------------------------------------*/
void
ATX_CopyString(char* dst, const char* src)
{
    while(*dst++ = *src++);
}
#endif

/*----------------------------------------------------------------------
|   ATX_FormatOutput
+---------------------------------------------------------------------*/
void
ATX_FormatOutput(void        (*function)(void* parameter, const char* message),
                 void*       function_parameter,
                 const char* format, 
                 va_list     args)
{
    char         local_buffer[ATX_FORMAT_LOCAL_BUFFER_SIZE];
    unsigned int buffer_size = ATX_FORMAT_LOCAL_BUFFER_SIZE;
    char*        buffer = local_buffer;

    for(;;) {
        int result;

        /* try to format the message (it might not fit) */
        result = ATX_vsnprintf(buffer, buffer_size-1, format, args);
        buffer[buffer_size-1] = 0; /* force a NULL termination */
        if (result >= 0) break;

        /* the buffer was too small, try something bigger */
        buffer_size = (buffer_size+ATX_FORMAT_BUFFER_INCREMENT)*2;
        if (buffer_size > ATX_FORMAT_BUFFER_MAX_SIZE) break;
        if (buffer != local_buffer) ATX_FreeMemory((void*)buffer);
        buffer = ATX_AllocateMemory(buffer_size);
        if (buffer == NULL) return;
    }

    (*function)(function_parameter, buffer);
    if (buffer != local_buffer) ATX_FreeMemory((void*)buffer);
}

/*----------------------------------------------------------------------
|   ATX_ScrubMemory
+---------------------------------------------------------------------*/
void 
ATX_ScrubMemory(void* buffer, ATX_Size size)
{
    if (buffer == NULL) return;

    {
        volatile char* mem = (volatile char*)buffer; 
    
        while (size--) {
            *mem++ = 0; 
        }
    }
}
