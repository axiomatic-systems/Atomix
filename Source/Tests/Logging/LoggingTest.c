/*****************************************************************
|
|   File: LoggingTest.c
|
|   Atomix Tests - Logging Test
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|  includes
+---------------------------------------------------------------------*/
#include "Atomix.h"

ATX_DEFINE_LOGGER(MyLogger, "atomix.test.logger")
ATX_SET_LOCAL_LOGGER("atomix.test.other")
ATX_DEFINE_LOGGER(FooLogger, "atomix.foo")

/*----------------------------------------------------------------------
|  main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_COMPILER_UNUSED(argc);
    ATX_COMPILER_UNUSED(argv);

    ATX_LOG_L(MyLogger, ATX_LOG_LEVEL_WARNING, "blabla");
    ATX_LOG_L2(MyLogger, ATX_LOG_LEVEL_WARNING, "blabla %d %d", 8, 9);

    ATX_LOG(ATX_LOG_LEVEL_WARNING, "blibli");
    ATX_LOG_2(ATX_LOG_LEVEL_INFO, "fofo %d %d", 5, 7);

    ATX_LOG_SEVERE("this is severe!");
    ATX_LOG_SEVERE_1("this is severe (%d)", 9);

    ATX_LOG_SEVERE_L(MyLogger, "this is severe!");
    ATX_LOG_SEVERE_L1(MyLogger, "this is severe (%d)", 9);

    ATX_LOG_SEVERE_L(FooLogger, "this is severe!");
    ATX_LOG_SEVERE_L1(FooLogger, "this is severe (%d)", 9);

    return 0;
}

