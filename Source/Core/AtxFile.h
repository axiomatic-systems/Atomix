/*****************************************************************
|
|   File: AtxFile.h
|
|   Atomix - File Storage
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_FILE_H_
#define _ATX_FILE_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxStreams.h"
#include "AtxDataBuffer.h"

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define ATX_FILE_OPEN_MODE_READ       0x01
#define ATX_FILE_OPEN_MODE_WRITE      0x02
#define ATX_FILE_OPEN_MODE_CREATE     0x04
#define ATX_FILE_OPEN_MODE_TRUNCATE   0x08
#define ATX_FILE_OPEN_MODE_UNBUFFERED 0x10

#define ATX_ERROR_NO_SUCH_FILE       (ATX_ERROR_BASE_FILE - 0)
#define ATX_ERROR_FILE_NOT_OPEN      (ATX_ERROR_BASE_FILE - 1)
#define ATX_ERROR_FILE_BUSY          (ATX_ERROR_BASE_FILE - 2)
#define ATX_ERROR_FILE_ALREADY_OPEN  (ATX_ERROR_BASE_FILE - 3)
#define ATX_ERROR_FILE_NOT_READABLE  (ATX_ERROR_BASE_FILE - 4)
#define ATX_ERROR_FILE_NOT_WRITABLE  (ATX_ERROR_BASE_FILE - 5)

#define ATX_FILE_STANDARD_INPUT  "@STDIN"
#define ATX_FILE_STANDARD_OUTPUT "@STDOUT"
#define ATX_FILE_STANDARD_ERROR  "@STDERR"

/*----------------------------------------------------------------------
|   ATX_InputStream
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_File)
ATX_BEGIN_INTERFACE_DEFINITION(ATX_File)
    ATX_Result (*Open)(ATX_File* self, ATX_Flags mode);
    ATX_Result (*Close)(ATX_File* self);
    ATX_Result (*GetSize)(ATX_File* self, ATX_Size* size);
    ATX_Result (*GetInputStream)(ATX_File* self, ATX_InputStream** stream);
    ATX_Result (*GetOutputStream)(ATX_File* self, ATX_OutputStream**  stream);
ATX_END_INTERFACE_DEFINITION

/*----------------------------------------------------------------------
|   convenience macros
+---------------------------------------------------------------------*/
#define ATX_File_Open(object, mode) \
ATX_INTERFACE(object)->Open(object, mode)

#define ATX_File_Close(object) \
ATX_INTERFACE(object)->Close(object)

#define ATX_File_GetSize(object, size) \
ATX_INTERFACE(object)->GetSize(object, size)

#define ATX_File_GetInputStream(object, stream) \
ATX_INTERFACE(object)->GetInputStream(object, stream)

#define ATX_File_GetOutputStream(object, stream) \
ATX_INTERFACE(object)->GetOutputStream(object, stream)

/*----------------------------------------------------------------------
|   prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern ATX_Result ATX_File_Create(ATX_CString name, ATX_File** file);
extern ATX_Result ATX_File_Load(ATX_File* file, ATX_DataBuffer** buffer);

/* helper functions */
extern ATX_Result ATX_LoadFile(ATX_CString filename, ATX_DataBuffer** buffer);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _ATX_FILE_H_ */


