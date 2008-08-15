/*****************************************************************
|
|   Atomix - Referenceable Interface
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Referenceable interface 
 */

#ifndef _ATX_REFERENCEABLE_H_
#define _ATX_REFERENCEABLE_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxInterfaces.h"
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxDebug.h"

/*----------------------------------------------------------------------
|   ATX_Referenceable interface
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
     * @param self Pointer to the object on which this method is called
     * @atx_method_result
     */
    ATX_Result (*AddReference)(ATX_Referenceable* self);
    /**
     * Decrements the reference counter.
     * @param self Pointer to the object on which this method is called
     * @atx_method_result
     */
    ATX_Result (*Release)(ATX_Referenceable* self);
ATX_END_INTERFACE_DEFINITION

/*----------------------------------------------------------------------
|   convenience macros
+---------------------------------------------------------------------*/
/**
 * Convenience macro used to call the AddReference() method on objects 
 * that implement the ATX_Referenceable interface 
 */
#define ATX_Referenceable_AddReference(object) \
ATX_INTERFACE(object)->AddReference(object)

/**
 * Convenience macro used to call the Release() method on objects 
 * that implement the ATX_Referenceable interface 
 */
#define ATX_Referenceable_Release(object) \
ATX_INTERFACE(object)->Release(object)

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
/**
 * Macro used to safely release a reference on an object.
 *
 * This macro will try to get an ATX_Referenceable interface for the
 * object. If the object does not implement that interface, an exception is thrown.
 * If the object implements that interface, the macro will call the Release()
 * method.
 * As a side effect, this macro will also clear its object reference to turn 
 * it into a NULL object reference, so that further use of that object 
 * reference may be prevented
 */
#define ATX_RELEASE_OBJECT(object)                            \
do {                                                          \
    if (object) {                                             \
        ATX_Referenceable* referenceable =                    \
            ATX_CAST(object, ATX_Referenceable);              \
        ATX_ASSERT(referenceable != NULL);                    \
        ATX_Referenceable_Release(referenceable);             \
        object = NULL;                                        \
    }                                                         \
} while(0)

/**
 * Macro used to safely add a reference to an object.
 *
 * This macro will try to get an ATX_Referenceable interface for the
 * object. If the object does not implement that interface, an exception is thrown.
 * If the object implements that interface, the macro will call the 
 * AddReference() method.
 */
#define ATX_REFERENCE_OBJECT(object)                          \
do {                                                          \
    if (object) {                                             \
        ATX_Referenceable* referenceable =                    \
            ATX_CAST(object, ATX_Referenceable);              \
        ATX_ASSERT(referenceable != NULL);                    \
        ATX_Referenceable_AddReference(referenceable);        \
    }                                                         \
} while(0)

#define ATX_IMPLEMENT_REFERENCEABLE_INTERFACE(_class, _counter)    \
ATX_METHOD _class##_AddReference(ATX_Referenceable* _self)         \
{                                                                  \
    _class* self = ATX_SELF(_class, ATX_Referenceable);            \
    self->_counter++;                                              \
    return ATX_SUCCESS;                                            \
}                                                                  \
ATX_METHOD _class##_Release(ATX_Referenceable* _self)              \
{                                                                  \
    _class* self = ATX_SELF(_class, ATX_Referenceable);            \
    if (--self->_counter == 0) {                                   \
        _class##_Destroy(self);                                    \
    }                                                              \
    return ATX_SUCCESS;                                            \
}                                                                  \
ATX_BEGIN_INTERFACE_MAP(_class, ATX_Referenceable)                 \
    _class##_AddReference,                                         \
    _class##_Release                                               \
};         

#define ATX_IMPLEMENT_REFERENCEABLE_INTERFACE_EX(_class, _base, _counter) \
ATX_METHOD _class##_AddReference(ATX_Referenceable* _self)                \
{                                                                         \
    _class* self = ATX_SELF_EX(_class, _base, ATX_Referenceable);         \
    ATX_BASE(self, _base)._counter++;                                     \
    return ATX_SUCCESS;                                                   \
}                                                                         \
ATX_METHOD _class##_Release(ATX_Referenceable* _self)                     \
{                                                                         \
    _class* self = ATX_SELF_EX(_class, _base, ATX_Referenceable);         \
    if (--ATX_BASE(self, _base)._counter == 0) {                          \
        _class##_Destroy(self);                                           \
    }                                                                     \
    return ATX_SUCCESS;                                                   \
}                                                                         \
ATX_BEGIN_INTERFACE_MAP_EX(_class, _base, ATX_Referenceable)              \
    _class##_AddReference,                                                \
    _class##_Release                                                      \
};         

#endif /* _ATX_REFERENCEABLE_H_ */





