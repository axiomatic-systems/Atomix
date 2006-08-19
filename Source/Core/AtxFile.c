/*****************************************************************
|
|   Atomix - File Storage
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxFile.h"
#include "AtxResults.h"
#include "AtxTypes.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"

/*----------------------------------------------------------------------
|   ATX_File_Load
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Load(ATX_File* file, ATX_DataBuffer** buffer)
{
    ATX_InputStream* input = NULL;
    ATX_Result       result;

    /* get the input stream for the file */
    ATX_CHECK(ATX_File_GetInputStream(file, &input));

    /* read the stream */
    result = ATX_InputStream_Load(input, 0, buffer);

    /* release the stream */
    ATX_RELEASE_OBJECT(input);

    return result;
}

/*----------------------------------------------------------------------
|   ATX_File_Save
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Save(ATX_File* file, ATX_DataBuffer* buffer)
{
    ATX_OutputStream* output = NULL;
    ATX_Result        result;

    /* get the input stream for the file */
    ATX_CHECK(ATX_File_GetOutputStream(file, &output));

    /* read the stream */
    result = ATX_OutputStream_WriteFully(output, 
        ATX_DataBuffer_GetData(buffer),
        ATX_DataBuffer_GetDataSize(buffer));

    /* release the stream */
    ATX_RELEASE_OBJECT(output);

    return result;
}

/*----------------------------------------------------------------------
|   ATX_LoadFile
+---------------------------------------------------------------------*/
ATX_Result
ATX_LoadFile(ATX_CString filename, ATX_DataBuffer** buffer)
{
    ATX_File*  file;
    ATX_Result result;

    /* open the file */
    ATX_CHECK(ATX_File_Create(filename, &file));
    result = ATX_File_Open(file, ATX_FILE_OPEN_MODE_READ);
    if (ATX_FAILED(result)) {
        ATX_DESTROY_OBJECT(file);
        return result;
    }

    /* load the file */
    result = ATX_File_Load(file, buffer);

    /* close and destroy the file */
    ATX_File_Close(file);
    ATX_DESTROY_OBJECT(file);

    return result;
}

/*----------------------------------------------------------------------
|   ATX_SaveFile
+---------------------------------------------------------------------*/
ATX_Result
ATX_SaveFile(ATX_CString filename, ATX_DataBuffer* buffer)
{
    ATX_File*  file;
    ATX_Result result;

    /* open the file */
    ATX_CHECK(ATX_File_Create(filename, &file));
    result = ATX_File_Open(file, 
                           ATX_FILE_OPEN_MODE_CREATE   | 
                           ATX_FILE_OPEN_MODE_TRUNCATE | 
                           ATX_FILE_OPEN_MODE_WRITE);
    if (ATX_FAILED(result)) {
        ATX_DESTROY_OBJECT(file);
        return result;
    }

    /* load the file */
    result = ATX_File_Save(file, buffer);

    /* close and destroy the file */
    ATX_File_Close(file);
    ATX_DESTROY_OBJECT(file);

    return result;
}
