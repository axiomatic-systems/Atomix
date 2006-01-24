/*****************************************************************
|
|      File: AtxStdcFile.c
|
|      Atomix - Files: StdC Implementation
|
|      (c) 2002-2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#include "AtxUtils.h"
#include "AtxStreams.h"
#include "AtxFile.h"
#include "AtxErrors.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal reference_count;
    FILE*        file;
    ATX_Offset   position;
    ATX_Size     size;
} StdcFileWrapper;

typedef struct {
    ATX_Cardinal     reference_count;
    StdcFileWrapper* file;
} StdcFileStream;

typedef struct {
    ATX_String     name;
    ATX_Size       size;
    ATX_Flags      mode;
    StdcFileWrapper* file;
} StdcFile;

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFileStream)
static const ATX_InputStreamInterface StdcFileStream_ATX_InputStreamInterface;
static const ATX_OutputStreamInterface StdcFileStream_ATX_OutputStreamInterface;

/*----------------------------------------------------------------------
|       StdcFileWrapper_Create
+---------------------------------------------------------------------*/
static ATX_Result
StdcFileWrapper_Create(FILE*             file, 
                       ATX_Size          size,
                       StdcFileWrapper** wrapper)
{
    /* allocate a new object */
    (*wrapper) = ATX_AllocateZeroMemory(sizeof(StdcFileWrapper));
    if (*wrapper == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*wrapper)->file            = file;
    (*wrapper)->position        = 0;
    (*wrapper)->size            = size;
    (*wrapper)->reference_count = 1;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileWrapper_Destroy
+---------------------------------------------------------------------*/
static void
StdcFileWrapper_Destroy(StdcFileWrapper* wrapper)
{
    if (wrapper->file != NULL   &&
        wrapper->file != stdin  &&
        wrapper->file != stdout &&
        wrapper->file != stderr) {
        fclose(wrapper->file);
    }
    ATX_FreeMemory((void*)wrapper);
}

/*----------------------------------------------------------------------
|       StdcFileWrapper_AddReference
+---------------------------------------------------------------------*/
static void
StdcFileWrapper_AddReference(StdcFileWrapper* wrapper)
{
    ++wrapper->reference_count;
}

/*----------------------------------------------------------------------
|       StdcFileWrapper_Release
+---------------------------------------------------------------------*/
static void
StdcFileWrapper_Release(StdcFileWrapper* wrapper)
{
    if (wrapper == NULL) return;
    if (--wrapper->reference_count == 0) {
        StdcFileWrapper_Destroy(wrapper);
    }
}

/*----------------------------------------------------------------------
|       StdcFileStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
StdcFileStream_Create(StdcFileWrapper* file, StdcFileStream** stream)
{
    /* create a new object */
    (*stream) = (StdcFileStream*)ATX_AllocateMemory(sizeof(StdcFileStream));
    if (*stream == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*stream)->reference_count = 1;
    (*stream)->file = file;

    /* keep a reference */
    StdcFileWrapper_AddReference(file);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
StdcFileStream_Destroy(StdcFileStream* stream)
{
    StdcFileWrapper_Release(stream->file);
    ATX_FreeMemory((void*)stream);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileStream_Seek(StdcFileStream* stream, 
                    ATX_Offset      where)
{
    if (fseek(stream->file->file, where, SEEK_SET) == 0) {
        stream->file->position = where;
        return ATX_SUCCESS;
    } else {
        return ATX_FAILURE;
    }
}

/*----------------------------------------------------------------------
|       StdcFileStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileStream_Tell(StdcFileStream* stream, 
                    ATX_Offset*     where)
{
    if (where) *where = stream->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
StdcFileInputStream_Create(StdcFileWrapper* file, 
                           ATX_InputStream* stream)
{
    StdcFileStream* file_stream = NULL;
    ATX_Result      result;

    /* create the object */
    result = StdcFileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_InputStreamInstance*)file_stream;
    ATX_INTERFACE(stream) = &StdcFileStream_ATX_InputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileInputStream_Read(ATX_InputStreamInstance* instance,
                         ATX_Any                  buffer, 
                         ATX_Size                 bytes_to_read, 
                         ATX_Size*                bytes_read)
{
    StdcFileStream* stream = (StdcFileStream*)instance;
    size_t          nb_read;

    nb_read = fread(buffer, 1, (size_t)bytes_to_read, stream->file->file);
    if (nb_read > 0 || bytes_to_read == 0) {
        if (bytes_read) *bytes_read = nb_read;
        stream->file->position += nb_read;
        return ATX_SUCCESS;
    } else {
        if (bytes_read) *bytes_read = 0;
        if (nb_read == 0 || feof(stream->file->file) != 0) {
            return ATX_ERROR_EOS;
        } else {
            return ATX_FAILURE;
        }
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileInputStream_Seek(ATX_InputStreamInstance* instance, 
                         ATX_Offset               where)
{
    return StdcFileStream_Seek((StdcFileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileInputStream_Tell(ATX_InputStreamInstance* instance, 
                         ATX_Offset*              where)
{
    return StdcFileStream_Tell((StdcFileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileInputStream_GetSize(ATX_InputStreamInstance* instance, 
                            ATX_Size*                size)
{
    StdcFileStream* stream = (StdcFileStream*)instance;
    *size = stream->file->size;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileInputStream_GetAvailable(ATX_InputStreamInstance* instance, 
                                 ATX_Size*                size)
{
    StdcFileStream* stream = (StdcFileStream*)instance;
    *size = stream->file->size - stream->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_InputStream interface
+---------------------------------------------------------------------*/
static const ATX_InputStreamInterface
StdcFileStream_ATX_InputStreamInterface = {
    StdcFileStream_GetInterface,
    StdcFileInputStream_Read,
    StdcFileInputStream_Seek,
    StdcFileInputStream_Tell,
    StdcFileInputStream_GetSize,
    StdcFileInputStream_GetAvailable
};

/*----------------------------------------------------------------------
|       StdcFileOutputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
StdcFileOutputStream_Create(StdcFileWrapper*  file, 
                            ATX_OutputStream* stream)
{
    StdcFileStream* file_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = StdcFileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_OutputStreamInstance*)file_stream;
    ATX_INTERFACE(stream) = &StdcFileStream_ATX_OutputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileOutputStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileOutputStream_Write(ATX_OutputStreamInstance* instance,
                           ATX_AnyConst              buffer, 
                           ATX_Size                  bytes_to_write, 
                           ATX_Size*                 bytes_written)
{
    StdcFileStream* stream = (StdcFileStream*)instance;
    size_t          nb_written;

    nb_written = fwrite(buffer, 1, (size_t)bytes_to_write, stream->file->file);
    if (nb_written > 0 || bytes_to_write == 0) {
        if (bytes_written) *bytes_written = nb_written;
        stream->file->position += nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        return ATX_FAILURE;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFileOutputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileOutputStream_Seek(ATX_OutputStreamInstance* instance, 
                          ATX_Offset                where)
{
    return StdcFileStream_Seek((StdcFileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       StdcFileOutputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileOutputStream_Tell(ATX_OutputStreamInstance* instance, 
                          ATX_Offset*               where)
{
    return StdcFileStream_Tell((StdcFileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       StdcFileOutputStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFileOutputStream_Flush(ATX_OutputStreamInstance* instance)
{
    ATX_COMPILER_UNUSED(instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_OutputStream interface
+---------------------------------------------------------------------*/
static const ATX_OutputStreamInterface
StdcFileStream_ATX_OutputStreamInterface = {
    StdcFileStream_GetInterface,
    StdcFileOutputStream_Write,
    StdcFileOutputStream_Seek,
    StdcFileOutputStream_Tell,
    StdcFileOutputStream_Flush
};

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_REFERENCEABLE_INTERFACE(StdcFileStream, reference_count)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFileStream) 
ATX_INTERFACE_MAP_ADD(StdcFileStream, ATX_InputStream)
ATX_INTERFACE_MAP_ADD(StdcFileStream, ATX_OutputStream)
ATX_INTERFACE_MAP_ADD(StdcFileStream, ATX_Referenceable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFileStream)

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFile)
static const ATX_FileInterface StdcFile_ATX_FileInterface;

/*----------------------------------------------------------------------
|       ATX_File_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Create(const char* filename, ATX_File* object)
{
    StdcFile* file;

    /* allocate a new object */
    file = (StdcFile*)ATX_AllocateZeroMemory(sizeof(StdcFile));
    if (file == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    ATX_SET_STRING(file->name, filename);

    /* get the size */
    if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_INPUT)  ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_OUTPUT) ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_ERROR)) {
        file->size = 0;
    } else {
        struct stat info;
        if (stat(filename, &info) == 0) {
            file->size = info.st_size;
        } else {
            file->size = 0;
        }
    }

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_FileInstance*)file;
    ATX_INTERFACE(object) = &StdcFile_ATX_FileInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFile_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFile_Destroy(ATX_DestroyableInstance* instance)
{
    StdcFile* file = (StdcFile*)instance;

    /* release the resources */
    ATX_DESTROY_STRING(file->name);
    StdcFileWrapper_Release(file->file);

    /* free the memory */
    ATX_FreeMemory((void*)instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFile_Open
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFile_Open(ATX_FileInstance* instance, ATX_Flags mode)
{ 
    StdcFile* file = (StdcFile*)instance;
    FILE*     stdc_file;

    /* decide wheter this is a file or stdin/stdout/stderr */
    if (!strcmp(file->name, "-stdin")) {
        stdc_file = stdin;
    } else if (!strcmp(file->name, "-stdout")) {
        stdc_file = stdout;
    } else if (!strcmp(file->name, "-stderr")) {
        stdc_file = stderr;
    } else {
        const char* fmode = "";

        /* compute mode */
        if (mode & ATX_FILE_OPEN_MODE_WRITE) {
            if (mode & ATX_FILE_OPEN_MODE_CREATE) {
                if (mode & ATX_FILE_OPEN_MODE_TRUNCATE) {
                    /* write, read, create, truncate */
                    fmode = "w+b";
                } else {
                    /* write, read, create */
                    fmode = "a+b";
                }
            } else {
                if (mode & ATX_FILE_OPEN_MODE_TRUNCATE) {
                    /* write, read, truncate */
                    fmode = "w+b";
                } else {
                    /* write, read */
                    fmode = "r+b";
                }
            }
        } else {
            /* read only */
            fmode = "rb";
        }

        /* try to open the file */
        stdc_file = fopen(file->name, fmode);
        if (stdc_file == NULL) {
            switch (errno) {
              case EACCES:
                return ATX_ERROR_ACCESS_DENIED;

              case ENOENT:
                return ATX_ERROR_NO_SUCH_FILE;

              default:
                return ATX_ERROR_ERRNO(errno);
            }
        }
    }

    /* set the buffered/unbuffered option */
    if (mode & ATX_FILE_OPEN_MODE_UNBUFFERED) {
        setbuf(stdc_file, NULL);
    }

    /* remember the mode */
    file->mode = mode;

    /* create a wrapper */
    return StdcFileWrapper_Create(stdc_file, file->size, &file->file);
}

/*----------------------------------------------------------------------
|       StdcFile_Close
+---------------------------------------------------------------------*/
static ATX_Result
StdcFile_Close(ATX_FileInstance* instance)
{
    StdcFile* file = (StdcFile*)instance;

    /* release the resources and reset */
    StdcFileWrapper_Release(file->file);
    file->file = NULL;
    file->mode = 0;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFile_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFile_GetSize(ATX_FileInstance* instance,
                 ATX_Size*         size)
{
    StdcFile* file = (StdcFile*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* return the size */
    if (size) *size = file->size;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       StdcFile_GetInputStream
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFile_GetInputStream(ATX_FileInstance* instance, 
                        ATX_InputStream*  stream)
{
    StdcFile* file = (StdcFile*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(file->mode & ATX_FILE_OPEN_MODE_READ)) {
        return ATX_ERROR_FILE_NOT_READABLE;
    }

    return StdcFileInputStream_Create(file->file, stream);
}

/*----------------------------------------------------------------------
|       StdcFile_GetOutputStream
+---------------------------------------------------------------------*/
ATX_METHOD
StdcFile_GetOutputStream(ATX_FileInstance* instance, 
                         ATX_OutputStream* stream)
{
    StdcFile* file = (StdcFile*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(file->mode & ATX_FILE_OPEN_MODE_WRITE)) {
        return ATX_ERROR_FILE_NOT_WRITABLE;
    }

    return StdcFileOutputStream_Create(file->file, stream);
}

/*----------------------------------------------------------------------
|       ATX_File interface
+---------------------------------------------------------------------*/
static const ATX_FileInterface
StdcFile_ATX_FileInterface = {
    StdcFile_GetInterface,
    StdcFile_Open,
    StdcFile_Close,
    StdcFile_GetSize,
    StdcFile_GetInputStream,
    StdcFile_GetOutputStream
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(StdcFile)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFile) 
ATX_INTERFACE_MAP_ADD(StdcFile, ATX_File)
ATX_INTERFACE_MAP_ADD(StdcFile, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(StdcFile)
