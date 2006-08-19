/*****************************************************************
|
|   Atomix - Time
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Atomix Time Interface Header file
 */

#ifndef _ATX_TIME_H_
#define _ATX_TIME_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxTypes.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Int32 seconds;
    ATX_Int32 nanoseconds;
} ATX_TimeStamp;

typedef ATX_TimeStamp ATX_Time;
typedef ATX_TimeStamp ATX_TimeInterval;

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
#define ATX_TimeStamp_Set(t, s, n) \
    do {(t).seconds = (s); (t).nanoseconds = (n);} while(0)

#define ATX_TimeStamp_IsLater(t0,t1)           \
(                                              \
    ((t0).seconds > (t1).seconds) ||    \
    (                                          \
        (t0).seconds == (t1).seconds &&        \
        (t0).nanoseconds > (t1).nanoseconds    \
    )                                          \
)

#define ATX_TimeStamp_IsLaterOrEqual(t0,t1)   \
(                                             \
    ((t0).seconds > (t1).seconds) ||   \
    (                                         \
        (t0).seconds == (t1).seconds &&       \
        (t0).nanoseconds >= (t1).nanoseconds  \
    )                                         \
)

#define ATX_TimeStamp_Add(t0,t1,t2) do {                        \
    (t0).seconds = (t1).seconds + (t2).seconds;                 \
    (t0).nanoseconds = (t1).nanoseconds + (t2).nanoseconds;     \
    if ((t0).nanoseconds > 1000000000) {                        \
        (t0).seconds++;                                         \
        (t0).nanoseconds -= 1000000000;                         \
    }                                                           \
} while (0)

#define ATX_TimeStamp_Sub(t0,t1,t2) do {                        \
    (t0).seconds = (t1).seconds - (t2).seconds;                 \
    (t0).nanoseconds = (t1).nanoseconds - (t2).nanoseconds;     \
    if ((t0).nanoseconds < 0) {                                 \
        (t0).seconds--;                                         \
        (t0).nanoseconds += 1000000000;                         \
    }                                                           \
} while (0)

#define ATX_TimeStamp_ToInt64(t, i) do {                        \
    ATX_Int64_Set_Int32(i, t.seconds);                          \
    ATX_Int64_Mul_Int32(i, 1000000000);                         \
    ATX_Int64_Add_Int32(i, t.nanoseconds);                      \
} while (0)

#endif /* _ATX_TIME_H_ */
