/*****************************************************************
|
|   Atomix - Linked Lists
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "AtxConfig.h"
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxList.h"
#include "AtxReferenceable.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
struct ATX_ListItem {
    ATX_Any       data;
    ATX_ListItem* next;
    ATX_ListItem* prev;
};

struct ATX_List {
    ATX_Cardinal  item_count;
    ATX_ListItem* head;
    ATX_ListItem* tail;
};

/*----------------------------------------------------------------------
|    ATX_List_Create
+---------------------------------------------------------------------*/
ATX_Result 
ATX_List_Create(ATX_List** list)
{
    /* allocate memory for the object */
    *list = ATX_AllocateZeroMemory(sizeof(ATX_List));
    if (*list == NULL) {
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the objects */
    /* (all fields are zero-ed out by the allocator) */

    /* done */
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_Destroy
+---------------------------------------------------------------------*/
ATX_Result
ATX_List_Destroy(ATX_List* list)
{
    if (list == NULL) return ATX_SUCCESS;

    /* destroy all items */
    ATX_List_Clear(list);

    /* destroy the list object */
    ATX_FreeMemory((void*)list);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_Clear
+---------------------------------------------------------------------*/
ATX_Result
ATX_List_Clear(ATX_List* list)
{
    ATX_ListItem* item = list->head;

    /* destroy all items */
    while (item) {
        ATX_ListItem* next = item->next;
        ATX_FreeMemory(item);
        item = next;
    }

    /* reset item count and pointers */
    list->item_count = 0;
    list->head = NULL;
    list->tail = NULL;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_AddItem
+---------------------------------------------------------------------*/
ATX_Result 
ATX_List_AddItem(ATX_List* list, ATX_ListItem* item)
{
    /* add the item */
    if (list->tail) {
        item->prev = list->tail;
        item->next = NULL;
        list->tail->next = item;
        list->tail = item;
    } else {
        list->head = item;
        list->tail = item;
        item->next = NULL;
        item->prev = NULL;
    }

    /* one more item in the list now */
    list->item_count++;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_CreateItem
+---------------------------------------------------------------------*/
ATX_ListItem*
ATX_List_CreateItem(ATX_List* list)
{
    ATX_ListItem* item;

    /* avoid compiler warning about the unused parameter */
    ATX_COMPILER_UNUSED(list);

    /* allocate a new item */
    item = (ATX_ListItem*)ATX_AllocateMemory(sizeof(ATX_ListItem));
    if (item == NULL) {
        return NULL;
    }

    /* initialize the item */
    item->data = NULL;
    
    return item;
}

/*----------------------------------------------------------------------
|    ATX_List_AddData
+---------------------------------------------------------------------*/
ATX_Result 
ATX_List_AddData(ATX_List* list, ATX_Any data)
{
    ATX_ListItem* item;

    /* create a new item */
    item = ATX_List_CreateItem(list);
    if (item == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* attach the data to the item */
    item->data = data;

    /* add the item to the list */
    return ATX_List_AddItem(list, item);
}

/*----------------------------------------------------------------------
|    ATX_List_DetachItem
+---------------------------------------------------------------------*/
ATX_Result
ATX_List_DetachItem(ATX_List* list, ATX_ListItem* item)
{
    if (item->prev) {
        /* item is not the head */
        if (item->next) {
            /* item is not the tail */
            item->next->prev = item->prev;
            item->prev->next = item->next;
        } else {
            /* item is the tail */
            list->tail = item->prev;
            list->tail->next = NULL;
        }
    } else {
        /* item is the head */
        list->head = item->next;
        if (list->head) {
            /* item is not the tail */
            list->head->prev = NULL;
        } else {
            /* item is also the tail */
            list->tail = NULL;
        }
    }

    /* one less item in the list now */
    list->item_count--;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_RemoveItem
+---------------------------------------------------------------------*/
ATX_Result
ATX_List_RemoveItem(ATX_List* list, ATX_ListItem* item)
{
    /* pop the item out of the list */
    ATX_List_DetachItem(list, item);

    /* free the item memory */
    ATX_FreeMemory(item);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_List_RemoveData
+---------------------------------------------------------------------*/
ATX_Result    
ATX_List_RemoveData(ATX_List* list, ATX_Any data)
{
    ATX_ListItem* item = list->head;

    while (item) {
        if (item->data == data) {
            /* match, remove the item from the list */
            return ATX_List_RemoveItem(list, item);
        }
        item = item->next;
    }

    return ATX_ERROR_NO_SUCH_ITEM;
}

/*----------------------------------------------------------------------
|    ATX_List_GetFirstItem
+---------------------------------------------------------------------*/
ATX_ListItem*
ATX_List_GetFirstItem(ATX_List* list)
{
    return list->head;
}

/*----------------------------------------------------------------------
|    ATX_List_GetLastItem
+---------------------------------------------------------------------*/
ATX_ListItem*
ATX_List_GetLastItem(ATX_List* list)
{
    return list->tail;
}

/*----------------------------------------------------------------------
|    ATX_List_GetItem
+---------------------------------------------------------------------*/
ATX_ListItem*
ATX_List_GetItem(ATX_List* list, ATX_Ordinal index)
{
    ATX_ListItem* item = list->head;
    
    /* check the range */
    if (index >= list->item_count) return NULL;

    /* advance to the requested item */
    while (index--) item = item->next;

    return item;
}

/*----------------------------------------------------------------------
|    ATX_List_GetItemCount
+---------------------------------------------------------------------*/
ATX_Cardinal  
ATX_List_GetItemCount(ATX_List* list)
{
    return list->item_count;
}

/*----------------------------------------------------------------------
|    ATX_List_FindData
+---------------------------------------------------------------------*/
ATX_ListItem* 
ATX_List_FindData(ATX_List* list, ATX_Any data)
{
    ATX_ListItem* item = list->head;

    while (item) {
        if (item->data == data) {
            /* match, return the item */
            return item;
        }
        item = item->next;
    }

    return NULL;
}

/*----------------------------------------------------------------------
|    ATX_ListItem_GetData
+---------------------------------------------------------------------*/
ATX_Any
ATX_ListItem_GetData(ATX_ListItem* item)
{
    return item->data;
}

/*----------------------------------------------------------------------
|    ATX_ListItem_SetData
+---------------------------------------------------------------------*/
ATX_Result
ATX_ListItem_SetData(ATX_ListItem* item, ATX_Any data)
{
    item->data = data;
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|    ATX_ListItem_GetNext
+---------------------------------------------------------------------*/
ATX_ListItem* 
ATX_ListItem_GetNext(ATX_ListItem* item)
{
    return item->next;
}

/*----------------------------------------------------------------------
|    ATX_ListItem_GetPrev
+---------------------------------------------------------------------*/
ATX_ListItem* 
ATX_ListItem_GetPrev(ATX_ListItem* item)
{
    return item->prev;
}
