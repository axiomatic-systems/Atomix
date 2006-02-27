/*****************************************************************
|
|      File: AtxReferenceable.h
|
|      Atomix - Referenceable Interface
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Referenceable interface 
 */

#ifndef _ATX_REFERENCEABLE_H_
#define _ATX_REFERENCEABLE_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|       ATX_Referenceable interface
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_Referenceable)
/**
 * Interface implemented by object that maintain a reference counter that
 * counts how many clients are referencing the object. The counter can be
 * incremented by calling AddReference(), and decremented by calling
 * Release(). When the reference counter reaches 0, the object is destroyed
 * and cannot be used anymore.
 */
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Referenceable)
    /**
     * Increments the reference counter.
     * @param instance Instance pointer of the object on which this method 
     * is called
     * @atx_method_result
     */
    ATX_Result (*AddReference)(ATX_ReferenceableInstance* instance);
    /**
     * Decrements the reference counter.
     * @param instance Instance pointer of the object on which this method 
     * is called
     * @atx_method_result
     */
    ATX_Result (*Release)(ATX_ReferenceableInstance* instance);
ATX_END_INTERFACE_DEFINITION(ATX_Referenceable)

/*----------------------------------------------------------------------
|       convenience macros
+---------------------------------------------------------------------*/
/**
 * Convenience macro used to call the AddReference() method on objects 
 * that implement the ATX_Referenceable interface 
 */
#define ATX_Referenceable_AddReference(object) \
ATX_INTERFACE(object)->AddReference(ATX_INSTANCE(object))

/**
 * Convenience macro used to call the Release() method on objects 
 * that implement the ATX_Referenceable interface 
 */
#define ATX_Referenceable_ReleaseReference(object) \
ATX_INTERFACE(object)->ReleaseReference(ATX_INSTANCE(object))

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
/**
 * Macro used to safely release a reference on an object.
 *
 * This macro will try to get an ATX_Referenceable interface for the
 * object. If the object does not implement that interface, nothing is done.
 * If the object implements that interface, the macro will call the Release()
 * method.
 * As a side effect, this macro will also clear its object reference to turn 
 * it into a NULL object reference, so that further use of that object 
 * reference may be prevented
 */
#define ATX_RELEASE_OBJECT(object)                                      \
do {                                                                    \
    if ((object)->instance &&                                           \
        (object)->interface &&                                          \
        (object)->interface->GetInterface) {                            \
        ATX_ReferenceableInterface* interface;                          \
        if ((object)->interface->GetInterface(                          \
            (ATX_Instance*)((object)->instance),                        \
            &ATX_INTERFACE_ID__ATX_Referenceable,                       \
            (const ATX_Interface**)(void*)&interface) == ATX_SUCCESS) { \
            interface->Release((ATX_ReferenceableInstance*)             \
                               ((object)->instance));                   \
        } else {                                                        \
            ATX_ASSERT(0); /* not referenceable */                      \
        }                                                               \
        ATX_CLEAR_OBJECT(object);                                       \
    }                                                                   \
} while(0)

/**
 * Macro used to safely add a reference to an object.
 *
 * This macro will try to get an ATX_Referenceable interface for the
 * object. If the object does not implement that interface, nothing is done.
 * If the object implements that interface, the macro will call the 
 * AddReference() method.
 */
#define ATX_REFERENCE_OBJECT(object)                                    \
do {                                                                    \
    if ((object)->instance &&                                           \
        (object)->interface &&                                          \
        (object)->interface->GetInterface) {                            \
        ATX_ReferenceableInterface* interface;                          \
        if ((object)->interface->GetInterface(                          \
            (ATX_Instance*)((object)->instance),                        \
            &ATX_INTERFACE_ID__ATX_Referenceable,                       \
            (const ATX_Interface**)(void*)&interface) == ATX_SUCCESS) { \
            interface->AddReference((ATX_ReferenceableInstance*)        \
                                    (object)->instance);                \
        } else {                                                        \
            ATX_ASSERT(0); /* not referenceable */                      \
        }                                                               \
    }                                                                   \
} while(0)

#define ATX_IMPLEMENT_SIMPLE_REFERENCEABLE_INTERFACE(_prefix, _counter)     \
ATX_METHOD _prefix##_AddReference(ATX_ReferenceableInstance* instance)      \
{                                                                           \
    _prefix* me = (_prefix*)instance;                                       \
    me->_counter++;                                                         \
    return ATX_SUCCESS;                                                     \
}                                                                           \
ATX_METHOD _prefix##_Release(ATX_ReferenceableInstance* instance)           \
{                                                                           \
    _prefix* me = (_prefix*)instance;                                       \
    if (--me->_counter == 0) {                                              \
        _prefix##_Destroy(me);                                              \
    }                                                                       \
    return ATX_SUCCESS;                                                     \
}                                                                           \
static const ATX_ReferenceableInterface                                     \
_prefix##_ATX_ReferenceableInterface = {                                    \
    _prefix##_GetInterface,                                                 \
    _prefix##_AddReference,                                                 \
    _prefix##_Release                                                       \
};         
 
#endif /* _ATX_REFERENCEABLE_H_ */





