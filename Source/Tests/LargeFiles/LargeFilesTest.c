/*****************************************************************
|
|      Atomix Tests - Large Files
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Atomix.h"

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#define SHOULD_SUCCEED(r)                                   \
    do {                                                    \
        if (ATX_FAILED(r)) {                                \
            ATX_ConsoleOutputF("failed line %d (%d)\n", __LINE__, r);\
            return ATX_FAILURE;                             \
        }                                                   \
    } while(0)                                         

#define SHOULD_FAIL(r)                                                  \
    do {                                                                \
        if (ATX_SUCCEEDED(r)) {                                         \
            ATX_ConsoleOutputF("should have failed line %d (%d)\n", __LINE__, r);\
            return ATX_FAILURE;                                         \
        }                                                               \
    } while(0)                                  

#define SHOULD_EQUAL_I(a, b)                                           \
    do {                                                               \
        if ((a) != (b)) {                                              \
            ATX_ConsoleOutputF("got %lld, expected %lld line %lld\n", (ATX_Int64)a, (ATX_Int64)b, __LINE__);\
            return ATX_FAILURE;                                        \
        }                                                              \
    } while(0)                                                            

/*----------------------------------------------------------------------
|   globals
+---------------------------------------------------------------------*/
const unsigned int BUFFER_SIZE = 8192;
const unsigned int MAX_RANDOM = 123;
typedef struct {
    ATX_Position  position;
    unsigned int  size;
    unsigned char fill_value;
} BufferInfo;
const ATX_UInt64 TARGET_SIZE = 0x112345678;

/*----------------------------------------------------------------------
|   TestLargeFiles
+---------------------------------------------------------------------*/
static ATX_Result
TestLargeFiles(const char* filename)
{
    ATX_List*         buffer_list;
    unsigned int      buffer_count;
    BufferInfo**      buffers;
    unsigned char*    buffer = ATX_AllocateZeroMemory(BUFFER_SIZE);
    unsigned int      progress = 0;
    ATX_UInt64        total_size = 0;
    ATX_File*         test_file = NULL;
    ATX_OutputStream* output_stream = NULL;
    ATX_InputStream*  input_stream = NULL;
    ATX_Result        result;
    ATX_ListItem*     cursor = NULL;
    unsigned int      i;
    
    
    // create enough buffers to fill up to the target size
    ATX_List_Create(&buffer_list);
    while (total_size < TARGET_SIZE) {
        unsigned int random = ATX_System_GetRandomInteger() % MAX_RANDOM;
        unsigned int buffer_size = 4096-MAX_RANDOM/2+random;
        BufferInfo* buffer_info = ATX_AllocateZeroMemory(sizeof(BufferInfo));
        buffer_info->position = total_size;
        buffer_info->size = buffer_size;
        buffer_info->fill_value = ATX_System_GetRandomInteger()%256;
        ATX_List_AddData(buffer_list, buffer_info);
        
        total_size += buffer_size;
    }
    buffer_count = ATX_List_GetItemCount(buffer_list);
    buffers = ATX_AllocateZeroMemory(buffer_count*sizeof(BufferInfo*));
    cursor = ATX_List_GetFirstItem(buffer_list);
    for (i=0; cursor; cursor = ATX_ListItem_GetNext(cursor), i++) {
        buffers[i] = ATX_ListItem_GetData(cursor);
    }
    
    // write random buffers
    printf("Writing sequential random-size buffers\n");
    ATX_File_Create(filename, &test_file);
    
    result = ATX_File_Open(test_file, ATX_FILE_OPEN_MODE_WRITE | ATX_FILE_OPEN_MODE_CREATE | ATX_FILE_OPEN_MODE_TRUNCATE);
    SHOULD_SUCCEED(result);
    result = ATX_File_GetOutputStream(test_file, &output_stream);
    SHOULD_SUCCEED(result);
    
    for (i=0; i<buffer_count; i++) {
        BufferInfo*  buffer_info = buffers[i];
        ATX_Position where = 0;
        
        unsigned int new_progress = (100*i)/buffer_count;
        if (new_progress != progress) {
            printf("\rProgress: %d%%", new_progress);
            fflush(stdout);
            progress = new_progress;
        }
        
        ATX_SetMemory(buffer, buffer_info->fill_value, buffer_info->size);
        
        result = ATX_OutputStream_Tell(output_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position);
        
        result = ATX_OutputStream_WriteFully(output_stream, buffer, buffer_info->size);
        SHOULD_SUCCEED(result);
        
        if ((buffer_info->fill_value % 7) == 0) {
            ATX_OutputStream_Flush(output_stream);
        }
        
        result = ATX_OutputStream_Tell(output_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position+buffer_info->size);
    }
    
    ATX_RELEASE_OBJECT(output_stream);
    ATX_File_Close(test_file);
    
    // read random buffers
    printf("\nReading sequential random-size buffers\n");
    result = ATX_File_Open(test_file, ATX_FILE_OPEN_MODE_READ);
    SHOULD_SUCCEED(result);
    result = ATX_File_GetInputStream(test_file, &input_stream);
    SHOULD_SUCCEED(result);
    
    for (i=0; i<buffer_count; i++) {
        BufferInfo*  buffer_info = buffers[i];
        ATX_Position where = 0;
        unsigned int x;
    
        unsigned int new_progress = (100*i)/buffer_count;
        if (new_progress != progress) {
            printf("\rProgress: %d%%", new_progress);
            fflush(stdout);
            progress = new_progress;
        }

        result = ATX_InputStream_Tell(input_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position);
        
        result = ATX_InputStream_ReadFully(input_stream, buffer, buffer_info->size);
        SHOULD_SUCCEED(result);
        
        for (x=0; x<buffer_info->size; x++) {
            SHOULD_EQUAL_I(buffer[x], buffer_info->fill_value);
        }
        
        result = ATX_InputStream_Tell(input_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position+buffer_info->size);
    }
    ATX_RELEASE_OBJECT(input_stream);
    ATX_File_Close(test_file);

    // read random buffers
    printf("\nReading random-access random-size buffers\n");
    result = ATX_File_Open(test_file, ATX_FILE_OPEN_MODE_READ);
    SHOULD_SUCCEED(result);
    result = ATX_File_GetInputStream(test_file, &input_stream);
    SHOULD_SUCCEED(result);
    
    for (i=0; i<buffer_count*5; i++) {
        BufferInfo*  buffer_info;
        unsigned int buffer_index;
        ATX_Position where;
        unsigned int x;
        
        unsigned int new_progress = (100*i)/(5*buffer_count);
        if (new_progress != progress) {
            printf("\rProgress: %d%%", new_progress);
            fflush(stdout);
            progress = new_progress;
        }
    
        buffer_index = ATX_System_GetRandomInteger()%buffer_count;
        buffer_info = buffers[buffer_index];

        result = ATX_InputStream_Seek(input_stream, buffer_info->position);
        SHOULD_SUCCEED(result);

        result = ATX_InputStream_Tell(input_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position);
        
        result = ATX_InputStream_ReadFully(input_stream, buffer, buffer_info->size);
        SHOULD_SUCCEED(result);
        
        for (x=0; x<buffer_info->size; x++) {
            SHOULD_EQUAL_I(buffer[x], buffer_info->fill_value);
        }
        
        result = ATX_InputStream_Tell(input_stream, &where);
        SHOULD_SUCCEED(result);
        SHOULD_EQUAL_I(where, buffer_info->position+buffer_info->size);
    }

    printf("\nSUCCESS!\n");
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/
int
main(int argc, char** argv)
{
    const char* output_filename = "largefile.bin";
    if (argc > 1) {
        output_filename = argv[1];
    }

    TestLargeFiles(output_filename);
    
    return 0;
}


