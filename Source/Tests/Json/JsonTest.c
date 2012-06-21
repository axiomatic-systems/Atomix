/*****************************************************************
|
|      Atomix Tests - JSON Tests
|
| Copyright (c) 2002-2010, Axiomatic Systems, LLC.
| All rights reserved.
|
| Redistribution and use in source and binary forms, with or without
| modification, are permitted provided that the following conditions are met:
|     * Redistributions of source code must retain the above copyright
|       notice, this list of conditions and the following disclaimer.
|     * Redistributions in binary form must reproduce the above copyright
|       notice, this list of conditions and the following disclaimer in the
|       documentation and/or other materials provided with the distribution.
|     * Neither the name of Axiomatic Systems nor the
|       names of its contributors may be used to endorse or promote products
|       derived from this software without specific prior written permission.
|
| THIS SOFTWARE IS PROVIDED BY AXIOMATIC SYSTEMS ''AS IS'' AND ANY
| EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
| WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
| DISCLAIMED. IN NO EVENT SHALL AXIOMATIC SYSTEMS BE LIABLE FOR ANY
| DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
| (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
| ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
| (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
| SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "Atomix.h"

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#define CHECK(x)                                            \
    do {                                                    \
        if (!(x)) {                                         \
            ATX_Debug("failed line %d (%d)\n", __LINE__, x);\
            ATX_ASSERT(0);                                  \
        }                                                   \
    } while(0)                                         

#define SHOULD_SUCCEED(r)                                   \
    do {                                                    \
        ATX_Result x = r;                                   \
        if (ATX_FAILED(x)) {                                \
            ATX_Debug("failed line %d (%d)\n", __LINE__, x);\
            ATX_ASSERT(0);                                  \
        }                                                   \
    } while(0)                                         

#define SHOULD_FAIL(r)                                                  \
    do {                                                                \
        ATX_Result x = r;                                               \
        if (ATX_SUCCEEDED(x)) {                                         \
            ATX_Debug("should have failed line %d (%d)\n", __LINE__, r);\
            ATX_ASSERT(0);                                              \
        }                                                               \
    } while(0)                                  

static char pass1_json[] = {
#if 0
[
    "JSON Test Pattern pass1",
    {"object with 1 member":["array with 1 element"]},
    {},
    [],
    -42,
    true,
    false,
    null,
    {
        "integer": 1234567890,
        "real": -9876.543210,
        "e": 0.123456789e-12,
        "E": 1.234567890E+34,
        "":  23456789012E66,
        "zero": 0,
        "one": 1,
        "space": " ",
        "quote": "\"",
        "backslash": "\\",
        "controls": "\b\f\n\r\t",
        "slash": "/ & \/",
        "alpha": "abcdefghijklmnopqrstuvwyz",
        "ALPHA": "ABCDEFGHIJKLMNOPQRSTUVWYZ",
        "digit": "0123456789",
        "0123456789": "digit",
        "special": "`1~!@#$%^&*()_+-={':[,]}|;.</>?",
        "hex": "\u0123\u4567\u89AB\uCDEF\uabcd\uef4A",
        "true": true,
        "false": false,
        "null": null,
        "array":[  ],
        "object":{  },
        "address": "50 St. James Street",
        "url": "http://www.JSON.org/",
        "comment": "// /* <!-- --",
        "# -- --> */": " ",
        " s p a c e d " :[1,2 , 3

,

4 , 5        ,          6           ,7        ],"compact":[1,2,3,4,5,6,7],
        "jsontext": "{\"object with 1 member\":[\"array with 1 element\"]}",
        "quotes": "&#34; \u0022 %22 0x22 034 &#x22;",
        "\/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"
: "A key can be any string"
    },
    0.5 ,98.6
,
99.44
,

1066,
1e1,
0.1e1,
1e-1,
1e00,2e+00,2e-00
,"rosebud"]
#endif
  0x5b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x22, 0x4a, 0x53, 0x4f, 0x4e, 0x20,
  0x54, 0x65, 0x73, 0x74, 0x20, 0x50, 0x61, 0x74, 0x74, 0x65, 0x72, 0x6e,
  0x20, 0x70, 0x61, 0x73, 0x73, 0x31, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x7b, 0x22, 0x6f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x20, 0x77, 0x69,
  0x74, 0x68, 0x20, 0x31, 0x20, 0x6d, 0x65, 0x6d, 0x62, 0x65, 0x72, 0x22,
  0x3a, 0x5b, 0x22, 0x61, 0x72, 0x72, 0x61, 0x79, 0x20, 0x77, 0x69, 0x74,
  0x68, 0x20, 0x31, 0x20, 0x65, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x22,
  0x5d, 0x7d, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x7b, 0x7d, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x5b, 0x5d, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x2d, 0x34, 0x32, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74, 0x72, 0x75,
  0x65, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x61, 0x6c, 0x73, 0x65,
  0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6e, 0x75, 0x6c, 0x6c, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x22, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x22, 0x3a,
  0x20, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2c,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x72, 0x65,
  0x61, 0x6c, 0x22, 0x3a, 0x20, 0x2d, 0x39, 0x38, 0x37, 0x36, 0x2e, 0x35,
  0x34, 0x33, 0x32, 0x31, 0x30, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x22, 0x65, 0x22, 0x3a, 0x20, 0x30, 0x2e, 0x31, 0x32,
  0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x65, 0x2d, 0x31, 0x32, 0x2c,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x45, 0x22,
  0x3a, 0x20, 0x31, 0x2e, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x30, 0x45, 0x2b, 0x33, 0x34, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x22, 0x22, 0x3a, 0x20, 0x20, 0x32, 0x33, 0x34, 0x35,
  0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x45, 0x36, 0x36, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x7a, 0x65, 0x72,
  0x6f, 0x22, 0x3a, 0x20, 0x30, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x22, 0x6f, 0x6e, 0x65, 0x22, 0x3a, 0x20, 0x31, 0x2c,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x73, 0x70,
  0x61, 0x63, 0x65, 0x22, 0x3a, 0x20, 0x22, 0x20, 0x22, 0x2c, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x71, 0x75, 0x6f, 0x74,
  0x65, 0x22, 0x3a, 0x20, 0x22, 0x5c, 0x22, 0x22, 0x2c, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x62, 0x61, 0x63, 0x6b, 0x73,
  0x6c, 0x61, 0x73, 0x68, 0x22, 0x3a, 0x20, 0x22, 0x5c, 0x5c, 0x22, 0x2c,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x63, 0x6f,
  0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x73, 0x22, 0x3a, 0x20, 0x22, 0x5c, 0x62,
  0x5c, 0x66, 0x5c, 0x6e, 0x5c, 0x72, 0x5c, 0x74, 0x22, 0x2c, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x73, 0x6c, 0x61, 0x73,
  0x68, 0x22, 0x3a, 0x20, 0x22, 0x2f, 0x20, 0x26, 0x20, 0x5c, 0x2f, 0x22,
  0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x61,
  0x6c, 0x70, 0x68, 0x61, 0x22, 0x3a, 0x20, 0x22, 0x61, 0x62, 0x63, 0x64,
  0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x79, 0x7a, 0x22, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x41, 0x4c, 0x50,
  0x48, 0x41, 0x22, 0x3a, 0x20, 0x22, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
  0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52,
  0x53, 0x54, 0x55, 0x56, 0x57, 0x59, 0x5a, 0x22, 0x2c, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x64, 0x69, 0x67, 0x69, 0x74,
  0x22, 0x3a, 0x20, 0x22, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x22, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x22, 0x3a, 0x20, 0x22, 0x64, 0x69, 0x67, 0x69, 0x74, 0x22, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x73, 0x70, 0x65,
  0x63, 0x69, 0x61, 0x6c, 0x22, 0x3a, 0x20, 0x22, 0x60, 0x31, 0x7e, 0x21,
  0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29, 0x5f, 0x2b, 0x2d,
  0x3d, 0x7b, 0x27, 0x3a, 0x5b, 0x2c, 0x5d, 0x7d, 0x7c, 0x3b, 0x2e, 0x3c,
  0x2f, 0x3e, 0x3f, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x22, 0x68, 0x65, 0x78, 0x22, 0x3a, 0x20, 0x22, 0x5c, 0x75,
  0x30, 0x31, 0x32, 0x33, 0x5c, 0x75, 0x34, 0x35, 0x36, 0x37, 0x5c, 0x75,
  0x38, 0x39, 0x41, 0x42, 0x5c, 0x75, 0x43, 0x44, 0x45, 0x46, 0x5c, 0x75,
  0x61, 0x62, 0x63, 0x64, 0x5c, 0x75, 0x65, 0x66, 0x34, 0x41, 0x22, 0x2c,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x74, 0x72,
  0x75, 0x65, 0x22, 0x3a, 0x20, 0x74, 0x72, 0x75, 0x65, 0x2c, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x66, 0x61, 0x6c, 0x73,
  0x65, 0x22, 0x3a, 0x20, 0x66, 0x61, 0x6c, 0x73, 0x65, 0x2c, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x6e, 0x75, 0x6c, 0x6c,
  0x22, 0x3a, 0x20, 0x6e, 0x75, 0x6c, 0x6c, 0x2c, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x61, 0x72, 0x72, 0x61, 0x79, 0x22,
  0x3a, 0x5b, 0x20, 0x20, 0x5d, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x22, 0x6f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x22, 0x3a,
  0x7b, 0x20, 0x20, 0x7d, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x22, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x22, 0x3a,
  0x20, 0x22, 0x35, 0x30, 0x20, 0x53, 0x74, 0x2e, 0x20, 0x4a, 0x61, 0x6d,
  0x65, 0x73, 0x20, 0x53, 0x74, 0x72, 0x65, 0x65, 0x74, 0x22, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x75, 0x72, 0x6c,
  0x22, 0x3a, 0x20, 0x22, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77,
  0x77, 0x77, 0x2e, 0x4a, 0x53, 0x4f, 0x4e, 0x2e, 0x6f, 0x72, 0x67, 0x2f,
  0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
  0x63, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x22, 0x3a, 0x20, 0x22, 0x2f,
  0x2f, 0x20, 0x2f, 0x2a, 0x20, 0x3c, 0x21, 0x2d, 0x2d, 0x20, 0x2d, 0x2d,
  0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
  0x23, 0x20, 0x2d, 0x2d, 0x20, 0x2d, 0x2d, 0x3e, 0x20, 0x2a, 0x2f, 0x22,
  0x3a, 0x20, 0x22, 0x20, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x22, 0x20, 0x73, 0x20, 0x70, 0x20, 0x61, 0x20, 0x63,
  0x20, 0x65, 0x20, 0x64, 0x20, 0x22, 0x20, 0x3a, 0x5b, 0x31, 0x2c, 0x32,
  0x20, 0x2c, 0x20, 0x33, 0x0a, 0x0a, 0x2c, 0x0a, 0x0a, 0x34, 0x20, 0x2c,
  0x20, 0x35, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2c, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x36, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2c, 0x37, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5d, 0x2c, 0x22, 0x63, 0x6f,
  0x6d, 0x70, 0x61, 0x63, 0x74, 0x22, 0x3a, 0x5b, 0x31, 0x2c, 0x32, 0x2c,
  0x33, 0x2c, 0x34, 0x2c, 0x35, 0x2c, 0x36, 0x2c, 0x37, 0x5d, 0x2c, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x6a, 0x73, 0x6f,
  0x6e, 0x74, 0x65, 0x78, 0x74, 0x22, 0x3a, 0x20, 0x22, 0x7b, 0x5c, 0x22,
  0x6f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20,
  0x31, 0x20, 0x6d, 0x65, 0x6d, 0x62, 0x65, 0x72, 0x5c, 0x22, 0x3a, 0x5b,
  0x5c, 0x22, 0x61, 0x72, 0x72, 0x61, 0x79, 0x20, 0x77, 0x69, 0x74, 0x68,
  0x20, 0x31, 0x20, 0x65, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x5c, 0x22,
  0x5d, 0x7d, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x22, 0x71, 0x75, 0x6f, 0x74, 0x65, 0x73, 0x22, 0x3a, 0x20, 0x22,
  0x26, 0x23, 0x33, 0x34, 0x3b, 0x20, 0x5c, 0x75, 0x30, 0x30, 0x32, 0x32,
  0x20, 0x25, 0x32, 0x32, 0x20, 0x30, 0x78, 0x32, 0x32, 0x20, 0x30, 0x33,
  0x34, 0x20, 0x26, 0x23, 0x78, 0x32, 0x32, 0x3b, 0x22, 0x2c, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x5c, 0x2f, 0x5c, 0x5c,
  0x5c, 0x22, 0x5c, 0x75, 0x43, 0x41, 0x46, 0x45, 0x5c, 0x75, 0x42, 0x41,
  0x42, 0x45, 0x5c, 0x75, 0x41, 0x42, 0x39, 0x38, 0x5c, 0x75, 0x46, 0x43,
  0x44, 0x45, 0x5c, 0x75, 0x62, 0x63, 0x64, 0x61, 0x5c, 0x75, 0x65, 0x66,
  0x34, 0x41, 0x5c, 0x62, 0x5c, 0x66, 0x5c, 0x6e, 0x5c, 0x72, 0x5c, 0x74,
  0x60, 0x31, 0x7e, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28,
  0x29, 0x5f, 0x2b, 0x2d, 0x3d, 0x5b, 0x5d, 0x7b, 0x7d, 0x7c, 0x3b, 0x3a,
  0x27, 0x2c, 0x2e, 0x2f, 0x3c, 0x3e, 0x3f, 0x22, 0x0a, 0x3a, 0x20, 0x22,
  0x41, 0x20, 0x6b, 0x65, 0x79, 0x20, 0x63, 0x61, 0x6e, 0x20, 0x62, 0x65,
  0x20, 0x61, 0x6e, 0x79, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x22,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x30, 0x2e, 0x35, 0x20, 0x2c, 0x39, 0x38, 0x2e, 0x36, 0x0a, 0x2c, 0x0a,
  0x39, 0x39, 0x2e, 0x34, 0x34, 0x0a, 0x2c, 0x0a, 0x0a, 0x31, 0x30, 0x36,
  0x36, 0x2c, 0x0a, 0x31, 0x65, 0x31, 0x2c, 0x0a, 0x30, 0x2e, 0x31, 0x65,
  0x31, 0x2c, 0x0a, 0x31, 0x65, 0x2d, 0x31, 0x2c, 0x0a, 0x31, 0x65, 0x30,
  0x30, 0x2c, 0x32, 0x65, 0x2b, 0x30, 0x30, 0x2c, 0x32, 0x65, 0x2d, 0x30,
  0x30, 0x0a, 0x2c, 0x22, 0x72, 0x6f, 0x73, 0x65, 0x62, 0x75, 0x64, 0x22,
  0x5d
};
unsigned int pass1_json_len = 1441;

/*----------------------------------------------------------------------
|       main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_Json* json = NULL;
    ATX_Json* child = NULL;
    ATX_String buffer = ATX_EMPTY_STRING;
    
    ATX_COMPILER_UNUSED(argc);
    ATX_COMPILER_UNUSED(argv);

    SHOULD_FAIL(ATX_Json_Parse("hello", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"hello", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"hello\\i\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"hello\\u\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"hello\\ubcdh\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"hello\\u000\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("a", &json));
    SHOULD_FAIL(ATX_Json_Parse("123z", &json));
    SHOULD_FAIL(ATX_Json_Parse("\"a\":3", &json));
    SHOULD_FAIL(ATX_Json_Parse(",", &json));
    SHOULD_FAIL(ATX_Json_Parse("1 2", &json));
    SHOULD_FAIL(ATX_Json_Parse("1,", &json));
    SHOULD_FAIL(ATX_Json_Parse("[],", &json));
    SHOULD_FAIL(ATX_Json_Parse("{},", &json));
    SHOULD_FAIL(ATX_Json_Parse("[,]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[1,]", &json));
    SHOULD_FAIL(ATX_Json_Parse("{1}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{,}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{1,}", &json));

    /* adapted from json.org test suite */
    SHOULD_FAIL(ATX_Json_Parse("[\"Unclosed array\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("{unquoted_key: \"keys must be quoted\"}", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"extra comma\",]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"double extra comma\",,]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[   , \"<-- missing value\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Comma after the close\"],", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Extra close\"]]", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Extra comma\": true,}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Extra value after close\": true} \"misplaced quoted value\"", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Illegal expression\": 1 + 2}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Illegal invocation\": alert()}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Numbers cannot have leading zeroes\": 013}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Numbers cannot be hex\": 0x14}", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Illegal backslash escape: \\x15\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\\naked]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Illegal backslash escape: \\017\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Missing colon\" null}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Double colon\":: null}", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Comma instead of colon\", null}", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Colon instead of comma\": false]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"Bad value\", truth]", &json));
    SHOULD_FAIL(ATX_Json_Parse("['single quote']", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"	tab	character	in	string	\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"tab\\   character\\   in\\  string\\  \"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"line\nbreak\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"line\\\nbreak\"]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[0e]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[0e+]", &json));
    SHOULD_FAIL(ATX_Json_Parse("[0e+-1]", &json));
    SHOULD_FAIL(ATX_Json_Parse("{\"Comma instead if closing brace\": true,", &json));
    SHOULD_FAIL(ATX_Json_Parse("[\"mismatch\"}", &json));

    SHOULD_SUCCEED(ATX_Json_Parse("[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_ARRAY);
    ATX_Json_Destroy(json);
        
    SHOULD_SUCCEED(ATX_Json_ParseBuffer(pass1_json, pass1_json_len, &json));
    CHECK(json != NULL);
    SHOULD_SUCCEED(ATX_Json_Serialize(json, &buffer, ATX_TRUE));
    printf("%s\n", ATX_CSTR(buffer));
    SHOULD_SUCCEED(ATX_Json_Serialize(json, &buffer, ATX_FALSE));
    printf("%s\n", ATX_CSTR(buffer));
    
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_ARRAY);
    CHECK(ATX_Json_GetChildCount(json) == 20);

    child = ATX_Json_GetChildAt(json, 0, NULL);
    CHECK(child != NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(child), "JSON Test Pattern pass1", ATX_FALSE));

    child = ATX_Json_GetChildAt(json, 1, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_OBJECT);
    CHECK(ATX_Json_GetChildCount(child) == 1);
    child = ATX_Json_GetChild(child, "object with 1 member");
    CHECK(child != NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_ARRAY);
    CHECK(ATX_Json_GetChildCount(child) == 1);
    child = ATX_Json_GetChildAt(child, 0, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(child), "array with 1 element", ATX_FALSE));
    child = ATX_Json_GetParent(child);
    
    child = ATX_Json_GetChildAt(json, 2, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_OBJECT);
    CHECK(ATX_Json_GetChildCount(child) == 0);

    child = ATX_Json_GetChildAt(json, 3, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_ARRAY);
    CHECK(ATX_Json_GetChildCount(child) == 0);

    child = ATX_Json_GetChildAt(json, 4, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsInteger(child) == -42);

    child = ATX_Json_GetChildAt(json, 5, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_BOOLEAN);
    CHECK(ATX_Json_AsBoolean(child) == ATX_TRUE);

    child = ATX_Json_GetChildAt(json, 6, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_BOOLEAN);
    CHECK(ATX_Json_AsBoolean(child) == ATX_FALSE);

    child = ATX_Json_GetChildAt(json, 7, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_NULL);

    child = ATX_Json_GetChildAt(json, 8, NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_OBJECT);
    CHECK(ATX_Json_GetChildCount(child) == 32);
    child = ATX_Json_GetChild(child, "integer");
    CHECK(child != NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsDouble(child) == 1234567890.0);
    
    child = ATX_Json_GetParent(child);


    ATX_Json_Destroy(json);
        
    /* misc tests */
    SHOULD_SUCCEED(ATX_Json_Parse("\"hello\"", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(json), "hello", ATX_FALSE));
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("\"\\\"hello\\\"\\nbye\\\\\"", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(json), "\"hello\"\nbye\\", ATX_FALSE));
    ATX_Json_Destroy(json);
    
    SHOULD_SUCCEED(ATX_Json_Parse("\"hello\\u03C0\"", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(json), "hello\xCF\x80", ATX_FALSE));
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("12345", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsInteger(json) == 12345);
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("-12345", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsInteger(json) == -12345);
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("1.2345", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsDouble(json) == 1.2345);
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("-1.2345", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsDouble(json) == -1.2345);
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("[]", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_ARRAY);
    CHECK(ATX_Json_GetChildCount(json) == 0);
    ATX_Json_Destroy(json);

    SHOULD_SUCCEED(ATX_Json_Parse("[1,\"a\"]", &json));
    CHECK(json != NULL);
    CHECK(ATX_Json_GetType(json) == ATX_JSON_TYPE_ARRAY);
    CHECK(ATX_Json_GetChildCount(json) == 2);
    child = ATX_Json_GetChildAt(json, 0, NULL);
    CHECK(child != NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_NUMBER);
    CHECK(ATX_Json_AsInteger(child) == 1);
    child = ATX_Json_GetChildAt(json, 1, NULL);
    CHECK(child != NULL);
    CHECK(ATX_Json_GetType(child) == ATX_JSON_TYPE_STRING);
    CHECK(ATX_String_Equals(ATX_Json_AsString(child), "a", ATX_FALSE));
    ATX_Json_Destroy(json);

    return 0;
}

