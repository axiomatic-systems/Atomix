/*****************************************************************
|
|      File: AtxInterfaces.h
|
|      Atomix - Interfaces
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header File for the ATX Interface Framework 
 */

#ifndef _ATX_INTERFACES_H_
#define _ATX_INTERFACES_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxDefs.h"
#include "AtxTypes.h"

/*----------------------------------------------------------------------
|       error codes
+---------------------------------------------------------------------*/
#define ATX_ERROR_NO_SUCH_INTERFACE (ATX_ERROR_BASE_INTERFACES - 0)
#define ATX_ERROR_INVALID_INTERFACE (ATX_ERROR_BASE_INTERFACES - 1)
#define ATX_ERROR_NO_SUCH_CLASS     (ATX_ERROR_BASE_INTERFACES - 2)

/*----------------------------------------------------------------------
|       Win32 adaptation
+---------------------------------------------------------------------*/
#if defined(WIN32)
#undef interface
#define interface iface
#endif /* WIN32 */

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
/**
 * Interface ID constant
 *
 * Each interface has a corresponding interface ID constant of this type,
 * with a unique interface type ID and a version number
 */
typedef struct {
    unsigned long type;     /**< unique type ID integer */
    unsigned long version;  /**< version integer        */
} ATX_InterfaceId;

/**
 * Generic opaque instance data structure
 */
typedef struct ATX_Instance ATX_Instance;

/**
 * Generic interface function table structure
 */
typedef struct ATX_Interface ATX_Interface;

/**
 * Generic untyped object reference
 */
typedef struct {
    ATX_Instance*        instance;  /**< object instance pointer  */
    const ATX_Interface* interface; /**< object interface pointer */
} ATX_Object;

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#ifdef __cplusplus
#define ATX_INTERFACE_ID_TYPE_MOD extern "C" const
#else
#define ATX_INTERFACE_ID_TYPE_MOD extern const
#endif /* __cplusplus */


#define ATX_DECLARE_INTERFACE(name)                                 \
ATX_INTERFACE_ID_TYPE_MOD ATX_InterfaceId ATX_INTERFACE_ID__##name; \
typedef struct name##Instance name##Instance;                       \
typedef struct name##Interface name##Interface;                     \
typedef struct {                                                    \
    name##Instance*        instance;                                \
    const name##Interface* interface;                               \
} name##Object, name;

#define ATX_BEGIN_INTERFACE_DEFINITION(name) struct name##Interface {   \
    ATX_Result (*GetInterface)(ATX_Instance*          instance,         \
                               const ATX_InterfaceId* id,               \
                               const ATX_Interface**  interface);     
#define ATX_END_INTERFACE_DEFINITION(name) };

/**
 * Returns the instance pointer of an object reference.
 */
#define ATX_INSTANCE(object) ((object)->instance)

/**
 * Returns the interface pointer of an object reference.
 */
#define ATX_INTERFACE(object) ((object)->interface)

/**
 * Calls the GetInterface() method of an object to get an interface pointer
 *
 * Call the GetInterface() method of and object for a given interface ID. 
 * This macro returns the return value of the GetInterface() method call.
 */
#define ATX_GET_INTERFACE(_object, _if_name, _interface) \
ATX_INTERFACE(_object)->GetInterface(                    \
    (ATX_Instance*)ATX_INSTANCE(_object),                \
    &ATX_INTERFACE_ID__##_if_name,                       \
    (const ATX_Interface**)(_interface))

/** 
 * Casts an object reference into a reference to the same object with a
 * different interface.
 *
 * If the cast succeeds (i.e the object reference we are casting from is
 * a reference to an object that implements the interface we are requesting),
 * this macro evaluates to ATX_SUCCESS, else it evaluates to a negative
 * error code (typically ATX_ERROR_NO_SUCH_INTERFACE)
 */
#define ATX_CAST_OBJECT(_from_object, _to_object, _if_name)                \
ATX_INTERFACE(_from_object)->GetInterface(                                 \
        (ATX_Instance*)                                                    \
        (ATX_INSTANCE(_to_object) =                                        \
        ((_if_name##Instance*)ATX_INSTANCE(_from_object))),                \
        &ATX_INTERFACE_ID__##_if_name,                                     \
        (const ATX_Interface**)&ATX_INTERFACE(_to_object)) 

/**
 * Static initializer for NULL object references.
 */
#define ATX_NULL_OBJECT {0,0}

/**
 * Clear an object reference.
 * Clearing an object reference sets the instance and interface pointers
 * to NULL. When a reference has been cleared, ATX_INSTANCE_IS_NULL(ref)
 * evaluates to 'true'
 */
#define ATX_CLEAR_OBJECT(object) do {   \
    (object)->instance  = NULL;         \
    (object)->interface = NULL;         \
} while(0)

/**
 * Test if an object reference has a NULL instance pointer
 */
#define ATX_INSTANCE_IS_NULL(object) ((object)->instance == NULL)

/**
 * Test if an object reference has a NULL interface pointer
 */
#define ATX_INTERFACE_IS_NULL(object) ((object)->interface == NULL)

/**
 * Test if an object reference is a NULL reference
 */
#define ATX_OBJECT_IS_NULL(object) ATX_INTERFACE_IS_NULL(object)

/**
 * Returns a reference to an interface ID constant
 */
#define ATX_INTERFACE_ID(i) ATX_INTERFACE_ID__##i

/**
 * Tests if two interface ID constants (ATX_InterfaceId type) are equal
 */
#define ATX_INTERFACE_IDS_EQUAL(ia,ib) \
(((ia)->type == (ib)->type) && ((ia)->version == (ib)->version))

#define ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(_prefix)            \
ATX_METHOD _prefix##_GetInterface(ATX_Instance*         instance,           \
                                  const ATX_InterfaceId* id,                \
                                  const ATX_Interface**  interface);     

#define ATX_BEGIN_SIMPLE_GET_INTERFACE_IMPLEMENTATION(_prefix)               \
static const ATX_PolymorphicInterface _prefix##_ATX_PolymorphicInterface = { \
    _prefix##_GetInterface                                                   \
};                                                                           \
ATX_METHOD _prefix##_GetInterface(ATX_Instance*          instance,           \
                                  const ATX_InterfaceId* id,                 \
                                  const ATX_Interface**  interface)          \
{                                                                            \
    (void)instance;                                                          \
    if (ATX_INTERFACE_IDS_EQUAL(id, &ATX_INTERFACE_ID__ATX_Polymorphic)) {   \
        if (interface) {                                                     \
            *interface = (const ATX_Interface*)                              \
                         &(_prefix##_ATX_PolymorphicInterface);              \
        }                                                                    \
        return ATX_SUCCESS;                                                  \
    }

#define ATX_INTERFACE_MAP_ADD(_name_pfx, _interface_pfx)                     \
else if (ATX_INTERFACE_IDS_EQUAL(id, &ATX_INTERFACE_ID__##_interface_pfx)) { \
    if (interface) {                                                         \
        *interface = (const ATX_Interface*)                                  \
                     &(_name_pfx##_##_interface_pfx##Interface);             \
    }                                                                        \
    return ATX_SUCCESS;                                                      \
}

#define ATX_END_SIMPLE_GET_INTERFACE_IMPLEMENTATION(_prefix)                 \
    else {                                                                   \
        return ATX_ERROR_NO_SUCH_INTERFACE;                                  \
    }                                                                        \
}                                                                            \

/*----------------------------------------------------------------------
|       ATX_Polymorphic interface
+---------------------------------------------------------------------*/
/**
 * Basic interface implicitely implemented by all objects in the framework.
 *
 * The ATX_Polymorphic interface is implicitely implemented by all objects
 * in the framework, and allows the client of an object to query the object
 * and obtain an interface pointer to any interface implemented by that 
 * object. This interface has a single method called GetInterface().
 */
ATX_DECLARE_INTERFACE(ATX_Polymorphic)
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Polymorphic)
ATX_END_INTERFACE_DEFINITION(ATX_Polymorphic)

#endif /* _ATX_INTERFACES_H_ */




