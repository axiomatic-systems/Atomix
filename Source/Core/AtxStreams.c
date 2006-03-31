/*****************************************************************
|
|   File: AtxStreams.c
|
|   Atomix - Streams Runtime Support
|
|   (c) 2002-2004 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxStreams.h"
#include "AtxReferenceable.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_InputStream);
    ATX_IMPLEMENTS(ATX_Referenceable);

    /* members */
    ATX_Cardinal           reference_count;
    ATX_InputStream*       parent;
    ATX_StreamTransformer* transformer;
    ATX_Size               size;
    ATX_Offset             offset;
    ATX_Offset             position;
} ATX_SubInputStream;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define ATX_INPUT_STREAM_LOAD_DEFAULT_READ_CHUNK 4096

/*----------------------------------------------------------------------
|   ATX_InputStream_ReadLine
+---------------------------------------------------------------------*/
ATX_Result
ATX_InputStream_ReadLine(ATX_InputStream* self, 
                         char*            buffer, 
                         ATX_Size         size, 
                         ATX_Size*        chars_read)
{
    ATX_Result result;
    ATX_Size   total = 0;

    /* check parameters */
    if (buffer == NULL || size < 1) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* read until EOF or newline */
    do {
        ATX_Size bytes_read;
        result = ATX_InputStream_Read(self, buffer, 1, &bytes_read);
        if (ATX_SUCCEEDED(result) && bytes_read == 1) {
            if (*buffer == '\n') {
                *buffer = '\0';
                if (chars_read) *chars_read = total;
                return ATX_SUCCESS;
            }  else if (*buffer == '\r') {
                continue;
            }
        } else {
            if (chars_read) *chars_read = total;
            if (result == ATX_ERROR_EOS) {
                *buffer = '\0';
                if (total != 0) {
                    return ATX_SUCCESS;
                } else {
                    return ATX_ERROR_EOS;
                }
            } else {
                return result;
            }
        }
        total++;
        buffer++;
    } while (total < size-1);
    
    /* terminate the line */
    *buffer = '\0';

    /* return what we have */
    if (chars_read) *chars_read = total;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_InputStream_ReadLineString
+---------------------------------------------------------------------*/
ATX_Result
ATX_InputStream_ReadLineString(ATX_InputStream* self, 
                               ATX_String*      string,
                               ATX_Size         max_length)
{
    ATX_Result result;

    /* reset the string */
    ATX_String_SetLength(string, 0);

    /* read until EOF or newline */
    do {
        ATX_Size bytes_read;
        char     c;
        result = ATX_InputStream_Read(self, &c, 1, &bytes_read);
        if (ATX_SUCCEEDED(result) && bytes_read == 1) {
            if (c == '\n') {
                return ATX_SUCCESS;
            }  else if (c == '\r') {
                continue;
            }
        } else {
            if (result == ATX_ERROR_EOS) {
                return ATX_String_IsEmpty(string)?ATX_ERROR_EOS:ATX_SUCCESS;
            } else {
                return result;
            }
        }
        ATX_String_AppendChar(string, c);
    } while (ATX_String_GetLength(string)<max_length);

    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|   ATX_InputStream_ReadFully
+---------------------------------------------------------------------*/
ATX_Result
ATX_InputStream_ReadFully(ATX_InputStream* self, 
                          ATX_Any          buffer, 
                          ATX_Size         bytes_to_read)
{
    ATX_Size   bytes_read;
    ATX_Result result;

    while (bytes_to_read) {
        result = ATX_InputStream_Read(self, buffer, bytes_to_read, &bytes_read);
        if (ATX_FAILED(result)) return result;
        if (bytes_read == 0) return ATX_FAILURE;
        bytes_to_read -= bytes_read;
        buffer = (ATX_Any)(((char*)buffer)+bytes_read);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_InputStream_Skip
+---------------------------------------------------------------------*/
ATX_Result
ATX_InputStream_Skip(ATX_InputStream* self, ATX_Size count)
{
    ATX_Offset position;
    ATX_Result result;

    /* get the current location */
    result = ATX_InputStream_Tell(self, &position);
    if (ATX_FAILED(result)) return result;
    
    /* seek ahead */
    return ATX_InputStream_Seek(self, position+count);
}

/*----------------------------------------------------------------------
|   ATX_InputStream_Load
+---------------------------------------------------------------------*/
ATX_Result
ATX_InputStream_Load(ATX_InputStream* self, ATX_Size max_read, ATX_DataBuffer** buffer)
{
    ATX_Result result;
    ATX_Size   total_bytes_read;
    ATX_Size   size = 0;

    /* create a buffer if none was given */
    if (*buffer == NULL) {
        ATX_CHECK(ATX_DataBuffer_Create(0, buffer));
    }

    /* reset the buffer */
    ATX_DataBuffer_SetDataSize(*buffer, 0);

    /* try to get the stream size */
    if (ATX_FAILED(ATX_InputStream_GetSize(self, &size))) {
        size = max_read;
    }  else {
        if (max_read && max_read < size) size = max_read;
    }

    /* pre-allocate the buffer */
    if (size) ATX_CHECK(ATX_DataBuffer_GrowBuffer(*buffer, size));

    /* read the data from the file */
    total_bytes_read = 0;
    do {
        ATX_Size  available = 0;
        ATX_Size  bytes_to_read;
        ATX_Size  bytes_read;
        ATX_Byte* data;

        /* check if we know how much data is available */
        result = ATX_InputStream_GetAvailable(self, &available);
        if (ATX_SUCCEEDED(result) && available) {
            /* we know how much is available */
            bytes_to_read = available;
        } else {
            bytes_to_read = ATX_INPUT_STREAM_LOAD_DEFAULT_READ_CHUNK;
        }

        /* make sure we don't read more than what was asked */
        if (size != 0 && total_bytes_read+bytes_to_read>size) {
            bytes_to_read = size-total_bytes_read;
        }

        /* stop if we've read everything */
        if (bytes_to_read == 0) break;

        /* allocate space in the buffer */
        ATX_CHECK(ATX_DataBuffer_GrowBuffer(*buffer, total_bytes_read+bytes_to_read));

        /* read the data */
        data = ATX_DataBuffer_UseData(*buffer)+total_bytes_read;
        result = ATX_InputStream_Read(self, (void*)data, bytes_to_read, &bytes_read);
        if (ATX_SUCCEEDED(result) && bytes_read != 0) {
            total_bytes_read += bytes_read;
            ATX_DataBuffer_SetDataSize(*buffer, total_bytes_read);
        }
    } while(ATX_SUCCEEDED(result) && (size==0 || total_bytes_read < size));

    if (result == ATX_ERROR_EOS) {
        return ATX_SUCCESS;
    } else {
        return result;
    }
}

/*----------------------------------------------------------------------
|   ATX_OutputStream_WriteFully
+---------------------------------------------------------------------*/
ATX_Result
ATX_OutputStream_WriteFully(ATX_OutputStream* self, 
                            const void*       buffer, 
                            ATX_Size          bytes_to_write)
{
    /* shortcut */
    if (bytes_to_write == 0) return ATX_SUCCESS;

    /* write until failure */
    while (bytes_to_write) {
        ATX_Size bytes_written;
        ATX_Result result = ATX_OutputStream_Write(self, buffer, bytes_to_write, &bytes_written);
        if (ATX_FAILED(result)) return result;
        if (bytes_written == 0) return ATX_FAILURE;
        ATX_ASSERT(bytes_written <= bytes_to_write);
        bytes_to_write -= bytes_written;
        buffer = (const void*)(((const ATX_Byte*)buffer)+bytes_written);
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_OutputStream_WriteString
+---------------------------------------------------------------------*/
ATX_Result
ATX_OutputStream_WriteString(ATX_OutputStream* self, ATX_CString string)
{
    /* shortcut */
    ATX_Size string_length;
    if (string == NULL || (string_length = ATX_StringLength(string)) == 0) {
        return ATX_SUCCESS;
    }

    /* write the string */
    return ATX_OutputStream_WriteFully(self, 
                                       (const void*)string, 
                                       string_length);
}

/*----------------------------------------------------------------------
|   ATX_OutputStream_WriteLine
+---------------------------------------------------------------------*/
ATX_Result
ATX_OutputStream_WriteLine(ATX_OutputStream* self, ATX_CString string)
{
    ATX_Result result;
    result = ATX_OutputStream_WriteString(self, string);
    if (ATX_FAILED(result)) return result;
    result = ATX_OutputStream_WriteFully(self, (const void*)"\r\n", 2);
    if (ATX_FAILED(result)) return result;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   forward declarations
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE_MAP(ATX_SubInputStream, ATX_InputStream)
ATX_DECLARE_INTERFACE_MAP(ATX_SubInputStream, ATX_Referenceable)

/*----------------------------------------------------------------------
|   ATX_SubInputStream_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_SubInputStream_Create(ATX_InputStream*       parent, 
                          ATX_Offset             offset,
                          ATX_Size               size,
                          ATX_StreamTransformer* transformer,
                          ATX_InputStream**      object)
{ 
    ATX_SubInputStream* stream;
    ATX_Result          result;

    /* check parameters */
    if (offset < 0) return ATX_ERROR_INVALID_PARAMETERS;

    /* allocate new object */
    stream = (ATX_SubInputStream*)ATX_AllocateZeroMemory(sizeof(ATX_SubInputStream));
    if (stream == NULL) {
        *object = NULL;
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct object */
    stream->reference_count = 1;
    stream->parent          = parent;
    if (transformer) {
        stream->transformer = transformer;
    }

    /* check the size and offset */
    {
        ATX_Size parent_size;

        stream->offset   = offset;
        stream->size     = size;
        stream->position = 0;

        result = ATX_InputStream_GetSize(parent, &parent_size);
        if (ATX_SUCCEEDED(result)) {
            if (parent_size > 0) {
                /* parent size known */
                if ((ATX_Size)offset >= parent_size) {
                    *object = NULL;
                    ATX_FreeMemory((void*)stream);
                    return ATX_ERROR_INVALID_PARAMETERS;
                }
                if (size == 0) {
                    /* unspecified size for the substream */
                    stream->size = parent_size - offset;
                } else {
                    if (size > parent_size - offset) {
                        stream->size = parent_size - offset;
                    }
                }
            }
        }
    }
    
    /* seek to the start point */
    result = ATX_InputStream_Seek(parent, offset);
    if (ATX_FAILED(result)) stream->offset = 0;

    /* keep a reference to the parent stream */
    ATX_REFERENCE_OBJECT(parent);

    /* setup the interfaces */
    ATX_SET_INTERFACE(stream, ATX_SubInputStream, ATX_InputStream);
    ATX_SET_INTERFACE(stream, ATX_SubInputStream, ATX_Referenceable);
    *object = &ATX_BASE(stream, ATX_InputStream);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_Destroy
+---------------------------------------------------------------------*/
static ATX_Result
ATX_SubInputStream_Destroy(ATX_SubInputStream* self)
{
    /* release the reference to the parent */
    ATX_RELEASE_OBJECT(self->parent);
    ATX_FreeMemory((void*)self);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_Read
+---------------------------------------------------------------------*/
ATX_METHOD
ATX_SubInputStream_Read(ATX_InputStream* _self,
                        ATX_Any          buffer, 
                        ATX_Size         bytes_to_read, 
                        ATX_Size*        bytes_read)
{
    ATX_SubInputStream* self = ATX_SELF(ATX_SubInputStream, ATX_InputStream);
    ATX_Size            local_read;
    ATX_Result          result;

    /* clip the request */
    if (self->size > 0) {
        if (bytes_to_read > self->size - self->position) {
            bytes_to_read = self->size - self->position;
            if (bytes_to_read == 0) {
                if (bytes_read != NULL) *bytes_read = 0;
                return ATX_ERROR_EOS;
            }
        }
    }

    /* read from the parent */
    result = ATX_InputStream_Read(self->parent, 
                                  buffer, 
                                  bytes_to_read, 
                                  &local_read);
    if (ATX_SUCCEEDED(result)) {
        /* transform if there is a transformer */
        if (self->transformer) {
            ATX_StreamTransformer_Transform(self->transformer, buffer, local_read);
        }

        /* update the position */
        self->position += local_read;

        /* return the number of bytes read */
        if (bytes_read) *bytes_read = local_read;
        return ATX_SUCCESS;
    } else {
        if (bytes_read) *bytes_read = 0;
        return result;
    }
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_Seek
+---------------------------------------------------------------------*/
ATX_METHOD
ATX_SubInputStream_Seek(ATX_InputStream* _self, 
                        ATX_Offset       where)
{
    ATX_SubInputStream* self = ATX_SELF(ATX_SubInputStream, ATX_InputStream);
    ATX_Offset          parent_offset;
    ATX_Result          result;

    /* shortcut */
    if (where == self->position) return ATX_SUCCESS;

    /* compute the parent offset */
    parent_offset = where + self->offset;
    
    /* seek */
    result = ATX_InputStream_Seek(self->parent, parent_offset);
    if (ATX_SUCCEEDED(result)) {
        self->position = where;
    }
    return result;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_Tell
+---------------------------------------------------------------------*/
ATX_METHOD
ATX_SubInputStream_Tell(ATX_InputStream* _self, 
                        ATX_Offset*      where)
{
    ATX_SubInputStream* self = ATX_SELF(ATX_SubInputStream, ATX_InputStream);
    if (where) *where = self->position;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_GetSize
+---------------------------------------------------------------------*/
ATX_METHOD
ATX_SubInputStream_GetSize(ATX_InputStream* _self,
                           ATX_Size*        size)
{
    ATX_SubInputStream* self = ATX_SELF(ATX_SubInputStream, ATX_InputStream);
    if (size) *size = self->size;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_GetAvailable
+---------------------------------------------------------------------*/
ATX_METHOD
ATX_SubInputStream_GetAvailable(ATX_InputStream* _self,
                                ATX_Size*        available)
{
    ATX_SubInputStream* self = ATX_SELF(ATX_SubInputStream, ATX_InputStream);
    ATX_Size            max_possible;
    ATX_Size            parent_available = 0;
    ATX_Result          result;

    /* compute the max possible value */
    max_possible = self->size - self->offset;

    /* see how many bytes are available from the parent */
    result = ATX_InputStream_GetAvailable(self->parent,
                                          &parent_available);
    if (ATX_SUCCEEDED(result)) {
        *available = 
            parent_available < max_possible ? 
            parent_available : max_possible;
    } else {
        *available = 0;
    }
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_SubInputStream_GetInterface
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(ATX_SubInputStream)
    ATX_GET_INTERFACE_ACCEPT(ATX_SubInputStream, ATX_InputStream)
    ATX_GET_INTERFACE_ACCEPT(ATX_SubInputStream, ATX_Referenceable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

/*----------------------------------------------------------------------
|   ATX_InputStream interface
+---------------------------------------------------------------------*/
ATX_BEGIN_INTERFACE_MAP(ATX_SubInputStream, ATX_InputStream)
    ATX_SubInputStream_Read,
    ATX_SubInputStream_Seek,
    ATX_SubInputStream_Tell,
    ATX_SubInputStream_GetSize,
    ATX_SubInputStream_GetAvailable
};

/*----------------------------------------------------------------------
|   ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_REFERENCEABLE_INTERFACE(ATX_SubInputStream, reference_count)

