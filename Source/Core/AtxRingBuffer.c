/*****************************************************************
|
|   Atomix - Ring Buffer
|
|   (c) 2001-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxRingBuffer.h"
#include "AtxResults.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
struct ATX_RingBuffer {
    struct {
        unsigned char* start;
        unsigned char* end;
    } data;
    unsigned char* in;
    unsigned char* out;
    ATX_Size       size;
};

/*----------------------------------------------------------------------
|   ATX_RingBuffer_Create
+---------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_Create(ATX_Size size, ATX_RingBuffer** buffer)
{
    ATX_RingBuffer* ring;
    
    /* allocate a new object */
    ring = (ATX_RingBuffer*)ATX_AllocateMemory(sizeof(ATX_RingBuffer));
    *buffer = ring;
    if (ring == NULL) {
        return ATX_ERROR_OUT_OF_MEMORY;
    }
    
    /* construct the object */
    ring->size           = size;
    ring->data.start     = ATX_AllocateZeroMemory(size);
    ring->data.end       = ring->data.start + size;
    ring->in = ring->out = ring->data.start;

    /* check that everything is ok */
    if (ring->data.start == NULL) {
        ATX_FreeMemory((void*)ring);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_RingBuffer_Destroy
+---------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_Destroy(ATX_RingBuffer* ring)
{
    /* free the data buffer */
    if (ring->data.start) {
        ATX_FreeMemory((void*)ring->data.start);
    }

    /* free the object */
    ATX_FreeMemory((void*)ring);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_RingBuffer_GetContiguousSpace
+---------------------------------------------------------------------*/
ATX_Size
ATX_RingBuffer_GetContiguousSpace(ATX_RingBuffer* ring)
{
    return 
        (ring->in < ring->out) ?
        (ring->out - ring->in - 1) :
        ((ring->out == ring->data.start) ? 
         (ring->data.end - ring->in - 1) : 
         (ring->data.end - ring->in));
}

/*----------------------------------------------------------------------
|   ATX_RingBuffer_GetSpace
+---------------------------------------------------------------------*/
ATX_Size
ATX_RingBuffer_GetSpace(ATX_RingBuffer* ring)
{
    return 
        (ring->in < ring->out) ? 
        (ring->out - ring->in - 1) : 
        (ring->data.end - ring->in + ring->out - ring->data.start - 1);
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_Write
+----------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_Write(ATX_RingBuffer*      ring,
                     ATX_ByteBuffer       buffer, 
                     ATX_Size             byte_count)
{
    if (!byte_count) return ATX_SUCCESS;
    if (ring->in < ring->out) {
        if (buffer) {
            ATX_CopyMemory(ring->in, buffer, byte_count);
        }
        ring->in += byte_count;
        if (ring->in == ring->data.end) {
            ring->in = ring->data.start;
        }
    } else {
        unsigned int chunk = ring->data.end - ring->in;
        if (chunk >= byte_count) {
            chunk = byte_count;
        }

        if (buffer) {
            ATX_CopyMemory(ring->in, buffer, chunk);
        }
        ring->in += chunk;
        if (ring->in == ring->data.end) {
            ring->in = ring->data.start;
        }
        if (chunk != byte_count) {
            if (buffer) {
                ATX_CopyMemory(ring->in, buffer+chunk, byte_count-chunk);
            }
            ring->in += byte_count-chunk;
            if (ring->in == ring->data.end) {
                ring->in = ring->data.start;
            }
        }
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   ATX_RingBuffer_GetContiguousAvailable
+---------------------------------------------------------------------*/
ATX_Size
ATX_RingBuffer_GetContiguousAvailable(ATX_RingBuffer* ring)
{
    return 
        (ring->out <= ring->in) ? 
        (ring->in-ring->out) :
        (ring->data.end - ring->out);
}

/*----------------------------------------------------------------------
|   ATX_RingBuffer_GetAvailable
+---------------------------------------------------------------------*/
ATX_Size
ATX_RingBuffer_GetAvailable(ATX_RingBuffer* ring)
{
    return 
        (ring->out <= ring->in) ? 
        (ring->in-ring->out) :
        (ring->data.end - ring->out + ring->in - ring->data.start);
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_Read
+----------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_Read(ATX_RingBuffer* ring,
                    ATX_ByteBuffer  buffer, 
                    ATX_Size        byte_count)
{
    if (!byte_count) return ATX_SUCCESS;
    if (ring->in > ring->out) {
        if (buffer) {
            ATX_CopyMemory(buffer, ring->out, byte_count);
        }
        ring->out += byte_count;
        if (ring->out == ring->data.end) {
            ring->out = ring->data.start;
        }
    } else {
        unsigned int chunk = ring->data.end - ring->out;
        if (chunk >= byte_count) {
            chunk = byte_count;
        }

        if (buffer) {
            ATX_CopyMemory(buffer, ring->out, chunk);
        }
        ring->out += chunk;
        if (ring->out == ring->data.end) {
            ring->out = ring->data.start;
        }
        if (chunk != byte_count) {
            if (buffer) {
                ATX_CopyMemory(buffer+chunk, ring->out, byte_count-chunk);
            }
            ring->out += byte_count-chunk;
            if (ring->out == ring->data.end) {
                ring->out = ring->data.start;
            }
        }
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_ReadByte
+----------------------------------------------------------------------*/
ATX_UInt8
ATX_RingBuffer_ReadByte(ATX_RingBuffer* ring)
{
    unsigned char result = *ring->out++;
    if (ring->out == ring->data.end) {
        ring->out = ring->data.start;
    }
    return result;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_PeekByte
+----------------------------------------------------------------------*/
ATX_UInt8
ATX_RingBuffer_PeekByte(ATX_RingBuffer* ring, ATX_Size offset)
{
    unsigned char *where;

    where = ring->out+offset;
    if (where >= ring->data.end) {
        where -= (ring->data.end - ring->data.start);
    }

    return *where;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_GetIn
+----------------------------------------------------------------------*/
ATX_ByteBuffer 
ATX_RingBuffer_GetIn(ATX_RingBuffer* ring)
{
    return ring->in;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_GetOut
+----------------------------------------------------------------------*/
ATX_ByteBuffer 
ATX_RingBuffer_GetOut(ATX_RingBuffer* ring)
{
    return ring->out;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_MoveIn
+----------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_MoveIn(ATX_RingBuffer* ring, ATX_Size offset)
{
    int fold;

    ring->in += offset;
    fold = ring->in - ring->data.end;
    if (fold >= 0) {
        ring->in = ring->data.start + fold;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------+
|    ATX_RingBuffer_MoveOut
+----------------------------------------------------------------------*/
ATX_Result
ATX_RingBuffer_MoveOut(ATX_RingBuffer* ring, ATX_Size offset)
{
    int fold;

    ring->out += offset;
    fold = ring->out - ring->data.end;
    if (fold >= 0) {
        ring->out = ring->data.start + fold;
    }

    return ATX_SUCCESS;
}

