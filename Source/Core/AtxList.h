/*****************************************************************
|
|   Atomix - Linked Lists
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_LIST_H_
#define _ATX_LIST_H_

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
typedef struct ATX_List ATX_List;
typedef struct ATX_ListItem ATX_ListItem;

typedef struct ATX_ListDataDestructor ATX_ListDataDestructor;
typedef struct ATX_ListDataDestructorInstance ATX_ListDataDestructorInstance;
struct ATX_ListDataDestructor {
    ATX_ListDataDestructorInstance* instance;
    void (*DestroyData)(ATX_ListDataDestructor* self, ATX_Any data, ATX_UInt32 type);
};

typedef struct ATX_ListDataPredicate ATX_ListDataPredicate;
typedef struct ATX_ListDataPredicateInstance ATX_ListDataPredicateInstance;
struct ATX_ListDataPredicate {
    ATX_ListDataPredicateInstance* instance;
    ATX_Boolean (*Evaluate)(const ATX_ListDataPredicate* self, ATX_Any data, ATX_UInt32 type);
};

/*----------------------------------------------------------------------
|    constants
+---------------------------------------------------------------------*/
#define ATX_LIST_ITEM_TYPE_UNKNOWN 0

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result    ATX_List_Create(ATX_List** list);
ATX_Result    ATX_List_CreateEx(const ATX_ListDataDestructor* destructor, ATX_List** list);
ATX_Result    ATX_List_Destroy(ATX_List* list);
ATX_Result    ATX_List_Clear(ATX_List* list);
ATX_ListItem* ATX_List_CreateItem(ATX_List* list);
ATX_Result    ATX_List_AddData(ATX_List* list, ATX_Any data);
ATX_Result    ATX_List_AddTypedData(ATX_List* list, ATX_Any data, ATX_UInt32 type);
ATX_Result    ATX_List_AddItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_InsertData(ATX_List* list, ATX_ListItem* where, ATX_Any data);
ATX_Result    ATX_List_InsertTypedData(ATX_List* list, ATX_ListItem* where, ATX_Any data, ATX_UInt32 type);
ATX_Result    ATX_List_InsertItem(ATX_List* list, ATX_ListItem* where, ATX_ListItem* item);
ATX_Result    ATX_List_RemoveData(ATX_List* list, ATX_Any data);
ATX_Result    ATX_List_RemoveItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_DetachItem(ATX_List* list, ATX_ListItem* item);
ATX_Result    ATX_List_DestroyItem(ATX_List* list, ATX_ListItem* item);
ATX_Cardinal  ATX_List_GetItemCount(ATX_List* list);
ATX_ListItem* ATX_List_GetFirstItem(ATX_List* list);
ATX_ListItem* ATX_List_GetLastItem(ATX_List* list);
ATX_ListItem* ATX_List_GetItem(ATX_List* list, ATX_Ordinal indx);
ATX_ListItem* ATX_List_FindData(ATX_List* list, ATX_Any data);
ATX_ListItem* ATX_List_Find(ATX_List* list, const ATX_ListDataPredicate* predicate);

ATX_Any       ATX_ListItem_GetData(ATX_ListItem* item);
ATX_Result    ATX_ListItem_SetData(ATX_ListItem* item, ATX_Any data);
ATX_UInt32    ATX_ListItem_GetType(ATX_ListItem* item);
ATX_Result    ATX_ListItem_SetType(ATX_ListItem* item, ATX_UInt32 type);
ATX_ListItem* ATX_ListItem_GetNext(ATX_ListItem* item);
ATX_ListItem* ATX_ListItem_GetPrev(ATX_ListItem* item);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_LIST_H_ */
