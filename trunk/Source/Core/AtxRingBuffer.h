/*****************************************************************
|
|      File: AtxRingBuffer.h
|
|      Atomix - Ring Buffers
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_RING_BUFFER_H_
#define _ATX_RING_BUFFER_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxInterfaces.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct ATX_RingBuffer ATX_RingBuffer;

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result     ATX_RingBuffer_Create(ATX_Size size, ATX_RingBuffer** buffer);
ATX_Result     ATX_RingBuffer_Destroy(ATX_RingBuffer* ring);
ATX_Size       ATX_RingBuffer_GetSpace(ATX_RingBuffer* ring);
ATX_Size       ATX_RingBuffer_GetContiguousSpace(ATX_RingBuffer* ring);
ATX_Size       ATX_RingBuffer_GetAvailable(ATX_RingBuffer* ring);
ATX_Size       ATX_RingBuffer_GetContiguousAvailable(ATX_RingBuffer* ring);
ATX_UInt8      ATX_RingBuffer_ReadByte(ATX_RingBuffer* ring);
ATX_UInt8      ATX_RingBuffer_PeekByte(ATX_RingBuffer* ring, 
                                       ATX_Size        offset);
ATX_ByteBuffer ATX_RingBuffer_GetIn(ATX_RingBuffer* ring);
ATX_ByteBuffer ATX_RingBuffer_GetOut(ATX_RingBuffer* ring);
ATX_Result     ATX_RingBuffer_Read(ATX_RingBuffer* ring, 
                                   ATX_ByteBuffer  buffer,
                                   ATX_Size        size);
ATX_Result     ATX_RingBuffer_Write(ATX_RingBuffer* ring, 
                                    ATX_ByteBuffer  buffer,
                                    ATX_Size        size);
ATX_Result     ATX_RingBuffer_MoveIn(ATX_RingBuffer* ring, ATX_Size size);
ATX_Result     ATX_RingBuffer_MoveOut(ATX_RingBuffer* ring, ATX_Size size);

#ifdef __cplusplus
}
#endif

#endif /* _ATX_RING_BUFFER_H_ */
