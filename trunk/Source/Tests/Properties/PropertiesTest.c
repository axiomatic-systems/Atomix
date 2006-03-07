/*****************************************************************
|
|      File: PropertiesTest.c
|
|      Atomix Tests - Properties
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxProperties.h"
#include "AtxDestroyable.h"
#include "AtxString.h"
#include "AtxUtils.h"
#include "AtxResults.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
    /* interfaces */
    ATX_IMPLEMENTS(ATX_PropertyListener);
    ATX_IMPLEMENTS(ATX_Destroyable);

    ATX_String name;
} Listener;

/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/
ATX_Property Properties[] = {
    {"Property 1", ATX_PROPERTY_TYPE_STRING,  {"Some String Value"}},
    {"Property 2", ATX_PROPERTY_TYPE_INTEGER, {(void*)0x23456789}},
    {"Property 3", ATX_PROPERTY_TYPE_FLOAT,   {(void*)0}},
    {"Property 4", ATX_PROPERTY_TYPE_BOOLEAN, {(void*)1}},
    {"Property 5", ATX_PROPERTY_TYPE_BOOLEAN, {(void*)0}},
    {"Property 6", ATX_PROPERTY_TYPE_INTEGER, {(void*)0x23456789}},
    {"Property 1", ATX_PROPERTY_TYPE_INTEGER, {(void*)7}}
};

/*----------------------------------------------------------------------
|       forward references
+---------------------------------------------------------------------*/
ATX_INTERFACE_MAP(Listener, ATX_PropertyListener);
ATX_INTERFACE_MAP(Listener, ATX_Destroyable);

/*----------------------------------------------------------------------
|       PrintProperty
+---------------------------------------------------------------------*/
static void
PrintProperty(ATX_CString name, ATX_PropertyType type, const ATX_PropertyValue* value)
{
    ATX_Debug("name=%s ", name);
    switch (type) {
      case ATX_PROPERTY_TYPE_INTEGER:
        ATX_Debug("[INTEGER] %d (%x)\n", value->integer, value->integer);
        break;

      case ATX_PROPERTY_TYPE_FLOAT:
        ATX_Debug("[FLOAT] %f\n", value->fp);
        break;

      case ATX_PROPERTY_TYPE_BOOLEAN:
        ATX_Debug("[BOOL] %s\n", 
                  value->boolean == ATX_TRUE ? "TRUE" : "FALSE");
        break;

      case ATX_PROPERTY_TYPE_STRING:
        ATX_Debug("[STRING] %s\n", value->string);
        break;

      case ATX_PROPERTY_TYPE_RAW_DATA:
        ATX_Debug("[DATA] %d bytes at %lx\n", 
                  value->raw_data.size, ATX_POINTER_TO_LONG(value->raw_data.data));
        break;

      default:
        ATX_Debug("[UNKNOWN]\n");
        break;
    }
}

/*----------------------------------------------------------------------
|       Listener_Create
+---------------------------------------------------------------------*/
ATX_METHOD
Listener_Create(ATX_CString            name,
                ATX_PropertyListener** object)
{
    /* allocate the object */
    Listener* listener = (Listener*)ATX_AllocateMemory(sizeof(Listener));
    if (listener == NULL) {
        *object = NULL;
        return ATX_ERROR_OUT_OF_MEMORY;
    }

    /* construct the object */
    listener->name = ATX_String_Create(name);

    /* setup the interfaces */
    ATX_SET_INTERFACE(listener, Listener, ATX_PropertyListener);
    ATX_SET_INTERFACE(listener, Listener, ATX_Destroyable);
    *object = &ATX_BASE(listener, ATX_PropertyListener);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Listener_Destroy
+---------------------------------------------------------------------*/
ATX_METHOD
Listener_Destroy(ATX_Destroyable* _self)
{
    Listener* self = ATX_SELF(Listener, ATX_Destroyable);
    ATX_String_Destruct(&self->name);
    ATX_FreeMemory((void*)self);

    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|       Listener_OnPropertyChanged
+---------------------------------------------------------------------*/
static void
Listener_OnPropertyChanged(ATX_PropertyListener*    _self,
                           ATX_CString              name,
                           ATX_PropertyType         type,
                           const ATX_PropertyValue* value)
{
    Listener* self = ATX_SELF(Listener, ATX_PropertyListener);

    ATX_Debug("OnPropertyChanged[%s]: ", ATX_CSTR(self->name));
    if (value) {
        PrintProperty(name, type, value);
    } else {
        ATX_Debug("name=%s [REMOVED]\n", name);
    }
}

/*----------------------------------------------------------------------
|       Listener interfaces
+---------------------------------------------------------------------*/
ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(Listener)
    ATX_GET_INTERFACE_ACCEPT(Listener, ATX_PropertyListener)
    ATX_GET_INTERFACE_ACCEPT(Listener, ATX_Destroyable)
ATX_END_GET_INTERFACE_IMPLEMENTATION

ATX_BEGIN_INTERFACE_MAP(Listener, ATX_PropertyListener)
    Listener_OnPropertyChanged
ATX_END_INTERFACE_MAP

ATX_IMPLEMENT_DESTROYABLE_INTERFACE(Listener)

/*----------------------------------------------------------------------
|       DumpProperties
+---------------------------------------------------------------------*/
static void
DumpProperties(ATX_Properties* properties)
{
    ATX_Iterator* iterator;
    ATX_Property* property;

    ATX_Debug("[PROPERTIES] -------------------------------\n");
    if (ATX_FAILED(ATX_Properties_GetIterator(properties, &iterator))) {
        return;
    }
    while (ATX_SUCCEEDED(ATX_Iterator_GetNext(iterator, 
                                              (ATX_Any*)(void*)&property))) {
        PrintProperty(property->name, property->type, &property->value);
    }
    ATX_Debug("--------------------------------------------\n");

    ATX_DESTROY_OBJECT(iterator);
}

/*----------------------------------------------------------------------
|       main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_Properties*            properties;
    ATX_PropertyListener*      listener0;
    ATX_PropertyListenerHandle listener0_handle = NULL;
    ATX_PropertyListener*      listener1;
    ATX_PropertyListenerHandle listener1_handle = NULL;
    ATX_PropertyListener*      listener2;
    ATX_PropertyListenerHandle listener2_handle = NULL;
    unsigned int               i;
    unsigned int               j;
    ATX_Result                 result;

    ATX_COMPILER_UNUSED(argc);
    ATX_COMPILER_UNUSED(argv);

    ATX_Debug("PropertiesTest -- Start\n");
    Listener_Create("Listener 0", &listener0);
    Listener_Create("Listener 1", &listener1);
    Listener_Create("Listener 2", &listener2);

    result = ATX_Properties_Create(&properties);

    Properties[2].value.fp = 0.123456789f;

    j = 0;
    for (i=0; i<10000; i++) {
        DumpProperties(properties);
        if (rand()&0x4) {
            ATX_Debug("** setting property '%s' [%d]\n", 
                      Properties[j].name, j);
            result = ATX_Properties_SetProperty(properties, 
                                                Properties[j].name,
                                                Properties[j].type,
                                                &Properties[j].value);
            ATX_Debug("(%d)\n", result);
        } else {
            ATX_Debug("&& unsetting property '%s' [%d]\n", 
                      Properties[j].name, j);
            result = ATX_Properties_UnsetProperty(properties,
                                                  Properties[j].name);
            ATX_Debug("(%d)\n", result);
        }
        j++;
        if (j >= sizeof(Properties)/sizeof(Properties[0])) {
            j = 0;
        }
        if (rand()%7 == 0) {
            int l = rand()%3;
            ATX_PropertyListener* listener;
            ATX_PropertyListenerHandle* listener_handle;
            if (l == 0) {
                listener = listener0;
                listener_handle = &listener0_handle;
            } else if (l == 1) {
                listener = listener1;
                listener_handle = &listener1_handle;
            } else {
                listener = listener2;
                listener_handle = &listener2_handle;
            }
            if (*listener_handle) {
                result = ATX_Properties_RemoveListener(properties, 
                                                       *listener_handle);
                ATX_Debug("## removed listener %d [%d]\n", 
                          l, result);
            }
            result = ATX_Properties_AddListener(properties, 
                                                Properties[j].name,
                                                listener,
                                                listener_handle);
            ATX_Debug("## added listener %d on %s [%d]\n", 
                      l, Properties[j].name, result);
        }
        ATX_Debug("++++++++++++++++++++++++++++++++++++++++++++\n");
    }

    ATX_DESTROY_OBJECT(properties);
    ATX_DESTROY_OBJECT(listener0);
    ATX_DESTROY_OBJECT(listener1);
    ATX_DESTROY_OBJECT(listener2);

    ATX_Debug("PropertiesTest -- End\n");

    return 0;
}

