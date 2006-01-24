/*****************************************************************
|
|      File: AtxList.h
|
|      Atomix - Linked Lists
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_LIST_H_
#define _ATX_LIST_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxErrors.h"
#include "AtxUtils.h"
#include "AtxInterfaces.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct ATX_List ATX_List;
typedef struct ATX_ListItem ATX_ListItem;

/*----------------------------------------------------------------------
|    error codes
+---------------------------------------------------------------------*/
#define ATX_ERROR_NO_SUCH_ITEM   (ATX_ERROR_BASE_LISTS - 0)

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result    ATX_List_Create(ATX_List** list);
ATX_Result    ATX_List_Destroy(ATX_List* list);
ATX_ListItem* ATX_List_CreateItem(ATX_List* list);
ATX_Result    ATX_List_AddData(ATX_List* list, ATX_Any data);
ATX_Result    ATX_List_AddObject(ATX_List* list, ATX_Object* object);
ATX_Result    ATX_List_AddItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_InsertData(ATX_List* list, ATX_ListItem* where, ATX_Any data);
ATX_Result    ATX_List_InsertObject(ATX_List* list, ATX_ListItem* where, ATX_Object* object);
ATX_Result    ATX_List_InsertItem(ATX_List* list, ATX_ListItem* where, ATX_ListItem* item);
ATX_Result    ATX_List_RemoveData(ATX_List* list, ATX_Any data);
ATX_Result    ATX_List_RemoveObject(ATX_List* list, ATX_Object* object);
ATX_Result    ATX_List_RemoveItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_DetachItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_ReleaseObjects(ATX_List* list);
ATX_Cardinal  ATX_List_GetItemCount(ATX_List* list);
ATX_ListItem* ATX_List_GetFirstItem(ATX_List* list);
ATX_ListItem* ATX_List_GetLastItem(ATX_List* list);
ATX_ListItem* ATX_List_FindData(ATX_List* list, ATX_Any data);
ATX_ListItem* ATX_List_FindObject(ATX_List* list, ATX_Object* object);

ATX_Result    ATX_ListItem_Destroy(ATX_ListItem* item);
ATX_Any       ATX_ListItem_GetData(ATX_ListItem* item);
ATX_Result    ATX_ListItem_SetData(ATX_ListItem* item, ATX_Any data);
ATX_Result    ATX_ListItem_GetObject(ATX_ListItem* item, ATX_Object* object);
ATX_Result    ATX_ListItem_SetObject(ATX_ListItem* item, ATX_Object* object);
ATX_ListItem* ATX_ListItem_GetNext(ATX_ListItem* item);
ATX_ListItem* ATX_ListItem_GetPrev(ATX_ListItem* item);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_LIST_H_ */
