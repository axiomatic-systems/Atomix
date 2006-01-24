/*****************************************************************
|
|      Atomix - Data Buffers
|
|      (c) 2001-2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxDataBuffer.h"
#include "AtxErrors.h"

/*----------------------------------------------------------------------
|       ATX_DataBuffer_Create
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_Create(ATX_Size size, ATX_DataBuffer** buffer)
{
    ATX_COMPILER_UNUSED(size);
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_Create
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_Destroy(ATX_DataBuffer* buffer)
{
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_SetBuffer
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_SetBuffer(ATX_DataBuffer* buffer,
                         ATX_Byte*       buffer_memory, 
                         ATX_Size        buffer_size)
{
    ATX_COMPILER_UNUSED(buffer);
    ATX_COMPILER_UNUSED(buffer_memory);
    ATX_COMPILER_UNUSED(buffer_size);

    /* not implemented yet */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_SetBufferSize
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_SetBufferSize(ATX_DataBuffer* buffer,
                             ATX_Size        buffer_size)
{
    ATX_COMPILER_UNUSED(buffer);
    ATX_COMPILER_UNUSED(buffer_size);

    /* not implemented yet */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_GetBufferSize
+---------------------------------------------------------------------*/
ATX_Size   
ATX_DataBuffer_GetBufferSize(ATX_DataBuffer* buffer)
{
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return 0;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_GetData
+---------------------------------------------------------------------*/
const ATX_Byte*  
ATX_DataBuffer_GetData(const ATX_DataBuffer* buffer)
{
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return NULL;
}
/*----------------------------------------------------------------------
|       ATX_DataBuffer_UseData
+---------------------------------------------------------------------*/
ATX_Byte* 
ATX_DataBuffer_UseData(ATX_DataBuffer* buffer)
{
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return NULL;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_GetDataSize
+---------------------------------------------------------------------*/
ATX_Size   
ATX_DataBuffer_GetDataSize(const ATX_DataBuffer* buffer)
{
    ATX_COMPILER_UNUSED(buffer);

    /* not implemented yet */
    return 0;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_SetDataSize
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_SetDataSize(ATX_DataBuffer* buffer, ATX_Size size)
{
    ATX_COMPILER_UNUSED(buffer);
    ATX_COMPILER_UNUSED(size);

    /* not implemented yet */
    return ATX_FAILURE;
}

/*----------------------------------------------------------------------
|       ATX_DataBuffer_SetData
+---------------------------------------------------------------------*/
ATX_Result 
ATX_DataBuffer_SetData(ATX_DataBuffer* buffer, 
                       ATX_Byte*       data,
                       ATX_Size        data_size)
{
    ATX_COMPILER_UNUSED(buffer);
    ATX_COMPILER_UNUSED(data);
    ATX_COMPILER_UNUSED(data_size);

    /* not implemented yet */
    return ATX_FAILURE;
}
