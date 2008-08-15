/*****************************************************************
||
|   Atomix - File Streams: Win32 Implementation
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#define STRICT
#include <windows.h>

#include "AtxInterfaces.h"
#include "AtxUtils.h"
#include "AtxResults.h"
#include "AtxStreams.h"
#include "AtxFile.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal reference_count;
    HANDLE       handle;
    ATX_Position position;
    ATX_Size     size;
    ATX_Boolean  append_mode;
} Win32FileHandleWrapper;

typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_InputStream);
    ATX_IMPLEMENTS(ATX_OutputStream);
    ATX_IMPLEMENTS(ATX_Referenceable);

    /* members */
    ATX_Cardinal            reference_count;
    Win32FileHandleWrapper* file;
} Win32FileStream;

typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_File);
    ATX_IMPLEMENTS(ATX_Destroyable);

    /* members */
    ATX_CString             name;
    ATX_Size                size;
    ATX_Flags               mode;
    Win32FileHandleWrapper* file;
} Win32File;

/*----------------------------------------------------------------------
|   Win32FileHandleWrapper_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileHandleWrapper_Create(HANDLE                   handle, 
                              ATX_Size                 size,
                              ATX_Boolean              append_mode,
                              Win32FileHandleWrapper** wrapper)
{
    /* allocate a new object */
    (*wrapper) = ATX_AllocateZeroMemory(sizeof(Win32FileHandleWrapper));
    if (*wrapper == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*wrapper)->handle          = handle;
    (*wrapper)->position        = 0;
    (*wrapper)->size            = size;
    (*wrapper)->append_mode     = append_mode;
    (*wrapper)->reference_count = 1;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileHandleWrapper_Destroy
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_Destroy(Win32FileHandleWrapper* self)
{
#if defined(_WIN32_WCE)
    CloseHandle(self->handle);
#else
    if (self->handle != GetStdHandle(STD_INPUT_HANDLE) &&
        self->handle != GetStdHandle(STD_OUTPUT_HANDLE) &&
        self->handle != GetStdHandle(STD_ERROR_HANDLE)) {
        CloseHandle(self->handle);
    }
#endif

    ATX_FreeMemory((void*)self);
}

/*----------------------------------------------------------------------
|   Win32FileHandleWrapper_AddReference
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_AddReference(Win32FileHandleWrapper* self)
{
    ++self->reference_count;
}

/*----------------------------------------------------------------------
|   Win32FileHandleWrapper_Release
+---------------------------------------------------------------------*/
static void
Win32FileHandleWrapper_Release(Win32FileHandleWrapper* self)
{
    if (self == NULL) return;
    if (--self->reference_count == 0) {
        Win32FileHandleWrapper_Destroy(self);
    }
}

/*----------------------------------------------------------------------
|   forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE_MAP(Win32FileStream, ATX_InputStream)
ATX_DECLARE_INTERFACE_MAP(Win32FileStream, ATX_OutputStream)
ATX_DECLARE_INTERFACE_MAP(Win32FileStream, ATX_Referenceable)

/*----------------------------------------------------------------------
|   Win32FileStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileStream_Create(Win32FileHandleWrapper* file,
                       Win32FileStream**       stream)
{
    /* create a new object */
    (*stream) = (Win32FileStream*)ATX_AllocateMemory(sizeof(Win32FileStream));
    if (*stream == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*stream)->reference_count = 1;
    (*stream)->file = file;

    /* keep a reference to the file */
    Win32FileHandleWrapper_AddReference(file);

    /* setup interfaces */
    ATX_SET_INTERFACE((*stream), Win32FileStream, ATX_InputStream);
    ATX_SET_INTERFACE((*stream), Win32FileStream, ATX_OutputStream);
    ATX_SET_INTERFACE((*stream), Win32FileStream, ATX_Referenceable);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileStream_Destroy(Win32FileStream* self)
{
    Win32FileHandleWrapper_Release(self->file);
    ATX_FreeMemory((void*)self);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileStream_Seek(Win32FileStream* self, ATX_Position where)
{
    SetFilePointer(self->file->handle, where, 0, FILE_BEGIN);
    self->file->position = where;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileStream_Tell(Win32FileStream* self, ATX_Position* where)
{
    if (where) *where = self->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileInputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileInputStream_Create(Win32FileHandleWrapper* file, 
                            ATX_InputStream**       stream)
{
    Win32FileStream* file_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = Win32FileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        *stream = NULL;
        return result;
    }

    /* select the ATX_InputStream interface */
    *stream = &ATX_BASE(file_stream, ATX_InputStream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileInputStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Read(ATX_InputStream* _self,
                          ATX_Any          buffer,
                          ATX_Size         bytes_to_read,
                          ATX_Size*        bytes_read)
{
    Win32FileStream* self = ATX_SELF(Win32FileStream, ATX_InputStream);
    DWORD            nb_read;
    BOOL             result;

    result = ReadFile(self->file->handle, 
                      buffer, 
                      bytes_to_read, 
                      &nb_read, 
                      NULL);
    if (result == TRUE) {
        if (bytes_read) *bytes_read = nb_read;
        self->file->position += nb_read;
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
|   Win32FileInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Seek(ATX_InputStream* _self, 
                          ATX_Position     where)
{
    return Win32FileStream_Seek(ATX_SELF(Win32FileStream, ATX_InputStream), 
                                where);
}

/*----------------------------------------------------------------------
|   Win32FileInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_Tell(ATX_InputStream* _self, 
                          ATX_Position*    where)
{
    return Win32FileStream_Tell(ATX_SELF(Win32FileStream, ATX_InputStream), 
                                where);
}

/*----------------------------------------------------------------------
|   Win32FileInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_GetSize(ATX_InputStream* _self, 
                             ATX_Size*        size)
{
    Win32FileStream* self = ATX_SELF(Win32FileStream, ATX_InputStream);
    *size = self->file->size;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileInputStream_GetAvailable(ATX_InputStream* _self, 
                                  ATX_Size*        size)
{
    Win32FileStream* self = ATX_SELF(Win32FileStream, ATX_InputStream);
    *size = self->file->size - self->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileOutputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
Win32FileOutputStream_Create(Win32FileHandleWrapper* file, 
                             ATX_OutputStream**      stream)
{
    Win32FileStream* file_stream = NULL;
    ATX_Result       result;

    /* create the object */
    result = Win32FileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        *stream = NULL;
        return result;
    }

    /* select the ATX_InputStream interface */
    *stream = &ATX_BASE(file_stream, ATX_OutputStream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileOutputStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Write(ATX_OutputStream* _self,
                            ATX_AnyConst      buffer, 
                            ATX_Size          bytes_to_write, 
                            ATX_Size*         bytes_written)
{
    Win32FileStream* self = ATX_SELF(Win32FileStream, ATX_OutputStream);
    DWORD            nb_written;
    BOOL             result;

    /* in append mode, seek to the end of the file */
    if (self->file->append_mode) {
        DWORD file_size = 0;
        SetFilePointer(self->file->handle, 0, 0, FILE_END);
        GetFileSize(self->file->handle, &file_size);
        self->file->position = file_size;
    }

    /* write to the file */
    result = WriteFile(self->file->handle, 
                       buffer, 
                       bytes_to_write, 
                       &nb_written, 
                       NULL);
    if (result == TRUE) {
        if (bytes_written) *bytes_written = nb_written;
        self->file->position += nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        return ATX_FAILURE;
    }
}

/*----------------------------------------------------------------------
|   Win32FileOutputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Seek(ATX_OutputStream* _self, 
                           ATX_Position      where)
{
    return Win32FileStream_Seek(ATX_SELF(Win32FileStream, ATX_OutputStream), 
                                where);
}

/*----------------------------------------------------------------------
|   Win32FileOutputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Tell(ATX_OutputStream* _self, 
                           ATX_Position*     where)
{
    return Win32FileStream_Tell(ATX_SELF(Win32FileStream, ATX_OutputStream), 
                                where);
}

/*----------------------------------------------------------------------
|   Win32FileOutputStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
Win32FileOutputStream_Flush(ATX_OutputStream* _self)
{
	ATX_COMPILER_UNUSED(_self);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32FileStream_GetInterface
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(Win32FileStream)
    ATX_GET_INTERFACE_ACCEPT(Win32FileStream, ATX_InputStream)
    ATX_GET_INTERFACE_ACCEPT(Win32FileStream, ATX_OutputStream)
    ATX_GET_INTERFACE_ACCEPT(Win32FileStream, ATX_Referenceable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

/*----------------------------------------------------------------------
|   ATX_InputStream interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(Win32FileStream, ATX_InputStream)
    Win32FileInputStream_Read,
    Win32FileInputStream_Seek,
    Win32FileInputStream_Tell,
    Win32FileInputStream_GetSize,
    Win32FileInputStream_GetAvailable
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|   ATX_OutputStream interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(Win32FileStream, ATX_OutputStream)
    Win32FileOutputStream_Write,
    Win32FileOutputStream_Seek,
    Win32FileOutputStream_Tell,
    Win32FileOutputStream_Flush
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|   ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_REFERENCEABLE_INTERFACE(Win32FileStream, reference_count)

/*----------------------------------------------------------------------
|   forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE_MAP(Win32File, ATX_File)
ATX_DECLARE_INTERFACE_MAP(Win32File, ATX_Destroyable)

#if defined (_WIN32_WCE)
/*----------------------------------------------------------------------
|   FindFirstFile_UTF8
+---------------------------------------------------------------------*/
static HANDLE
FindFirstFile_UTF8(LPCSTR filename, LPWIN32_FIND_DATA info)
{
    HANDLE handle;
    unsigned int filename_length = ATX_StringLength(filename);
    WCHAR* filename_w = ATX_AllocateMemory(2*(filename_length+1));
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, filename_w, filename_length+1);
    handle = FindFirstFile(filename_w, info);
    ATX_FreeMemory(filename_w);

    return handle;
}

/*----------------------------------------------------------------------
|   CreateFile_UTF8
+---------------------------------------------------------------------*/
static HANDLE
CreateFile_UTF8(LPCSTR lpFileName,
                DWORD dwDesiredAccess,
                DWORD dwShareMode,
                LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                DWORD dwCreationDisposition,
                DWORD dwFlagsAndAttributes,
                HANDLE hTemplateFile)
{
    HANDLE handle;
    unsigned int filename_length = ATX_StringLength(lpFileName);
    WCHAR* filename_w = ATX_AllocateMemory(2*(filename_length+1));
    MultiByteToWideChar(CP_UTF8, 0, lpFileName, -1, filename_w, filename_length+1);
    handle = CreateFile(filename_w, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    ATX_FreeMemory(filename_w);

    return handle;
}

#endif 

/*----------------------------------------------------------------------
|   ATX_File_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Create(const char* filename, ATX_File** object)
{
    Win32File* file;

    /* allocate a new object */
    file = (Win32File*)ATX_AllocateZeroMemory(sizeof(Win32File));
    if (file == NULL) {
        *object = NULL;
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    ATX_SET_CSTRING(file->name, filename);

    /* get the size */
#if !defined(_WIN32_WCE)
    if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_INPUT)  ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_OUTPUT) ||
        ATX_StringsEqual(filename, ATX_FILE_STANDARD_ERROR)) {
        file->size = 0;
    } else 
#endif
    {
        WIN32_FIND_DATA info;
        HANDLE          f;
#if defined(_WIN32_WCE)
        f = FindFirstFile_UTF8(filename, &info);
#else
        f = FindFirstFile(filename, &info);
#endif

        if (f == INVALID_HANDLE_VALUE) {
            file->size = 0;
        } else {
            file->size = info.nFileSizeLow;
            FindClose(f);
        }
    }

    /* setup the interfaces */
    ATX_SET_INTERFACE(file, Win32File, ATX_File);
    ATX_SET_INTERFACE(file, Win32File, ATX_Destroyable);
    *object = &ATX_BASE(file, ATX_File);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32File_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Destroy(ATX_Destroyable* _self)
{
    Win32File* self = ATX_SELF(Win32File, ATX_Destroyable);

    /* release the resources */
    ATX_DESTROY_CSTRING(self->name);
    Win32FileHandleWrapper_Release(self->file);

    /* free the memory */
    ATX_FreeMemory((void*)self);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32File_Open
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Open(ATX_File* _self, ATX_Flags mode)
{
    Win32File*  self = ATX_SELF(Win32File, ATX_File);
    HANDLE      handle;
    DWORD       access_mode = 0;
    DWORD       share_mode  = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD       create_mode = 0;
    const char* filename = self->name;

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
#if !defined(_WIN32_WCE)
    if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_INPUT)) {
        handle = GetStdHandle(STD_INPUT_HANDLE);
    } else if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_OUTPUT)) {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
    } else if (ATX_StringsEqual(filename, ATX_FILE_STANDARD_ERROR)) {
        handle = GetStdHandle(STD_ERROR_HANDLE);
    } else 
#endif
    {
        /* try to open the file */
#if defined(_WIN32_WCE)
        handle = CreateFile_UTF8(
#else
        handle = CreateFile(
#endif
                            filename, 
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
    self->mode = mode;

    /* create a handle wrapper */
    return Win32FileHandleWrapper_Create(handle, self->size, (mode&ATX_FILE_OPEN_MODE_APPEND)?ATX_TRUE:ATX_FALSE, &self->file);
}

/*----------------------------------------------------------------------
|   Win32File_Close
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_Close(ATX_File* _self)
{
    Win32File* self = ATX_SELF(Win32File, ATX_File);

    /* release the resources and reset */
    Win32FileHandleWrapper_Release(self->file);
    self->file = NULL;
    self->mode = 0;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   Win32File_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetSize(ATX_File* _self, ATX_Size* size)
{
    Win32File* self = ATX_SELF(Win32File, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* return the size */
    if (size) *size = self->size;

    return ATX_SUCCESS;
}


/*----------------------------------------------------------------------
|   Win32File_GetInputStream
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetInputStream(ATX_File*          _self, 
                         ATX_InputStream**  stream)
{
    Win32File* self = ATX_SELF(Win32File, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(self->mode & ATX_FILE_OPEN_MODE_READ)) {
        return ATX_ERROR_FILE_NOT_READABLE;
    }

    return Win32FileInputStream_Create(self->file, stream);
}

/*----------------------------------------------------------------------
|   Win32File_GetOutputStream
+---------------------------------------------------------------------*/
ATX_METHOD
Win32File_GetOutputStream(ATX_File*          _self, 
                          ATX_OutputStream** stream)
{
    Win32File* self = ATX_SELF(Win32File, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(self->mode & ATX_FILE_OPEN_MODE_WRITE)) {
        return ATX_ERROR_FILE_NOT_WRITABLE;
    }

    return Win32FileOutputStream_Create(self->file, stream);
}

/*----------------------------------------------------------------------
|   Win32File_GetInterface
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(Win32File)
    ATX_GET_INTERFACE_ACCEPT(Win32File, ATX_File)
    ATX_GET_INTERFACE_ACCEPT(Win32File, ATX_Destroyable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

/*----------------------------------------------------------------------
|   ATX_File interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(Win32File, ATX_File)
    Win32File_Open,
    Win32File_Close,
    Win32File_GetSize,
    Win32File_GetInputStream,
    Win32File_GetOutputStream
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|   ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_DESTROYABLE_INTERFACE(Win32File)
