/*****************************************************************
|
|      File: AtxAndroidFile.c
|
| Copyright (c) 2002-2016, Axiomatic Systems, LLC.
| All rights reserved.
|
| Redistribution and use in source and binary forms, with or without
| modification, are permitted provided that the following conditions are met:
|     * Redistributions of source code must retain the above copyright
|       notice, this list of conditions and the following disclaimer.
|     * Redistributions in binary form must reproduce the above copyright
|       notice, this list of conditions and the following disclaimer in the
|       documentation and/or other materials provided with the distribution.
|     * Neither the name of Axiomatic Systems nor the
|       names of its contributors may be used to endorse or promote products
|       derived from this software without specific prior written permission.
|
| THIS SOFTWARE IS PROVIDED BY AXIOMATIC SYSTEMS ''AS IS'' AND ANY
| EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
| WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
| DISCLAIMED. IN NO EVENT SHALL AXIOMATIC SYSTEMS BE LIABLE FOR ANY
| DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
| (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
| ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
| (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
| SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "AtxUtils.h"
#include "AtxStreams.h"
#include "AtxFile.h"
#include "AtxResults.h"
#include "AtxReferenceable.h"
#include "AtxDestroyable.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal  reference_count;
    int           fd;
    ATX_LargeSize size;
    ATX_Position  position;
    ATX_String    name;
} AndroidFileWrapper;

typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_InputStream);
    ATX_IMPLEMENTS(ATX_OutputStream);
    ATX_IMPLEMENTS(ATX_Referenceable);

    /* members */
    ATX_Cardinal     reference_count;
    AndroidFileWrapper* file;
} AndroidFileStream;

typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_File);
    ATX_IMPLEMENTS(ATX_Destroyable);

    /* members */
    ATX_String       name;
    ATX_Flags        mode;
    AndroidFileWrapper* file;
} AndroidFile;

/*----------------------------------------------------------------------
|   MapErrno
+---------------------------------------------------------------------*/
static ATX_Result
MapErrno(int err) {
    switch (err) {
      case EACCES:       return ATX_ERROR_ACCESS_DENIED;
      case EPERM:        return ATX_ERROR_ACCESS_DENIED;
      case ENOENT:       return ATX_ERROR_NO_SUCH_FILE;
#if defined(ENAMETOOLONG)
      case ENAMETOOLONG: return ATX_ERROR_INVALID_PARAMETERS;
#endif
      case EBUSY:        return ATX_ERROR_FILE_BUSY;
      case EROFS:        return ATX_ERROR_FILE_NOT_WRITABLE;
      default:           return ATX_ERROR_ERRNO(err);
    }
}

/*----------------------------------------------------------------------
|       AndroidFileWrapper_Create
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFileWrapper_Create(int               fd,
                          ATX_String*       name,
                          AndroidFileWrapper** wrapper)
{
    /* allocate a new object */
    (*wrapper) = ATX_AllocateZeroMemory(sizeof(AndroidFileWrapper));
    if (*wrapper == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*wrapper)->fd              = fd;
    (*wrapper)->size            = 0;
    (*wrapper)->position        = 0;
    (*wrapper)->reference_count = 1;
    if (name) {
        ATX_String_Copy(&(*wrapper)->name, name);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
 |       AndroidFileWrapper_UpdateSize
 +---------------------------------------------------------------------*/
static void
AndroidFileWrapper_UpdateSize(AndroidFileWrapper* self)
{
    if (self->fd < 0              ||
        self->fd == STDIN_FILENO  ||
        self->fd == STDOUT_FILENO ||
        self->fd == STDERR_FILENO) {
        return;
    }

    {
        struct stat info;
        if (stat(ATX_CSTR(self->name), &info) == 0) {
            self->size = info.st_size;
        }
    }
}


/*----------------------------------------------------------------------
|       AndroidFileWrapper_Destroy
+---------------------------------------------------------------------*/
static void
AndroidFileWrapper_Destroy(AndroidFileWrapper* self)
{
    if (self->fd >= 0             &&
        self->fd != STDIN_FILENO  &&
        self->fd != STDOUT_FILENO &&
        self->fd != STDERR_FILENO) {
        close(self->fd);
    }
    ATX_String_Destruct(&self->name);
    ATX_FreeMemory((void*)self);
}

/*----------------------------------------------------------------------
|       AndroidFileWrapper_AddReference
+---------------------------------------------------------------------*/
static void
AndroidFileWrapper_AddReference(AndroidFileWrapper* self)
{
    ++self->reference_count;
}

/*----------------------------------------------------------------------
|       AndroidFileWrapper_Release
+---------------------------------------------------------------------*/
static void
AndroidFileWrapper_Release(AndroidFileWrapper* self)
{
    if (self == NULL) return;
    if (--self->reference_count == 0) {
        AndroidFileWrapper_Destroy(self);
    }
}

/*----------------------------------------------------------------------
|   forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE_MAP(AndroidFileStream, ATX_InputStream)
ATX_DECLARE_INTERFACE_MAP(AndroidFileStream, ATX_OutputStream)
ATX_DECLARE_INTERFACE_MAP(AndroidFileStream, ATX_Referenceable)

/*----------------------------------------------------------------------
|       AndroidFileStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFileStream_Create(AndroidFileWrapper* file, AndroidFileStream** stream)
{
    /* create a new object */
    (*stream) = (AndroidFileStream*)ATX_AllocateMemory(sizeof(AndroidFileStream));
    if (*stream == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* construct the object */
    (*stream)->reference_count = 1;
    (*stream)->file = file;

    /* keep a reference */
    AndroidFileWrapper_AddReference(file);

    /* setup interfaces */
    ATX_SET_INTERFACE((*stream), AndroidFileStream, ATX_InputStream);
    ATX_SET_INTERFACE((*stream), AndroidFileStream, ATX_OutputStream);
    ATX_SET_INTERFACE((*stream), AndroidFileStream, ATX_Referenceable);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFileStream_Destroy(AndroidFileStream* self)
{
    AndroidFileWrapper_Release(self->file);
    ATX_FreeMemory((void*)self);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileStream_Seek(AndroidFileStream* self, ATX_Position where)
{
    //off_t result = lseek(self->file->fd, where, SEEK_SET);
    off64_t result = lseek64(self->file->fd, where, SEEK_SET);
    if (result >= 0) {
        self->file->position = where;
        return ATX_SUCCESS;
    } else {
        return MapErrno(errno);
    }
}

/*----------------------------------------------------------------------
|       AndroidFileStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileStream_Tell(AndroidFileStream* self, ATX_Position* where)
{
    if (where) *where = self->file->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileStream_Flush(AndroidFileStream* self)
{
    ATX_COMPILER_UNUSED(self);
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFileInputStream_Create(AndroidFileWrapper* file,
                              ATX_InputStream**   stream)
{
    AndroidFileStream* file_stream = NULL;
    ATX_Result      result;

    /* create the object */
    result = AndroidFileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        *stream = NULL;
        return result;
    }

    /* select the ATX_InputStream interface */
    *stream = &ATX_BASE(file_stream, ATX_InputStream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileInputStream_Read(ATX_InputStream* _self,
                            ATX_Any          buffer,
                            ATX_Size         bytes_to_read,
                            ATX_Size*        bytes_read)
{
    AndroidFileStream* self = ATX_SELF(AndroidFileStream, ATX_InputStream);
    ssize_t            nb_read;

    /* shortcut */
    if (bytes_to_read == 0) {
        if (bytes_read) *bytes_read = 0;
        return ATX_SUCCESS;
    }

    /* if we try to read past the end, update the size in case the file has grown */
    if (self->file->position+bytes_to_read > self->file->size) {
        AndroidFileWrapper_UpdateSize(self->file);
    }

    /* read from the file */
    nb_read = read(self->file->fd, buffer, (size_t)bytes_to_read);
    if (nb_read == 0) {
        if (bytes_read) *bytes_read = 0;
        return ATX_ERROR_EOS;
    } else if (nb_read > 0) {
        if (bytes_read) *bytes_read = (ATX_Size)nb_read;
        self->file->position += nb_read;
        return ATX_SUCCESS;
    } else {
        if (bytes_read) *bytes_read = 0;
        return MapErrno(errno);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileInputStream_Seek(ATX_InputStream* _self,
                            ATX_Position     where)
{
    return AndroidFileStream_Seek(ATX_SELF(AndroidFileStream, ATX_InputStream), where);
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileInputStream_Tell(ATX_InputStream* _self,
                            ATX_Position*    where)
{
    return AndroidFileStream_Tell(ATX_SELF(AndroidFileStream, ATX_InputStream), where);
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileInputStream_GetSize(ATX_InputStream* _self,
                               ATX_LargeSize*   size)
{
    AndroidFileStream* self = ATX_SELF(AndroidFileStream, ATX_InputStream);
    AndroidFileWrapper_UpdateSize(self->file);
    *size = self->file->size;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileInputStream_GetAvailable(ATX_InputStream* _self,
                                    ATX_LargeSize*   size)
{
    AndroidFileStream* self = ATX_SELF(AndroidFileStream, ATX_InputStream);

    AndroidFileWrapper_UpdateSize(self->file);
    if (self->file->position > self->file->size) {
        *size = self->file->size - self->file->position;
    } else {
        *size = 0;
    }
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileOutputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFileOutputStream_Create(AndroidFileWrapper* file,
                               ATX_OutputStream**  stream)
{
    AndroidFileStream* file_stream = NULL;
    ATX_Result         result;

    /* create the object */
    result = AndroidFileStream_Create(file, &file_stream);
    if (ATX_FAILED(result)) {
        *stream = NULL;
        return result;
    }

    /* select the ATX_InputStream interface */
    *stream = &ATX_BASE(file_stream, ATX_OutputStream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileOutputStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileOutputStream_Write(ATX_OutputStream* _self,
                              ATX_AnyConst      buffer,
                              ATX_Size          bytes_to_write,
                              ATX_Size*         bytes_written)
{
    AndroidFileStream* self = ATX_SELF(AndroidFileStream, ATX_OutputStream);
    ssize_t            nb_written;

    nb_written = write(self->file->fd, buffer, (ssize_t)bytes_to_write);
    if (nb_written > 0 || bytes_to_write == 0) {
        if (bytes_written) *bytes_written = (ATX_Size)nb_written;
        self->file->position += nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        return MapErrno(errno);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFileOutputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileOutputStream_Seek(ATX_OutputStream* _self,
                             ATX_Position      where)
{
    return AndroidFileStream_Seek(ATX_SELF(AndroidFileStream, ATX_OutputStream), where);
}

/*----------------------------------------------------------------------
|       AndroidFileOutputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileOutputStream_Tell(ATX_OutputStream* _self,
                             ATX_Position*     where)
{
    return AndroidFileStream_Tell(ATX_SELF(AndroidFileStream, ATX_OutputStream), where);
}

/*----------------------------------------------------------------------
|       AndroidFileOutputStream_Flush
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFileOutputStream_Flush(ATX_OutputStream* _self)
{
    return AndroidFileStream_Flush(ATX_SELF(AndroidFileStream, ATX_OutputStream));
}

/*----------------------------------------------------------------------
|   GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(AndroidFileStream)
    ATX_GET_INTERFACE_ACCEPT(AndroidFileStream, ATX_InputStream)
    ATX_GET_INTERFACE_ACCEPT(AndroidFileStream, ATX_OutputStream)
    ATX_GET_INTERFACE_ACCEPT(AndroidFileStream, ATX_Referenceable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

/*----------------------------------------------------------------------
|       ATX_InputStream interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(AndroidFileStream, ATX_InputStream)
    AndroidFileInputStream_Read,
    AndroidFileInputStream_Seek,
    AndroidFileInputStream_Tell,
    AndroidFileInputStream_GetSize,
    AndroidFileInputStream_GetAvailable
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|       ATX_OutputStream interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(AndroidFileStream, ATX_OutputStream)
    AndroidFileOutputStream_Write,
    AndroidFileOutputStream_Seek,
    AndroidFileOutputStream_Tell,
    AndroidFileOutputStream_Flush
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_REFERENCEABLE_INTERFACE(AndroidFileStream, reference_count)

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE_MAP(AndroidFile, ATX_File)
ATX_DECLARE_INTERFACE_MAP(AndroidFile, ATX_Destroyable)

/*----------------------------------------------------------------------
|       ATX_File_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_File_Create(const char* filename, ATX_File** object)
{
    AndroidFile* file;

    /* allocate a new object */
    file = (AndroidFile*)ATX_AllocateZeroMemory(sizeof(AndroidFile));
    if (file == NULL) {
        *object = NULL;
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    file->name = ATX_String_Create(filename);

    /* setup the interfaces */
    ATX_SET_INTERFACE(file, AndroidFile, ATX_File);
    ATX_SET_INTERFACE(file, AndroidFile, ATX_Destroyable);
    *object = &ATX_BASE(file, ATX_File);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFile_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFile_Destroy(ATX_Destroyable* _self)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_Destroyable);

    /* release the resources */
    ATX_String_Destruct(&self->name);
    AndroidFileWrapper_Release(self->file);

    /* free the memory */
    ATX_FreeMemory((void*)self);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFile_Open
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFile_Open(ATX_File* _self, ATX_Flags mode)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_File);
    int          fd   = -1;

    /* decide wheter this is a file or stdin/stdout/stderr */
    if (ATX_String_Equals(&self->name,
                          ATX_FILE_STANDARD_INPUT,
                          ATX_FALSE)) {
        fd = STDIN_FILENO;
    } else if (ATX_String_Equals(&self->name,
                                 ATX_FILE_STANDARD_OUTPUT,
                                 ATX_FALSE)) {
        fd = STDOUT_FILENO;
    } else if (ATX_String_Equals(&self->name,
                                 ATX_FILE_STANDARD_ERROR,
                                 ATX_FALSE)) {
        fd = STDERR_FILENO;
    } else {
        int open_flags  = 0;
        int create_perm = 0;

        /* compute the open flags */
        if (mode & ATX_FILE_OPEN_MODE_WRITE) {
            if (mode & ATX_FILE_OPEN_MODE_READ) {
                open_flags |= O_RDWR;
            } else {
                open_flags |= O_WRONLY;
            }
            if (mode & ATX_FILE_OPEN_MODE_APPEND) {
                open_flags |= O_APPEND;
            }
            if (mode & ATX_FILE_OPEN_MODE_CREATE) {
                create_perm = 0666;
                open_flags |= O_CREAT;
            }
            if (mode & ATX_FILE_OPEN_MODE_TRUNCATE) {
                open_flags |= O_TRUNC;
            }
        } else {
            open_flags |= O_RDONLY;
        }

        /* try to open the file */
        fd = open(ATX_CSTR(self->name), open_flags, create_perm);
        if (fd < 0) {
            return MapErrno(errno);
        }
    }

    /* remember the mode */
    self->mode = mode;

    /* create a wrapper */
    return AndroidFileWrapper_Create(fd, &self->name, &self->file);
}

/*----------------------------------------------------------------------
|       AndroidFile_Close
+---------------------------------------------------------------------*/
static ATX_Result
AndroidFile_Close(ATX_File* _self)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_File);

    /* release the resources and reset */
    AndroidFileWrapper_Release(self->file);
    self->file = NULL;
    self->mode = 0;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFile_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFile_GetSize(ATX_File* _self, ATX_LargeSize* size)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* return the size */
    AndroidFileWrapper_UpdateSize(self->file);
    *size = self->file->size;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       AndroidFile_GetInputStream
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFile_GetInputStream(ATX_File*         _self,
                           ATX_InputStream** stream)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(self->mode & ATX_FILE_OPEN_MODE_READ)) {
        return ATX_ERROR_FILE_NOT_READABLE;
    }

    return AndroidFileInputStream_Create(self->file, stream);
}

/*----------------------------------------------------------------------
|       AndroidFile_GetOutputStream
+---------------------------------------------------------------------*/
ATX_METHOD
AndroidFile_GetOutputStream(ATX_File*          _self,
                         ATX_OutputStream** stream)
{
    AndroidFile* self = ATX_SELF(AndroidFile, ATX_File);

    /* check that the file is open */
    if (self->file == NULL) return ATX_ERROR_FILE_NOT_OPEN;

    /* check that the mode is compatible */
    if (!(self->mode & ATX_FILE_OPEN_MODE_WRITE)) {
        return ATX_ERROR_FILE_NOT_WRITABLE;
    }

    return AndroidFileOutputStream_Create(self->file, stream);
}

/*----------------------------------------------------------------------
|   AndroidFile_GetInterface
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(AndroidFile)
    ATX_GET_INTERFACE_ACCEPT(AndroidFile, ATX_File)
    ATX_GET_INTERFACE_ACCEPT(AndroidFile, ATX_Destroyable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

/*----------------------------------------------------------------------
|       ATX_File interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(AndroidFile, ATX_File)
    AndroidFile_Open,
    AndroidFile_Close,
    AndroidFile_GetSize,
    AndroidFile_GetInputStream,
    AndroidFile_GetOutputStream
ATX_END_INTERFACE_MAP

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_DESTROYABLE_INTERFACE(AndroidFile)
