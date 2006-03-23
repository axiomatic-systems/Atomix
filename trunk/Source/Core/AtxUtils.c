/*****************************************************************
|
|      File: AtxUtils.c
|
|      Atomix - Utils
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxUtils.h"
#include "AtxResults.h"
#include "AtxConfig.h"
#if defined(ATX_CONFIG_HAVE_MATH_H)
#include <math.h>
#endif

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
        ( ((unsigned long)buffer[0])<<24 ) |
        ( ((unsigned long)buffer[1])<<16 ) |
        ( ((unsigned long)buffer[2])<<8  ) |
        ( ((unsigned long)buffer[3])     );
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
        ( ((unsigned long)buffer[3])<<24 ) |
        ( ((unsigned long)buffer[2])<<16 ) |
        ( ((unsigned long)buffer[1])<<8  ) |
        ( ((unsigned long)buffer[0])     );
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

    while ((c = *str++)) {
        if (c >= '0' && c <= '9') {
            value = 10*value + (c-'0');
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
    *result = negative ? -value : value;
    return ATX_SUCCESS;
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
    if (ATX_StringLength(s)+1 > buffer_size) return ATX_ERROR_OUT_OF_RANGE;

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

    /* process the digits */
    do {
        unsigned int digit = value%10;
        *c-- = '0'+digit;
        value /= 10;
    } while(value);
    ++c;

    /* check that the string fits */
    if (ATX_StringLength(s)+1 > buffer_size) return ATX_ERROR_OUT_OF_RANGE;

    /* copy the string */
    ATX_CopyString(buffer, c);

    return ATX_SUCCESS;
}

#if !defined(ATX_CONFIG_HAVE_STRCPY)
/*----------------------------------------------------------------------
|    ATX_CopyString
+---------------------------------------------------------------------*/
char*
ATX_CopyString(char* dst, const char* src)
{
	char* result = dst;
	while(*dst++ = *src++);
	return result;
}
#endif
