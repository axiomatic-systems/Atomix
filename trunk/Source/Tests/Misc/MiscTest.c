/*****************************************************************
|
|      File: MiscTest.c
|
|      Atomix Tests - Misc
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "Atomix.h"

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#define SHOULD_SUCCEED(r, m)                    \
    do {                                        \
        if (ATX_FAILED(r)) {                    \
            ATX_Debug("%s failed (%d)\n", m, r);\
            exit(1);                            \
        }                                       \
    } while(0)                                  \

#define SHOULD_FAIL(r, m)                       \
    do {                                        \
        if (ATX_SUCCEEDED(r)) {                 \
            ATX_Debug("%s should have failed (%d)\n", m, r);\
            exit(1);                            \
        }                                       \
    } while(0)                                  \

#define SHOULD_EQUAL_I(a, b, m)                       \
    do {                                        \
        if ((a) != (b)) {                 \
            ATX_Debug("got %l, expected %l in %s\n", a, b, m);\
            exit(1);                            \
        }                                       \
    } while(0)                                  \

#define SHOULD_EQUAL_F(a, b, m)                       \
    do {                                        \
        if ((a) != (b)) {                 \
            ATX_Debug("got %f, expected %f in %s\n", a, b, m);\
            exit(1);                            \
        }                                       \
    } while(0)                                  \

/*----------------------------------------------------------------------
|       main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    float f;
    long  i;

    SHOULD_FAIL(ATX_ParseInteger("ssdfsdf", &i, ATX_FALSE), "test a1");
    SHOULD_FAIL(ATX_ParseInteger("", &i, ATX_FALSE), "test a2");
    SHOULD_FAIL(ATX_ParseInteger(NULL, &i, ATX_FALSE), "test a3");
    SHOULD_FAIL(ATX_ParseInteger("123a", &i, ATX_FALSE), "test a4");
    SHOULD_FAIL(ATX_ParseInteger("a123", &i, ATX_FALSE), "test a5");
    SHOULD_FAIL(ATX_ParseInteger(" 123", &i, ATX_FALSE), "test a6");
    SHOULD_FAIL(ATX_ParseInteger("a 123", &i, ATX_TRUE), "test a7");
    SHOULD_FAIL(ATX_ParseInteger(" a123", &i, ATX_TRUE), "test a8");

    SHOULD_SUCCEED(ATX_ParseInteger("+1", &i, ATX_FALSE), "test b1");
    SHOULD_EQUAL_I(i, 1, "test b1");
    SHOULD_SUCCEED(ATX_ParseInteger("+123", &i, ATX_FALSE), "test b2");
    SHOULD_EQUAL_I(i, 123, "test b2");
    SHOULD_SUCCEED(ATX_ParseInteger("-1", &i, ATX_FALSE), "test b3");
    SHOULD_EQUAL_I(i, -1, "test b3");
    SHOULD_SUCCEED(ATX_ParseInteger("-123", &i, ATX_FALSE), "test b4");
    SHOULD_EQUAL_I(i, -123, "test b4");
    SHOULD_SUCCEED(ATX_ParseInteger("-123fgs", &i, ATX_TRUE), "test b5");
    SHOULD_EQUAL_I(i, -123, "test b5");
    SHOULD_SUCCEED(ATX_ParseInteger("  -123fgs", &i, ATX_TRUE), "test b6");
    SHOULD_EQUAL_I(i, -123, "b6");
    SHOULD_SUCCEED(ATX_ParseInteger("0", &i, ATX_TRUE), "test b7");
    SHOULD_EQUAL_I(i, 0, "b7");
    SHOULD_SUCCEED(ATX_ParseInteger("7768", &i, ATX_TRUE), "test b8");
    SHOULD_EQUAL_I(i, 7768, "b8");

    SHOULD_FAIL(ATX_ParseFloat("ssdfsdf", &f, ATX_FALSE), "test c1");
    SHOULD_FAIL(ATX_ParseFloat("", &f, ATX_FALSE), "test c2");
    SHOULD_FAIL(ATX_ParseFloat(NULL, &f, ATX_FALSE), "test c3");
    SHOULD_FAIL(ATX_ParseFloat("123.", &f, ATX_FALSE), "test c4");
    SHOULD_FAIL(ATX_ParseFloat("a123", &f, ATX_FALSE), "test c5");
    SHOULD_FAIL(ATX_ParseFloat(" 123", &f, ATX_FALSE), "test c6");
    SHOULD_FAIL(ATX_ParseFloat(" 127.89E5ff", &f, ATX_FALSE), "test c7");

    SHOULD_SUCCEED(ATX_ParseFloat("+1.0", &f, ATX_FALSE), "test d1");
    SHOULD_EQUAL_F(f, 1.0f, "test d1");
    SHOULD_SUCCEED(ATX_ParseFloat("+123", &f, ATX_FALSE), "test d2");
    SHOULD_EQUAL_F(f, 123.0f, "test d2");
    SHOULD_SUCCEED(ATX_ParseFloat("-0.1", &f, ATX_FALSE), "test d3");
    SHOULD_EQUAL_F(f, -0.1f, "test d3");
    SHOULD_SUCCEED(ATX_ParseFloat("0.23e-13", &f, ATX_FALSE), "test d4");
    SHOULD_EQUAL_F(f, 0.23e-13f, "test d4");
    SHOULD_SUCCEED(ATX_ParseFloat(" 127.89E5ff", &f, ATX_TRUE), "test d5");
    SHOULD_EQUAL_F(f, 127.89E5f, "test d5");
    SHOULD_SUCCEED(ATX_ParseFloat("+0.3db", &f, ATX_TRUE), "test d6");
    SHOULD_EQUAL_F(f, 0.3f, "d6");
    SHOULD_SUCCEED(ATX_ParseFloat("+.3db", &f, ATX_TRUE), "test d7");
    SHOULD_EQUAL_F(f, 0.3f, "d7");
    SHOULD_SUCCEED(ATX_ParseFloat("-.3db", &f, ATX_TRUE), "test d8");
    SHOULD_EQUAL_F(f, -0.3f, "d8");
    SHOULD_SUCCEED(ATX_ParseFloat(".3db", &f, ATX_TRUE), "test d9");
    SHOULD_EQUAL_F(f, .3f, "d9");

    return 0;
}

