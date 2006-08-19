/*****************************************************************
|
|   Atomix - Maps
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
****************************************************************/

#ifndef _ATX_MAP_H_
#define _ATX_MAP_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxList.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct ATX_Map ATX_Map;
typedef struct ATX_MapEntry ATX_MapEntry;

typedef struct {
    ATX_Boolean is_set;
    ATX_Any     data;
    ATX_UInt32  type;
} ATX_MapEntryInfo;

/*----------------------------------------------------------------------
|    constants
+---------------------------------------------------------------------*/
#define ATX_MAP_ITEM_TYPE_UNKNOWN 0

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result    ATX_Map_Create(ATX_Map** map);
ATX_Result    ATX_Map_CreateEx(const ATX_ListDataDestructor* destructor, ATX_Map** map);
ATX_Result    ATX_Map_Destroy(ATX_Map* self);
ATX_Result    ATX_Map_Clear(ATX_Map* self);
ATX_Result    ATX_Map_Put(ATX_Map*          self, 
                          ATX_CString       key, 
                          ATX_Any           data, 
                          ATX_MapEntryInfo* previous);
ATX_Result    ATX_Map_PutTyped(ATX_Map*          self, 
                               ATX_CString       key, 
                               ATX_Any           data, 
                               ATX_UInt32        type,
                               ATX_MapEntryInfo* previous);
ATX_MapEntry* ATX_Map_Get(ATX_Map* self, const char* key);
ATX_Result    ATX_Map_Remove(ATX_Map* self, ATX_CString key, ATX_MapEntryInfo* entry_info);
ATX_Boolean   ATX_Map_HasKey(ATX_Map* self, ATX_CString key);
ATX_List*     ATX_Map_AsList(ATX_Map* self);

ATX_CString   ATX_MapEntry_GetKey(ATX_MapEntry* self);
ATX_Any       ATX_MapEntry_GetData(ATX_MapEntry* self);
ATX_Result    ATX_MapEntry_SetData(ATX_MapEntry* self, ATX_Any data);
ATX_UInt32    ATX_MapEntry_GetType(ATX_MapEntry* self);
ATX_Result    ATX_MapEntry_SetType(ATX_MapEntry* self, ATX_UInt32 type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_MAP_H_ */
