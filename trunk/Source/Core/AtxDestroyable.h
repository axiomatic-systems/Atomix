/*****************************************************************
|
|      File: AtxDestroyable.h
|
|      Atomix - Destroyable Interface
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Destroyable interface 
*/

#ifndef _ATX_DESTROYABLE_H_
#define _ATX_DESTROYABLE_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       ATX_Destroyable interface
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
     * @param instance Instance pointer of the object on which this method 
     * is called
     * @atx_method_result
     */
    ATX_Result (*Destroy)(ATX_DestroyableInstance* instance);
ATX_END_INTERFACE_DEFINITION(ATX_Destroyable)

/*----------------------------------------------------------------------
|       convenience macros
+---------------------------------------------------------------------*/
/**
 * Convenience macro used to call the Destroy() method on objects 
 * that implement the ATX_Destroyable interface 
 */
#define ATX_Destroyable_Destroy(object) \
ATX_INTERFACE(object)->Destroy(ATX_INSTANCE(object))

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
/**
 * Macro to safely destroy an object through the ATX_Destroyable interface
 *
 * This macro will first try to get an ATX_Destroyable intrerface for an
 * object. If the object does not implement the ATX_Destroyable interface,
 * nothing is done. If the object implements the ATX_Destroyble interface,
 * the macro calls the Destroy() method.
 * As a side effect, this macro clears its object reference argument, 
 * making it a NULL object reference.
 */
#define ATX_DESTROY_OBJECT(object)                                     \
do {                                                                   \
    if ((object)->instance &&                                          \
        (object)->interface &&                                         \
        (object)->interface->GetInterface) {                           \
        ATX_DestroyableInterface* interface;                           \
        if ((object)->interface->GetInterface(                         \
            (ATX_Instance*)(void*)(object)->instance,                  \
            &ATX_INTERFACE_ID__ATX_Destroyable,                        \
            (const ATX_Interface**)(void*)&interface) == ATX_SUCCESS) {\
            interface->Destroy((ATX_DestroyableInstance*)              \
                               ((object)->instance));                  \
            } else {                                                   \
                ATX_ASSERT(0); /* not destroyable */                   \
            }                                                          \
        ATX_CLEAR_OBJECT(object);                                      \
    }                                                                  \
} while(0)

#define ATX_IMPLEMENT_SIMPLE_DESTROYABLE_INTERFACE(_prefix) \
static const ATX_DestroyableInterface                    \
_prefix##_ATX_DestroyableInterface = {                   \
    _prefix##_GetInterface,                              \
    _prefix##_Destroy                                    \
};         
 
#endif /* _ATX_DESTROYABLE_H_ */





