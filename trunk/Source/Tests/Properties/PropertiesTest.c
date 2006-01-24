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
#include "Atomix.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct {
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
static const ATX_PropertyListenerInterface Listener_ATX_PropertyListenerInterface;

/*----------------------------------------------------------------------
|       PrintProperty
+---------------------------------------------------------------------*/
static void
PrintProperty(ATX_String name, ATX_PropertyType type, ATX_PropertyValue* value)
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
                  value->raw_data.size, (int)value->raw_data.data);
        break;

      default:
        ATX_Debug("[UNKNOWN]\n");
        break;
    }
}

/*----------------------------------------------------------------------
|       Listener_Create
+---------------------------------------------------------------------*/
static void
Listener_Create(ATX_String            name,
                ATX_PropertyListener* object)
{
    Listener* listener = ATX_AllocateMemory(sizeof(Listener));
    listener->name = ATX_DuplicateString(name);
    ATX_INSTANCE(object) = (ATX_PropertyListenerInstance*)listener;
    ATX_INTERFACE(object) = &Listener_ATX_PropertyListenerInterface;
}

/*----------------------------------------------------------------------
|       Listener_Destroy
+---------------------------------------------------------------------*/
static void
Listener_Destroy(ATX_PropertyListenerInstance* instance)
{
    Listener* listener = (Listener*)instance;
    ATX_FreeMemory((void*)listener->name);
    ATX_FreeMemory((void*)listener);
}

/*----------------------------------------------------------------------
|       Listener_OnPropertyChanged
+---------------------------------------------------------------------*/
static void
Listener_OnPropertyChanged(ATX_PropertyListenerInstance* instance,
                           ATX_String                    name,
                           ATX_PropertyType              type,
                           ATX_PropertyValue*            value)
{
    Listener* listener = (Listener*)instance;

    ATX_Debug("OnPropertyChanged[%s]: ", listener->name);
    if (value) {
        PrintProperty(name, type, value);
    } else {
        ATX_Debug("name=%s [REMOVED]\n", name);
    }
}

/*----------------------------------------------------------------------
|       Listener interfaces
+---------------------------------------------------------------------*/
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Listener)
static const ATX_PropertyListenerInterface Listener_ATX_PropertyListenerInterface = {
    Listener_GetInterface,
    Listener_OnPropertyChanged
};
ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Listener) 
ATX_INTERFACE_MAP_ADD(Listener, ATX_PropertyListener)
ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(Listener)

/*----------------------------------------------------------------------
|       DumpProperties
+---------------------------------------------------------------------*/
static void
DumpProperties(ATX_Properties* properties)
{
    ATX_Iterator  iterator;
    ATX_Property* property;

    ATX_Debug("[PROPERTIES] -------------------------------\n");
    if (ATX_FAILED(ATX_Properties_GetIterator(properties, &iterator))) {
        return;
    }
    while (ATX_SUCCEEDED(ATX_Iterator_GetNext(&iterator, 
                                              (ATX_Any*)&property))) {
        PrintProperty(property->name, property->type, &property->value);
    }
    ATX_Debug("--------------------------------------------\n");

    ATX_DESTROY_OBJECT(&iterator);
}

/*----------------------------------------------------------------------
|       main
+---------------------------------------------------------------------*/
int 
main(int argc, char** argv)
{
    ATX_Properties             properties;
    ATX_PropertyListener       listener0;
    ATX_PropertyListenerHandle listener0_handle = NULL;
    ATX_PropertyListener       listener1;
    ATX_PropertyListenerHandle listener1_handle = NULL;
    ATX_PropertyListener       listener2;
    ATX_PropertyListenerHandle listener2_handle = NULL;
    unsigned int         i;
    unsigned int         j;
    ATX_Result           result;

    ATX_COMPILER_UNUSED(argc);
    ATX_COMPILER_UNUSED(argv);

    ATX_Debug("PropertiesTest -- Start\n");
    Listener_Create("Listener 0", &listener0);
    Listener_Create("Listener 1", &listener1);
    Listener_Create("Listener 2", &listener2);

    result = ATX_Properties_Create(&properties);

    Properties[2].value.fp = 0.123456789;

    j = 0;
    for (i=0; i<10000; i++) {
        DumpProperties(&properties);
        if (rand()&0x4) {
            ATX_Debug("** setting property '%s' [%d]\n", 
                      Properties[j].name, j);
            result = ATX_Properties_SetProperty(&properties, 
                                                Properties[j].name,
                                                Properties[j].type,
                                                &Properties[j].value);
            ATX_Debug("(%d)\n", result);
        } else {
            ATX_Debug("&& unsetting property '%s' [%d]\n", 
                      Properties[j].name, j);
            result = ATX_Properties_UnsetProperty(&properties,
                                                  Properties[j].name);
            ATX_Debug("(%d)\n", result);
        }
        j++;
        if (j >= sizeof(Properties)/sizeof(Properties[0])) {
            j = 0;
        }
        if (rand()%7 == 0) {
            int l = rand()%3;
            ATX_PropertyListener listener;
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
                result = ATX_Properties_RemoveListener(&properties, 
                                                       *listener_handle);
                ATX_Debug("## removed listener %d [%d]\n", 
                          l, result);
            }
            result = ATX_Properties_AddListener(&properties, 
                                                Properties[j].name,
                                                &listener,
                                                listener_handle);
            ATX_Debug("## added listener %d on %s [%d]\n", 
                      l, Properties[j].name, result);
        }
        ATX_Debug("++++++++++++++++++++++++++++++++++++++++++++\n");
    }

    ATX_DESTROY_OBJECT(&properties);

    Listener_Destroy(ATX_INSTANCE(&listener0));
    Listener_Destroy(ATX_INSTANCE(&listener1));
    Listener_Destroy(ATX_INSTANCE(&listener2));

    ATX_Debug("PropertiesTest -- End\n");

    return 0;
}

