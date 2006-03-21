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
ATX_SET_LOCAL_LOGGER("atomix.test.other");


/*----------------------------------------------------------------------
|  main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_COMPILER_UNUSED(argc);
    ATX_COMPILER_UNUSED(argv);

    ATX_LOG_L(MyLogger, 100, "blabla");
    ATX_LOG_L2(MyLogger, 100, "blabla %d %d", 8, 9);

    ATX_LOG(100, "blibli");
    ATX_LOG_2(200, "fofo %d %d", 5, 7);

    ATX_LOG_SEVERE("this is severe!");
    ATX_LOG_SEVERE_1("this is severe (%d)", 9);

    ATX_LOG_SEVERE_L(MyLogger, "this is severe!");
    ATX_LOG_SEVERE_L1(MyLogger, "this is severe (%d)", 9);

    return 0;
}

