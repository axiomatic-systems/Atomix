/*****************************************************************
|
|      File: AtxProperties.c
|
|      Atomix - Properties Interface
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxTypes.h"
#include "AtxUtils.h"
#include "AtxErrors.h"
#include "AtxDestroyable.h"
#include "AtxProperties.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct PropertyNode {
    ATX_Property         property;
    struct PropertyNode* next;
} PropertyNode;

typedef struct PropertyListenerNode {
    ATX_CString                  name;
    ATX_PropertyListener         listener;
    struct PropertyListenerNode* next;
} PropertyListenerNode;

typedef struct {
    PropertyNode*         property_nodes;
    PropertyListenerNode* listener_nodes;
} Properties;

typedef struct {
    Properties*   properties;
    PropertyNode* node;
} PropertiesIterator;

/*----------------------------------------------------------------------
|      forward declarations
+---------------------------------------------------------------------*/
static const ATX_PropertiesInterface Properties_ATX_PropertiesInterface;
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Properties)
static const ATX_IteratorInterface PropertiesIterator_ATX_IteratorInterface;
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(PropertiesIterator)

/*----------------------------------------------------------------------
|       PropertiesIterator_Create
+---------------------------------------------------------------------*/
static ATX_Result
PropertiesIterator_Create(Properties*   properties,
                          ATX_Iterator* object)
{
    PropertiesIterator* iterator;

    /* create a new object */
    iterator = (PropertiesIterator*)
        ATX_AllocateMemory(sizeof(PropertiesIterator));
    if (iterator == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }
    
    /* construct the object */
    iterator->properties = properties;
    iterator->node = properties->property_nodes;

    /* return an object reference */
    ATX_INSTANCE(object) = (ATX_IteratorInstance*)iterator;
    ATX_INTERFACE(object) = &PropertiesIterator_ATX_IteratorInterface;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       PropertiesIterator_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
PropertiesIterator_Destroy(ATX_DestroyableInstance* instance)
{
    ATX_FreeMemory(instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       PropertiesIterator_GetNext
+---------------------------------------------------------------------*/
ATX_METHOD
PropertiesIterator_GetNext(ATX_IteratorInstance* instance,
                           ATX_Any*              item)
{
    PropertiesIterator* iterator = (PropertiesIterator*)instance;
    if (iterator->node) {
        *item = (ATX_Any)iterator->node;
        iterator->node = iterator->node->next;
    } else {
        *item = (ATX_Any)NULL;
        return ATX_ERROR_END_OF_ITERATION;
    }
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       PropertiesIterator_Reset
+---------------------------------------------------------------------*/
ATX_METHOD
PropertiesIterator_Reset(ATX_IteratorInstance* instance)
{
    PropertiesIterator* iterator = (PropertiesIterator*)instance;

    /* reset to the first node */
    iterator->node = iterator->properties->property_nodes;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|      ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(PropertiesIterator)

/*----------------------------------------------------------------------
|      ATX_Iterator interface
+---------------------------------------------------------------------*/
static const ATX_IteratorInterface PropertiesIterator_ATX_IteratorInterface = {
    PropertiesIterator_GetInterface,
    PropertiesIterator_GetNext,
    PropertiesIterator_Reset
};

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(PropertiesIterator) 
ATX_INTERFACE_MAP_ADD(PropertiesIterator, ATX_Iterator)
ATX_INTERFACE_MAP_ADD(PropertiesIterator, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(PropertiesIterator)

/*----------------------------------------------------------------------
|       PropertyNode_SetValue
+---------------------------------------------------------------------*/
static ATX_Result
PropertyNode_SetValue(PropertyNode*            node,
                      ATX_PropertyType         type, 
                      const ATX_PropertyValue* value)
{
    node->property.type = type;
    switch (type) {
      case ATX_PROPERTY_TYPE_INTEGER:
      case ATX_PROPERTY_TYPE_FLOAT:
      case ATX_PROPERTY_TYPE_BOOLEAN:
        node->property.value = *value;
        break;

      case ATX_PROPERTY_TYPE_STRING:
        node->property.value.string = ATX_DuplicateString(value->string);
        break;

      case ATX_PROPERTY_TYPE_RAW_DATA:
        node->property.value.raw_data.size = value->raw_data.size;
        node->property.value.raw_data.data = 
            ATX_AllocateMemory(node->property.value.raw_data.size);
        if (node->property.value.raw_data.data == NULL) {
            return ATX_ERROR_OUT_OF_MEMORY;
        }
        ATX_CopyMemory(node->property.value.raw_data.data,
                       value->raw_data.data,
                       value->raw_data.size);
        break;

      default:
        break;
    }

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       PropertyNode_DestroyValue
+---------------------------------------------------------------------*/
static void
PropertyNode_DestroyValue(PropertyNode* node)
{
    /* destruct the node */
    switch (node->property.type) {
      case ATX_PROPERTY_TYPE_STRING:
        ATX_FreeMemory((void*)node->property.value.string);
        break;

      case ATX_PROPERTY_TYPE_RAW_DATA:
        if (node->property.value.raw_data.data) {
            ATX_FreeMemory((void*)node->property.value.raw_data.data);
        }
        break;

      default:
        break;
    }
}

/*----------------------------------------------------------------------
|       PropertyNode_Create
+---------------------------------------------------------------------*/
static PropertyNode*
PropertyNode_Create(ATX_CString              name, 
                    ATX_PropertyType         type, 
                    const ATX_PropertyValue* value)
{
    PropertyNode* node;

    /* allocate a new node */
    node = (PropertyNode*)ATX_AllocateMemory(sizeof(PropertyNode));
    if (node == NULL) return NULL;

    /* construct the node */
    node->next = NULL;
    node->property.name = ATX_DuplicateString(name);
    if (ATX_FAILED(PropertyNode_SetValue(node, type, value))) {
        ATX_FreeMemory((void*)node->property.name);
        ATX_FreeMemory((void*)node);
        return NULL;
    }

    return node;
}

/*----------------------------------------------------------------------
|       PropertyNode_Destroy
+---------------------------------------------------------------------*/
static void
PropertyNode_Destroy(PropertyNode* node)
{
    /* destruct the node */
    ATX_FreeMemory((void*)node->property.name);
    PropertyNode_DestroyValue(node);

    /* free the node */
    ATX_FreeMemory((void*)node);
}

/*----------------------------------------------------------------------
|       PropertyListenerNode_Create
+---------------------------------------------------------------------*/
static PropertyListenerNode*
PropertyListenerNode_Create(ATX_CString           name, 
                            ATX_PropertyListener* listener)
{
    PropertyListenerNode* node;

    /* allocate a new node */
    node = (PropertyListenerNode*)
        ATX_AllocateMemory(sizeof(PropertyListenerNode));
    if (node == NULL) return NULL;

    /* construct the node */
    node->next     = NULL;
    node->listener = *listener;
    if (name != NULL) {
        node->name = ATX_DuplicateString(name);
    } else {
        node->name = NULL;
    }

    return node;
}

/*----------------------------------------------------------------------
|       PropertyListenerNode_Destroy
+---------------------------------------------------------------------*/
static void
PropertyListenerNode_Destroy(PropertyListenerNode* node)
{
    /* destruct the node */
    if (node->name != NULL) {
        ATX_FreeMemory((void*)node->name);
    }

    /* free the node */
    ATX_FreeMemory((void*)node);
}

/*----------------------------------------------------------------------
|       Properties_NotifyListeners
+---------------------------------------------------------------------*/
static void
Properties_NotifyListeners(Properties*              properties,
                           ATX_CString              name,
                           ATX_PropertyType         type,
                           const ATX_PropertyValue* value)
{
    PropertyListenerNode* node;

    /* look for listeners registered for this name */
    node = properties->listener_nodes;
    while (node) {
        if (node->name == NULL || ATX_StringsEqual(node->name, name)) {
            ATX_PropertyListener_OnPropertyChanged(&node->listener,
                                                   name,
                                                   type,
                                                   value);
        }
        node = node->next;
    }
}

/*----------------------------------------------------------------------
|       Properties_FindProperty
+---------------------------------------------------------------------*/
static PropertyNode*
Properties_FindProperty(Properties* properties, ATX_CString name)
{
    PropertyNode* node; 

    /* find the node with that name */
    node = properties->property_nodes;
    while (node) {
        if (ATX_StringsEqual(node->property.name, name)) {
            /* match */
            return node;
        }
        node = node->next;
    }

    /* no match */
    return NULL;
}

/*----------------------------------------------------------------------
|       Properties_GetProperty
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_GetProperty(ATX_PropertiesInstance* instance,
                       ATX_CString             name,
                       ATX_Property*           property)
{
    Properties*   properties = (Properties*)instance;
    PropertyNode* node; 

    /* find the node with that name */
    node = Properties_FindProperty(properties, name);
    if (node) {
        *property = node->property;
        return ATX_SUCCESS;
    } else {
        return ATX_ERROR_NO_SUCH_PROPERTY;
    }
}

/*----------------------------------------------------------------------
|       Properties_SetProperty
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_SetProperty(ATX_PropertiesInstance*  instance,
                       ATX_CString              name,
                       ATX_PropertyType         type,
                       const ATX_PropertyValue* value)
{
    Properties*   properties = (Properties*)instance;
    PropertyNode* node;

    /* check parameters */
    if (name == NULL || value == NULL) {
        return ATX_ERROR_INVALID_PARAMETERS;
    }

    /* find the property with that name */
    node = Properties_FindProperty(properties, name);
    if (node) {
        /* a property with that name exists, check the type */
        if (node->property.type != type) {
            return ATX_ERROR_PROPERTY_TYPE_MISMATCH;
        }
        PropertyNode_DestroyValue(node);
        PropertyNode_SetValue(node, type, value);
    } else {
        /* no property with that name, create one */
        node = PropertyNode_Create(name, type, value);
        if (node == NULL) return ATX_ERROR_OUT_OF_MEMORY;

        /* add the node to the list */
        node->next = properties->property_nodes;
        properties->property_nodes = node;
    } 

    /* notify the listeners */
    Properties_NotifyListeners(properties, name, type, value);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Properties_UnsetProperty
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_UnsetProperty(ATX_PropertiesInstance* instance,
                         ATX_CString             name)
{
    Properties*   properties = (Properties*)instance;
    PropertyNode* node;
    PropertyNode* prev;

    /* find and remove the property */
    node = properties->property_nodes;
    prev = NULL;
    while (node) {
        if (ATX_StringsEqual(node->property.name, name)) {
            /* match */
            if (prev) {
                prev->next = node->next;
            } else {
                properties->property_nodes = node->next;
            }
    
            /* notify the listeners */
            Properties_NotifyListeners(properties, 
                                       name, 
                                       ATX_PROPERTY_TYPE_NONE, 
                                       NULL);

            /* destroy the node */
            PropertyNode_Destroy(node);
            return ATX_SUCCESS;
        }
        prev = node;
        node = node->next;
    }
    
    return ATX_ERROR_NO_SUCH_PROPERTY;
}

/*----------------------------------------------------------------------
|       Properties_Clear
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_Clear(ATX_PropertiesInstance* instance)
{
    Properties*   properties = (Properties*)instance;
    PropertyNode* property_node;

    /* notify the listeners */
    PropertyListenerNode* node = properties->listener_nodes;
    while (node) {
        ATX_PropertyListener_OnPropertyChanged(&node->listener, 
                                               node->name, 
                                               ATX_PROPERTY_TYPE_NONE, 
                                               NULL);
        node = node->next;
    }

    /* free property nodes */
    property_node = properties->property_nodes;
    while (property_node) {
        PropertyNode* next = property_node->next;
        PropertyNode_Destroy(property_node);
        property_node = next;
    }

    /* clear the head */
    properties->property_nodes = NULL;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Properties_GetIterator
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_GetIterator(ATX_PropertiesInstance* instance,
                       ATX_Iterator*           iterator)
{
    Properties* properties = (Properties*)instance;

    return PropertiesIterator_Create(properties, iterator);
}

/*----------------------------------------------------------------------
|       Properties_AddListener
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_AddListener(ATX_PropertiesInstance*     instance,
                       ATX_CString                 name,
                       ATX_PropertyListener*       listener, 
                       ATX_PropertyListenerHandle* handle)
{
    Properties*           properties = (Properties*)instance;
    PropertyListenerNode* node;

    /* create a new listener node */
    node = PropertyListenerNode_Create(name, listener);
    if (node == NULL) return ATX_ERROR_OUT_OF_MEMORY;

    /* add the listener to the list */
    node->next = properties->listener_nodes;
    properties->listener_nodes = node;

    /* return a handle */
    if (handle) *handle = (ATX_PropertyListenerHandle*)node;

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Properties_RemoveListener
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_RemoveListener(ATX_PropertiesInstance*    instance,
                          ATX_PropertyListenerHandle handle)
{
    Properties*           properties = (Properties*)instance;
    PropertyListenerNode* node;
    PropertyListenerNode* prev;

    /* look for the node with this handle */
    node = properties->listener_nodes;
    prev = NULL;
    while (node) {
        if (node == (PropertyListenerNode*)handle) {
            /* match */
            if (prev) {
                prev->next = node->next;
            } else {
                properties->listener_nodes = node->next;
            }
            PropertyListenerNode_Destroy(node);
            return ATX_SUCCESS;
        }
        prev = node;
        node = node->next;
    }

    return ATX_ERROR_NO_SUCH_LISTENER;
}

/*----------------------------------------------------------------------
|       Properties_Create
+---------------------------------------------------------------------*/
ATX_Result 
ATX_Properties_Create(ATX_Properties* object)
{
    Properties* properties;

    /* allocate the object */
    properties = (Properties*)ATX_AllocateMemory(sizeof(Properties));
    if (properties == NULL) {
        ATX_CLEAR_OBJECT(object);
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    properties->property_nodes = NULL;
    properties->listener_nodes = NULL;

    /* return an object reference */
    ATX_INSTANCE(object)  = (ATX_PropertiesInstance*)properties;
    ATX_INTERFACE(object) = &Properties_ATX_PropertiesInterface;
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Properties_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
Properties_Destroy(ATX_DestroyableInstance* instance)
{
    Properties*           properties = (Properties*)instance;
    PropertyNode*         property_node;
    PropertyListenerNode* listener_node;

    /* free property nodes */
    property_node = properties->property_nodes;
    while (property_node) {
        PropertyNode* next = property_node->next;
        PropertyNode_Destroy(property_node);
        property_node = next;
    }

    /* free the listener nodes */
    listener_node = properties->listener_nodes;
    while (listener_node) {
        PropertyListenerNode* next = listener_node->next;
        PropertyListenerNode_Destroy(listener_node);
        listener_node = next;
    } 

    /* free the object */
    ATX_FreeMemory((void*)instance);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|      ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(Properties)

/*----------------------------------------------------------------------
|      ATX_Properties interface
+---------------------------------------------------------------------*/
static const ATX_PropertiesInterface Properties_ATX_PropertiesInterface = {
    Properties_GetInterface,
    Properties_GetProperty,
    Properties_SetProperty,
    Properties_UnsetProperty,
    Properties_Clear,
    Properties_GetIterator,
    Properties_AddListener,
    Properties_RemoveListener
};

/*----------------------------------------------------------------------
|       standard GetInterface implementation
+---------------------------------------------------------------------*/
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Properties) 
ATX_INTERFACE_MAP_ADD(Properties, ATX_Properties)
ATX_INTERFACE_MAP_ADD(Properties, ATX_Destroyable)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Properties)











