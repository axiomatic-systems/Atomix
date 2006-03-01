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
|    includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct ATX_DataBuffer ATX_DataBuffer;

/*----------------------------------------------------------------------
|    functions
+---------------------------------------------------------------------*/
ATX_Result ATX_DataBuffer_Create(ATX_Size size, ATX_DataBuffer** buffer);
ATX_Result ATX_DataBuffer_Destroy(ATX_DataBuffer* buffer);
ATX_Result ATX_DataBuffer_SetBuffer(ATX_DataBuffer* buffer,
                                    ATX_Byte*       buffer_memory, 
                                    ATX_Size        buffer_size);
ATX_Result ATX_DataBuffer_SetBufferSize(ATX_DataBuffer* buffer,
                                        ATX_Size        buffer_size);
ATX_Size   ATX_DataBuffer_GetBufferSize(const ATX_DataBuffer* buffer);
const ATX_Byte*  ATX_DataBuffer_GetData(const ATX_DataBuffer* buffer);
ATX_Byte*  ATX_DataBuffer_UseData(ATX_DataBuffer* buffer);
ATX_Size   ATX_DataBuffer_GetDataSize(const ATX_DataBuffer* buffer);
ATX_Result ATX_DataBuffer_SetDataSize(ATX_DataBuffer* buffer, ATX_Size size);
ATX_Result ATX_DataBuffer_SetData(ATX_DataBuffer* buffer, 
                                  const ATX_Byte* data,
                                  ATX_Size        data_size);

#endif /* _ATX_DATA_BUFFER_H_ */
