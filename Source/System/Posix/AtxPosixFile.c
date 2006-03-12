/*****************************************************************
|
|      File: AtxPosixFile.c
|
|      Atomix - File ByteStream: StdC Implementation
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxByteStream.h"
#include "AtxFileByteStream.h"
#include "AtxReferenceable.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    ATX_Cardinal reference_count;
    int          fd;
    ATX_Size     size;
    ATX_Offset   offset;
} FileByteStream;

/*----------------------------------------------------------------------
|       forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(FileByteStream)
static const ATX_ByteStreamInterface FileByteStream_ATX_ByteStreamInterface;

/*----------------------------------------------------------------------
|       ATX_FileByteStream_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_FileByteStream_Create(const char*     filename, 
                          ATX_Flags       mode, 
                          ATX_ByteStream* object)
{ 
    FileByteStream* stream;
    int             fd;

    /* decide wheter this is a file or stdin/stdout/stderr */
    if (!strcmp(filename, "-stdin")) {
        fd = 0;
    } else if (!strcmp(filename, "-stdout")) {
        fd = 1;
    } else if (!strcmp(filename, "-stderr")) {
        fd = 2;
    } else {
        int flags = 0;

        /* compute mode */
        if (mode & ATX_FILE_BYTE_STREAM_MODE_READ) {
            if (mode & ATX_FILE_BYTE_STREAM_MODE_WRITE) {
                flags |= O_RDWR;
            } else {
                flags |= O_RDONLY;
            }
        } else {
            if (mode & ATX_FILE_BYTE_STREAM_MODE_WRITE) {
                flags |= O_WRONLY;
            }
        }
        if (mode & ATX_FILE_BYTE_STREAM_MODE_CREATE) {
            flags |= O_CREAT;
        } 
        if (mode & ATX_FILE_BYTE_STREAM_MODE_TRUNCATE) {
            flags |= O_TRUNC;
        }

        /* try to open the file */
        fd = open(filename, flags, 
                  S_IWOTH | 
                  S_IRGRP | S_IWGRP | 
                  S_IWUSR | S_IRUSR);
        if (fd == -1) {
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

    /* allocate new object */
    stream = (FileByteStream*)ATX_AllocateZeroMemory(sizeof(FileByteStream));
    if (stream == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    stream->reference_count = 1;
    stream->fd              = fd;
    stream->offset          = 0;

    /* get the size */
    {
        struct stat info;
        if (fstat(fd, &info) == 0) {
            stream->size = info.st_size;
        } else {
            stream->size = 0;
        }
    }

    /* return reference */
    ATX_INSTANCE(object)  = (ATX_ByteStreamInstance*)stream;
    ATX_INTERFACE(object) = &FileByteStream_ATX_ByteStreamInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
FileByteStream_Destroy(FileByteStream* stream)
{
    if (stream->fd >= 0) close(stream->fd);
    ATX_FreeMemory((void*)stream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
FileByteStream_Read(ATX_ByteStreamInstance* instance,
                    ATX_Any                 buffer, 
                    ATX_Size                bytes_to_read, 
                    ATX_Size*               bytes_read)
{
    FileByteStream* stream = (FileByteStream*)instance;
    ssize_t          nb_read;

    nb_read = read(stream->fd, buffer, (size_t)bytes_to_read);
    if (nb_read > 0) {
        if (bytes_read) *bytes_read = nb_read;
        stream->offset += nb_read;
        return ATX_SUCCESS;
    } else {
        if (bytes_read) *bytes_read = 0;
        if (nb_read == 0) {
            return ATX_ERROR_EOS;
        } else {
            return ATX_FAILURE;
        }
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_Write
+---------------------------------------------------------------------*/
ATX_METHOD
FileByteStream_Write(ATX_ByteStreamInstance* instance,
                     ATX_AnyConst            buffer, 
                     ATX_Size                bytes_to_write, 
                     ATX_Size*               bytes_written)
{
    FileByteStream* stream = (FileByteStream*)instance;
    ssize_t         nb_written;

    nb_written = write(stream->fd, buffer, (size_t)bytes_to_write);
    if (nb_written > 0) {
        if (bytes_written) *bytes_written = nb_written;
        stream->offset += nb_written;
        return ATX_SUCCESS;
    } else {
        if (bytes_written) *bytes_written = 0;
        return ATX_FAILURE;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
FileByteStream_Seek(ATX_ByteStreamInstance* instance, 
                    ATX_Offset              where)
{
    FileByteStream* stream = (FileByteStream*)instance;
    lseek(stream->fd, (off_t)where, SEEK_SET);
    stream->offset = where;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
FileByteStream_Tell(ATX_ByteStreamInstance* instance, 
                    ATX_Offset*             where)
{
    FileByteStream* stream = (FileByteStream*)instance;
    if (where) *where = stream->offset;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       FileByteStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
FileByteStream_GetSize(ATX_ByteStreamInstance* instance,
                       ATX_Size*               size)
{
    FileByteStream* stream = (FileByteStream*)instance;
    if (size) *size = stream->size;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       ByteStream interface
+---------------------------------------------------------------------*/
static const ATX_ByteStreamInterface
FileByteStream_ATX_ByteStreamInterface = {
    FileByteStream_GetInterface,
    FileByteStream_Read,
    FileByteStream_Write,
    FileByteStream_Seek,
    FileByteStream_Tell,
    FileByteStream_GetSize
};

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_REFERENCEABLE_INTERFACE(FileByteStream, reference_count)

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(FileByteStream) 
ATX_INTERFACE_MAP_ADD(FileByteStream, ATX_ByteStream)
ATX_INTERFACE_MAP_ADD(FileByteStream, ATX_Referenceable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(FileByteStream)

