/*****************************************************************
|
|   Atomix - Common Definitions
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Header file for common preprocessor definitions
 */

#ifndef _ATX_DEFS_H_
#define _ATX_DEFS_H_

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#ifndef __cplusplus
#ifndef NULL
#define NULL ((void*)0)
#endif
#endif /* __cplusplus */

/*----------------------------------------------------------------------
|   calling conventions
+---------------------------------------------------------------------*/
#define ATX_PROCEDURE ATX_Result
#define ATX_EXPORTED_PROCEDURE ATX_Result

/**
 * used for standard method return type 
 */
#define ATX_METHOD static ATX_Result

#define ATX_DIRECT_METHOD ATX_Result
#define ATX_VOID_METHOD static void 
#define ATX_EXPORTED_METHOD ATX_Result

#endif /* _ATX_DEFS_H_ */
