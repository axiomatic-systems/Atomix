/*****************************************************************
|
|      File: AtxWin32File.c
|
|      Atomix - File Streams: Win32 Implementation
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#define STRICT
#include <windows.h>

#include "AtxInterfaces.h"
#include "AtxUtils.h"
#include "AtxErrors.h"
#include "AtxStreams.h"
#include "AtxFile.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal reference_count;
    HANDLE       handle;
    ATX_Offset   position;
    ATX_Size     size;
} Win32FileHandleWrapper;

typedef struct {
    ATX_Cardinal            reference_count;
    Win32FileHandleWrapper* file;
} Win32FileStream;

typedef struct {
    ATX_CString             name;
    ATX_Size                size;
    ATX_Flags               mode;
    Win32FileHandleWrapper* file;
} Win32File;

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32FileStream)
static const ATX_InputStreamInterface Win32FileStream_ATX_InputStreamInterface;
static const ATX_OutputStreamInterface Win32FileStream_ATX_OutputStreamInterface;

/*----------------------------------------------------------------------
|       Win32FileHandleWrapper_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileHandleWrapper_Create(HANDLE                   handle, 
                              ATX_Size                 size,
                              Win32FileHandleWrapper** wrapper)
{
    /* allocate a new object */
    (*wrapper) = ATX_AllocateZeroMemory(sizeof(Win32FileHandleWrapper));
    if (*wrapper == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*wrapper)->handle          = handle;
    (*wrapper)->position        = 0;
    (*wrapper)->size            = size;
    (*wrapper)->reference_count = 1;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileHandleWrapper_Destroy
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_Destroy(Win32FileHandleWrapper* wrapper)
{
    if (wrapper->handle != GetStdHandle(STD_INPUT_HANDLE) &&
        wrapper->handle != GetStdHandle(STD_OUTPUT_HANDLE) &&
        wrapper->handle != GetStdHandle(STD_ERROR_HANDLE)) {
        CloseHandle(wrapper->handle);
    }
    ATX_FreeMemory((void*)wrapper);
}

/*----------------------------------------------------------------------
|       Win32FileHandleWrapper_AddReference
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_AddReference(Win32FileHandleWrapper* wrapper)
{
    ++wrapper->reference_count;
}

/*----------------------------------------------------------------------
|       Win32FileHandleWrapper_Release
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_Release(Win32FileHandleWrapper* wrapper)
{
    if (wrapper == NULL) return;
    if (--wrapper->reference_count == 0) {
        Win32FileHandleWrapper_Destroy(wrapper);
    }
}

/*----------------------------------------------------------------------
|       Win32FileStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileStream_Create(Win32FileHandleWrapper* file, Win32FileStream** stream)
{
    /* create a new object */
    (*stream) = (Win32FileStream*)ATX_AllocateMemory(sizeof(Win32FileStream));
    if (*stream == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*stream)->reference_count = 1;
    (*stream)->file = file;

    /* keep a reference */
    Win32FileHandleWrapper_AddReference(file);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileStream_Destroy(Win32FileStream* stream)
{
    Win32FileHandleWrapper_Release(stream->file);
    ATX_FreeMemory((void*)stream);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileStream_Seek(Win32FileStream* stream, ATX_Offset where)
{
    SetFilePointer(stream->file->handle, where, 0, FILE_BEGIN);
    stream->file->position = where;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileStream_Tell(Win32FileStream* stream, ATX_Offset* where)
{
    if (where) *where = stream->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileInputStream_Create(Win32FileHandleWrapper* file, 
                            ATX_InputStream*        stream)
{
    Win32FileStream* file_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = Win32FileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_InputStreamInstance*)file_stream;
    ATX_INTERFACE(stream) = &Win32FileStream_ATX_InputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Read(ATX_InputStreamInstance* instance,
                          ATX_Any                  buffer,
                          ATX_Size                 bytes_to_read,
                          ATX_Size*                bytes_read)
{
    Win32FileStream* stream = (Win32FileStream*)instance;
    DWORD            nb_read;
    BOOL             result;

    result = ReadFile(stream->file->handle, 
                      buffer, 
                      bytes_to_read, 
                      &nb_read, 
                      NULL);
    if (result == TRUE) {
        if (bytes_read) *bytes_read = nb_read;
        stream->file->position += nb_read;
        if (nb_read == 0) {
            return ATX_ERROR_EOS;
        } else {
            return ATX_SUCCESS;
        }
    } else {
        if (bytes_read) *bytes_read = 0;
        if (GetLastError() == ERROR_HANDLE_EOF ) {
            return ATX_ERROR_EOS;
        } else {
            return ATX_FAILURE;
        }
    }
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Seek(ATX_InputStreamInstance* instance, 
                          ATX_Offset               where)
{
    return Win32FileStream_Seek((Win32FileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Tell(ATX_InputStreamInstance* instance, 
                          ATX_Offset*              where)
{
    return Win32FileStream_Tell((Win32FileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_GetSize(ATX_InputStreamInstance* instance, 
                             ATX_Size*                size)
{
    Win32FileStream* stream = (Win32FileStream*)instance;
    *size = stream->file->size;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_GetAvailable(ATX_InputStreamInstance* instance, 
                                  ATX_Size*                size)
{
    Win32FileStream* stream = (Win32FileStream*)instance;
    *size = stream->file->size - stream->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_InputStream interface
+---------------------------------------------------------------------*/
static const ATX_InputStreamInterface
Win32FileStream_ATX_InputStreamInterface = {
    Win32FileStream_GetInterface,
    Win32FileInputStream_Read,
    Win32FileInputStream_Seek,
    Win32FileInputStream_Tell,
    Win32FileInputStream_GetSize,
    Win32FileInputStream_GetAvailable
};

/*----------------------------------------------------------------------
|       Win32FileOutputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileOutputStream_Create(Win32FileHandleWrapper* file, 
                             ATX_OutputStream*       stream)
{
    Win32FileStream* file_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = Win32FileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        ATX_CLEAR_OBJECT(stream);
        return result;
    }

    /* set the interface */
    ATX_INSTANCE(stream) = (ATX_OutputStreamInstance*)file_stream;
    ATX_INTERFACE(stream) = &Win32FileStream_ATX_OutputStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32FileOutputStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Write(ATX_OutputStreamInstance* instance,
                            ATX_AnyConst              buffer, 
                            ATX_Size                  bytes_to_write, 
                            ATX_Size*                 bytes_written)
{
    Win32FileStream* stream = (Win32FileStream*)instance;
    DWORD       nb_written;
    BOOL        result;

    result = WriteFile(stream->file->handle, 
                       buffer, 
                       bytes_to_write, 
                       &nb_written, 
                       NULL);
    if (result == TRUE) {
        if (bytes_written) *bytes_written = nb_written;
        stream->file->position += nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        return ATX_FAILURE;
    }
}

/*----------------------------------------------------------------------
|       Win32FileOutputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Seek(ATX_OutputStreamInstance* instance, 
                           ATX_Offset                where)
{
    return Win32FileStream_Seek((Win32FileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       Win32FileOutputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Tell(ATX_OutputStreamInstance* instance, 
                           ATX_Offset*               where)
{
    return Win32FileStream_Tell((Win32FileStream*)instance, where);
}

/*----------------------------------------------------------------------
|       Win32FileOutputStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Flush(ATX_OutputStreamInstance* instance)
{
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ATX_OutputStream interface
+---------------------------------------------------------------------*/
static const ATX_OutputStreamInterface
Win32FileStream_ATX_OutputStreamInterface = {
    Win32FileStream_GetInterface,
    Win32FileOutputStream_Write,
    Win32FileOutputStream_Seek,
    Win32FileOutputStream_Tell,
    Win32FileOutputStream_Flush
};

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_REFERENCEABLE_INTERFACE(Win32FileStream, reference_count)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32FileStream) 
ATX_INTERFACE_MAP_ADD(Win32FileStream, ATX_InputStream)
ATX_INTERFACE_MAP_ADD(Win32FileStream, ATX_OutputStream)
ATX_INTERFACE_MAP_ADD(Win32FileStream, ATX_Referenceable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32FileStream)

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32File)
static const ATX_FileInterface Win32File_ATX_FileInterface;

/*----------------------------------------------------------------------
|       ATX_File_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Create(const char* filename, ATX_File* object)
{
    Win32File* file;

    /* allocate a new object */
    file = (Win32File*)ATX_AllocateZeroMemory(sizeof(Win32File));
    if (file == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    ATX_SET_CSTRING(file->name, filename);

    /* get the size */
    if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_INPUT)  ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_OUTPUT) ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_ERROR)) {
        file->size = 0;
    } else {
        WIN32_FIND_DATA info;
        HANDLE f = FindFirstFile(filename, &info);
        if (f == INVALID_HANDLE_VALUE) {
            file->size = 0;
        } else {
            file->size = info.nFileSizeLow;
            FindClose(f);
        }
    }

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_FileInstance*)file;
    ATX_INTERFACE(object) = &Win32File_ATX_FileInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32File_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Destroy(ATX_DestroyableInstance* instance)
{
    Win32File* file = (Win32File*)instance;

    /* release the resources */
    ATX_DESTROY_CSTRING(file->name);
    Win32FileHandleWrapper_Release(file->file);

    /* free the memory */
    ATX_FreeMemory((void*)instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32File_Open
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Open(ATX_FileInstance* instance, ATX_Flags mode)
{
    Win32File*  file = (Win32File*)instance;
    HANDLE      handle;
    DWORD       access_mode = 0;
	DWORD       share_mode  = FILE_SHARE_READ | FILE_SHARE_WRITE;
	DWORD       create_mode = 0;
    const char* filename = file->name;
    ATX_Result  result;

    /* compute modes */
    if (mode & ATX_FILE_OPEN_MODE_READ) {
        access_mode |= GENERIC_READ;
    }
    if (mode & ATX_FILE_OPEN_MODE_WRITE) {
        access_mode |= GENERIC_WRITE;
    }
    if (mode & ATX_FILE_OPEN_MODE_CREATE) {
        if (mode & ATX_FILE_OPEN_MODE_TRUNCATE) {
            create_mode |= CREATE_ALWAYS;
        } else {
            create_mode |= OPEN_ALWAYS;
        }
    } else {
        if (mode & ATX_FILE_OPEN_MODE_TRUNCATE) {
            create_mode |= TRUNCATE_EXISTING;
        } else {
		    create_mode |= OPEN_EXISTING;
        }
	}

    /* handle special names */
    if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_INPUT)) {
        handle = GetStdHandle(STD_INPUT_HANDLE);
    } else if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_OUTPUT)) {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
    } else if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_ERROR)) {
        handle = GetStdHandle(STD_ERROR_HANDLE);
    } else {
        /* try to open the file */
	    handle = CreateFile(filename, 
	                        access_mode, 
		                    share_mode, 
	                        NULL, 
                            create_mode, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL);
    }
	if (handle == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
		switch (error) {
		case ERROR_FILE_NOT_FOUND:
		    return ATX_ERROR_NO_SUCH_FILE;
		default:
		    return ATX_FAILURE;
		}
	}

    /* remember the mode */
    file->mode = mode;

    /* create a handle wrapper */
    result = Win32FileHandleWrapper_Create(handle, file->size, &file->file);
    if (ATX_FAILED(result)) return result;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32File_Close
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Close(ATX_FileInstance* instance)
{
    Win32File* file = (Win32File*)instance;

    /* release the resources and reset */
    Win32FileHandleWrapper_Release(file->file);
    file->file = NULL;
    file->mode = 0;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Win32File_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetSize(ATX_FileInstance* instance, ATX_Size* size)
{
    Win32File* file = (Win32File*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* return the size */
    if (size) *size = file->size;

    return ATX_SUCCESS;
}


/*----------------------------------------------------------------------
|       Win32File_GetInputStream
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetInputStream(ATX_FileInstance* instance, 
                         ATX_InputStream*  stream)
{
    Win32File* file = (Win32File*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(file->mode & ATX_FILE_OPEN_MODE_READ)) {
        return ATX_ERROR_FILE_NOT_READABLE;
    }

    return Win32FileInputStream_Create(file->file, stream);
}

/*----------------------------------------------------------------------
|       Win32File_GetOutputStream
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetOutputStream(ATX_FileInstance* instance, 
                          ATX_OutputStream* stream)
{
    Win32File* file = (Win32File*)instance;

    /* check that the file is open */
    if (file->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(file->mode & ATX_FILE_OPEN_MODE_WRITE)) {
        return ATX_ERROR_FILE_NOT_WRITABLE;
    }

    return Win32FileOutputStream_Create(file->file, stream);
}


/*----------------------------------------------------------------------
|       ATX_File interface
+---------------------------------------------------------------------*/
static const ATX_FileInterface
Win32File_ATX_FileInterface = {
    Win32File_GetInterface,
    Win32File_Open,
    Win32File_Close,
    Win32File_GetSize,
    Win32File_GetInputStream,
    Win32File_GetOutputStream
};

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(Win32File)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32File) 
ATX_INTERFACE_MAP_ADD(Win32File, ATX_File)
ATX_INTERFACE_MAP_ADD(Win32File, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Win32File)
