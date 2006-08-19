/*****************************************************************
|
|   Atomix - Data Buffers
|
|   (c) 2001-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_DATA_BUFFER_H_
#define _ATX_DATA_BUFFER_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct ATX_DataBuffer ATX_DataBuffer;

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result ATX_DataBuffer_Create(ATX_Size size, ATX_DataBuffer** buffer);
ATX_Result ATX_DataBuffer_Clone(const ATX_DataBuffer* self,
                                ATX_DataBuffer**      clone);
ATX_Result ATX_DataBuffer_Destroy(ATX_DataBuffer* self);
ATX_Result ATX_DataBuffer_SetBuffer(ATX_DataBuffer* self,
                                    ATX_Byte*       buffer_memory, 
                                    ATX_Size        buffer_size);
ATX_Result ATX_DataBuffer_SetBufferSize(ATX_DataBuffer* self,
                                        ATX_Size        buffer_size);
ATX_Size   ATX_DataBuffer_GetBufferSize(const ATX_DataBuffer* self);
ATX_Result ATX_DataBuffer_Reserve(ATX_DataBuffer* self,
                                  ATX_Size        buffer_size);
const ATX_Byte*  ATX_DataBuffer_GetData(const ATX_DataBuffer* self);
ATX_Byte*  ATX_DataBuffer_UseData(ATX_DataBuffer* self);
ATX_Size   ATX_DataBuffer_GetDataSize(const ATX_DataBuffer* self);
ATX_Result ATX_DataBuffer_SetDataSize(ATX_DataBuffer* self, ATX_Size size);
ATX_Result ATX_DataBuffer_SetData(ATX_DataBuffer* self, 
                                  const ATX_Byte* data,
                                  ATX_Size        data_size);
ATX_Boolean ATX_DataBuffer_Equals(const ATX_DataBuffer* self,
                                  const ATX_DataBuffer* other);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_DATA_BUFFER_H_ */
