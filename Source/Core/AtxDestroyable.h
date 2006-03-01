/*****************************************************************
|
|   File: AtxDestroyable.h
|
|   Atomix - Destroyable Interface
|
|   (c) 2002-2003 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Destroyable interface 
*/

#ifndef _ATX_DESTROYABLE_H_
#define _ATX_DESTROYABLE_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|   ATX_Destroyable interface
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_Destroyable)
/**
 * Interface implemented by objects that can be destroyed.
 */
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Destroyable)
    /**
     * Destroys the object instance.
     * After destroying an instance, that instance can no longer be used
     * (clients can no longer call any of the methods on that instance)
     * @param self Pointer the the object on which this method 
     * is called
     * @atx_method_result
     */
    ATX_Result (*Destroy)(ATX_Destroyable* self);
ATX_END_INTERFACE_DEFINITION(ATX_Destroyable)

/*----------------------------------------------------------------------
|   convenience macros
+---------------------------------------------------------------------*/
/**
 * Convenience macro used to call the Destroy() method on objects 
 * that implement the ATX_Destroyable interface 
 */
#define ATX_Destroyable_Destroy(object) \
ATX_INTERFACE(object)->Destroy(ATX_INSTANCE(object))

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
/**
 * Macro to destroy an object through the ATX_Destroyable interface
 *
 * This macro will first try to get an ATX_Destroyable intrerface for an
 * object. If the object does not implement the ATX_Destroyable interface,
 * an exception is thrown. If the object implements the ATX_Destroyble interface,
 * the macro calls the Destroy() method.
 * As a side effect, this macro clears its object reference argument, 
 * making it a NULL object reference.
 */
#define ATX_DESTROY_OBJECT(object)                                        \
do {                                                                      \
    if (object) {                                                         \
        ATX_Destroyable* destroyable = ATX_CAST(object, ATX_Destroyable); \
        ATX_ASSERT(destroyable != NULL);                                  \
        ATX_INTERFACE(destroyable)->Destroy(destroyable);                 \
        object = NULL;                                                    \
    }                                                                     \
} while(0)  

#define ATX_IMPLEMENT_DESTROYABLE_INTERFACE(_class)            \
ATX_IMPLEMENT_GET_INTERFACE_ADAPTER(_class, ATX_Destroyable)   \
static const ATX_DestroyableInterface                          \
_class##_ATX_DestroyableInterface = {                          \
    ATX_GET_INTERFACE_ADAPTER(_class, ATX_Destroyable),        \
    _class##_Destroy                                           \
};         
 
#endif /* _ATX_DESTROYABLE_H_ */





