/*****************************************************************
|
|   Atomix - Common Definitions
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Header file for debug logging
 */

#ifndef _ATX_DEBUG_H_
#define _ATX_DEBUG_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#if defined(ATX_CONFIG_HAVE_ASSERT_H) && defined(ATX_DEBUG)
#include <assert.h>
#define ATX_ASSERT(_x) assert(_x)
#else
#define ATX_ASSERT(_x) ((void)0)
#endif

/*----------------------------------------------------------------------
|   prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void ATX_Debug(const char* format, ...);
extern void ATX_DebugOutput(const char* message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_DEBUG_H_ */
