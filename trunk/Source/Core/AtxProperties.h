/*****************************************************************
|
|   Atomix - Properties Interface
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Properties interface 
 */

#ifndef _ATX_PROPERTIES_H_
#define _ATX_PROPERTIES_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxTypes.h"
#include "AtxIterator.h"

/*----------------------------------------------------------------------
|   error codes
+---------------------------------------------------------------------*/
#define ATX_ERROR_NO_SUCH_PROPERTY       (ATX_ERROR_BASE_PROPERTIES - 0)
#define ATX_ERROR_NO_SUCH_LISTENER       (ATX_ERROR_BASE_PROPERTIES - 1)
#define ATX_ERROR_PROPERTY_TYPE_MISMATCH (ATX_ERROR_BASE_PROPERTIES - 2)

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
/**
 * Type of data represented by the 'value' field of an ATX_Property structure.
 */
typedef enum {
    ATX_PROPERTY_TYPE_NONE,     /**< There is no value                    */
    ATX_PROPERTY_TYPE_INTEGER,  /**< The value is an integer              */
    ATX_PROPERTY_TYPE_FLOAT,    /**< The value is a floating point number */
    ATX_PROPERTY_TYPE_STRING,   /**< The value is a string                */
    ATX_PROPERTY_TYPE_BOOLEAN,  /**< The value is a boolean               */
    ATX_PROPERTY_TYPE_RAW_DATA  /**< The value is a raw data block        */
} ATX_PropertyType;

typedef struct {
    ATX_Size size;
    ATX_Any  data;
} ATX_PropertyRawData;

/**
 * Union different possible types the 'value' field of an ATX_Property 
 * structure.
 */
typedef union {
    void*               any;      /**< Any pointer               */
    ATX_CString         string;   /**< A character string        */
    ATX_Int32           integer;  /**< An integer number         */
    ATX_Float           fp;       /**< A floating point number   */
    ATX_Boolean         boolean;  /**< A boolean value           */
    ATX_PropertyRawData raw_data; /**< A pointer to untyped data */
} ATX_PropertyValue;

typedef const void* ATX_PropertyListenerHandle;

/**
 * A property that has a name and a value (the value can be of one of several
 * types)
 */
typedef struct {
    ATX_CString       name;  /**< Name of the property              */
    ATX_PropertyType  type;  /**< Type of the value of the property */
    ATX_PropertyValue value; /**< Value of the property             */
} ATX_Property;

/*----------------------------------------------------------------------
|   ATX_PropertyListener interface
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_PropertyListener)
ATX_BEGIN_INTERFACE_DEFINITION(ATX_PropertyListener)
    void (*OnPropertyChanged)(ATX_PropertyListener*    self,
                              ATX_CString              name,
                              ATX_PropertyType         type,
                              const ATX_PropertyValue* value);
ATX_END_INTERFACE_DEFINITION

/*----------------------------------------------------------------------
|   ATX_Properties interface
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_Properties)
/**
 * Interface implemented by objects that want to expose a list of properties
 * to their clients. 
 *
 * Properties are name/value pairs (ATX_Property type), 
 * where the name is a string, and the value can be of one of several 
 * possible types (string, integer, etc...)
 */
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Properties)
    /**
     * Returns the property with a specified name in the property list.
     * @param self Pointer to the object on which this method is called
     * @param name Name of the property that is requested
     * @param property Address of the property variable where the property
     * will be returned
     * @return ATX_SUCCESS if the property with the specified name is found
     * and returned, ATX_ERROR_NO_SUCH_PROPERY if there is no property 
     * with that name in the list, or a negative ATX_Result error code 
     * if the call fails
     */
    ATX_Result (*GetProperty)(ATX_Properties* self,
                              ATX_CString     name,
                              ATX_Property*   property);

    /**
     * Sets the value of a property in a property list.
     * @param self Pointer to the object on which this method is called
     * @param name Name of the property to set
     * @param type Type of the property to set
     * @param value Pointer to the value of the property to set, or NULL
     * to unset the property (remove the property from the list)
     * @atx_method_result
     */
    ATX_Result (*SetProperty)(ATX_Properties*          self,
                              ATX_CString              name,
                              ATX_PropertyType         type,
                              const ATX_PropertyValue* value);

    /**
     * Unsets (removes) a property from a property list.
     * @param self Pointer to the object on which this method is called
     * @param name Name of the property to unset
     * @atx_method_result
     */
    ATX_Result (*UnsetProperty)(ATX_Properties* self,
                                ATX_CString     name);

    /**
     * Unsets all properties.
     * @param self Pointer to the object on which this method is called
     * @atx_method_result
     */
    ATX_Result (*Clear)(ATX_Properties* self);

    ATX_Result (*GetIterator)(ATX_Properties* self,
                              ATX_Iterator**  iterator);
#if 0
    ATX_Result (*AddAlias)(ATX_PropertiesInstance* instance,
                           ATX_CString             alias,
                           ATX_CString             name);

    ATX_Result (*RemoveAlias)(ATX_PropertiesInstance* instance,
                              ATX_CString             alias);

    /**
     * Returns an iterator over the aliases for a property name.
     * @param instance Instance pointer of the object on which this method 
     * is called
     * @param name Name of the property for which to iterate aliases
     * @param iterator Pointer to the object reference where the iterator
     * will be returned.
     * @atx_method_result
     */
    ATX_Result (*GetAliases)(ATX_PropertiesInstance* instance,
                             ATX_CString             name,
                             ATX_Iterator*           iterator);
#endif
    ATX_Result (*AddListener)(ATX_Properties*             self,
                              ATX_CString                 name,
                              ATX_PropertyListener*       listener, 
                              ATX_PropertyListenerHandle* handle);

    ATX_Result (*RemoveListener)(ATX_Properties*            self,
                                 ATX_PropertyListenerHandle handle);

ATX_END_INTERFACE_DEFINITION

/*----------------------------------------------------------------------
|   convenience macros
+---------------------------------------------------------------------*/
/**
 * Convenience macro used to call the GetProperty() method on objects 
 * that implement the ATX_Properties interface 
 */
#define ATX_Properties_GetProperty(object, name, property)  \
ATX_INTERFACE(object)->GetProperty(object,                  \
                                   name,                    \
                                   property)

/**
 * Convenience macro used to call the SetProperty() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_SetProperty(object, name, type, value)    \
ATX_INTERFACE(object)->SetProperty(object,                       \
                                   name,                         \
                                   type,                         \
                                   value)

/**
 * Convenience macro used to call the UnsetProperty() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_UnsetProperty(object, name)               \
ATX_INTERFACE(object)->UnsetProperty(object, name)

/**
 * Convenience macro used to call the Clear() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_Clear(object)                            \
ATX_INTERFACE(object)->Clear(object)

/**
 * Convenience macro used to call the GetIterator() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_GetIterator(object, iterator)               \
ATX_INTERFACE(object)->GetIterator(object, iterator)

/**
 * Convenience macro used to call the AddListener() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_AddListener(object, name, listener, handle) \
ATX_INTERFACE(object)->AddListener(object,                         \
                                   name,                           \
                                   listener,                       \
                                   handle)

/**
 * Convenience macro used to call the RemoveListener() method on objects that 
 * implement the ATX_Properties interface 
 */
#define ATX_Properties_RemoveListener(object, handle)    \
ATX_INTERFACE(object)->RemoveListener(object, handle)

/**
 * Convenience macro used to call the OnPropertyChanged() method on objects 
 * that implement the ATX_PropertyListener interface 
 */
#define ATX_PropertyListener_OnPropertyChanged(object, name, type, value) \
ATX_INTERFACE(object)->OnPropertyChanged(object,                          \
                                         name,                            \
                                         type,                            \
                                         value)

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Result ATX_Properties_Create(ATX_Properties** properties);

#ifdef __cplusplus
}
#endif

#endif /* _ATX_PROPERTIES_H_ */









