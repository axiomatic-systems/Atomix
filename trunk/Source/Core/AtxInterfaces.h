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

/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#ifdef __cplusplus
#define ATX_INTERFACE_ID_TYPE_MOD extern "C" const
#else
#define ATX_INTERFACE_ID_TYPE_MOD extern const
#endif /* __cplusplus */

#define ATX_OFFSET_OF(_member,_type) (ATX_POINTER_TO_LONG(&( ((_type *)0)->_member)))

#define ATX_SELF_O(_object, _self_type, _interface_type) \
    ( (_self_type *)( ((ATX_Byte*)(_object)) - ATX_OFFSET_OF(_interface_type##_Base, _self_type)) )

#define ATX_SELF(_self_type, _interface_type) ATX_SELF_O(_self, _self_type, _interface_type)

#define ATX_SELF_EX_O(_object, _self_type, _base_type, _interface_type)   \
( (_self_type *)( ((ATX_Byte*)(_object)) -                                \
    ATX_OFFSET_OF(_base_type##_Base._interface_type##_Base, _self_type)) )

#define ATX_SELF_EX(_self_type, _base_type, _interface_type) \
    ATX_SELF_EX_O(_self, _self_type, _base_type, _interface_type)

#define ATX_SELF_M(_member, _self_type, _interface_type) \
( (_self_type *)( ((ATX_Byte*)(_self)) -                 \
    ATX_OFFSET_OF(_member._interface_type##_Base, _self_type)) )

#define ATX_BASE(_object, _base) (_object)->_base##_Base

#define ATX_BASE_EX(_object, _parent_base, _base) \
    (_object)->_parent_base##_Base._base##_Base

#define ATX_DECLARE_INTERFACE(_iface)                                   \
ATX_INTERFACE_ID_TYPE_MOD ATX_InterfaceId ATX_INTERFACE_ID__##_iface;   \
typedef struct _iface##Interface _iface##Interface;                     \
typedef struct {                                                        \
    const _iface##Interface* interface;                                 \
} _iface;

#define ATX_BEGIN_INTERFACE_DEFINITION(_iface) struct _iface##Interface { \
    ATX_Object* (*GetInterface)(_iface*                instance,       \
                                const ATX_InterfaceId* id);     
#define ATX_END_INTERFACE_DEFINITION };

#define ATX_BEGIN_INTERFACE_IMPLEMENTATION(_iface,_class)       \
static const _iface##Interface _class##_##_class##Interface = { \
    _class##_GetInterface,                                      
#define ATX_END_INTERFACE_IMPLEMENTATION(_iface,_class) };

#define ATX_IMPLEMENTS(_iface) _iface _iface##_Base
#define ATX_EXTENDS(_class)    _class _class##_Base

/**
 * Returns the interface pointer of an object reference.
 */
#define ATX_INTERFACE(_object) ((_object)->interface)

/**
 * Returns the interface pointer of a class that implements
 * multiple interfaces.
 */
#define ATX_INTERFACE_C(_object,_iface) ((_object)->_iface##_Base.interface)

/**
 */
#define ATX_CAST(_object, _iface)                   \
(_iface*)ATX_INTERFACE(_object)->GetInterface(      \
    _object,                                        \
    &ATX_INTERFACE_ID__##_iface)

/**
 * Returns a reference to an interface ID constant
 */
#define ATX_INTERFACE_ID(_iface) ATX_INTERFACE_ID__##_iface

/**
 * Tests if two interface ID constants (ATX_InterfaceId type) are equal
 */
#define ATX_INTERFACE_IDS_EQUAL(_iface_a,_iface_b) \
(((_iface_a)->type == (_iface_b)->type) && ((_iface_a)->version == (_iface_b)->version))

#define ATX_DECLARE_GET_INTERFACE_IMPLEMENTATION(_class)                     \
static ATX_Object* _class##_GetInterface(_class*                self,        \
                                         const ATX_InterfaceId* id);

#define ATX_BEGIN_GET_INTERFACE_IMPLEMENTATION(_class)                       \
static ATX_Object* _class##_GetInterface(_class*                self,        \
                                  const ATX_InterfaceId* id)                 \
{                                                                            \
    if (ATX_INTERFACE_IDS_EQUAL(id, &ATX_INTERFACE_ID__ATX_Object)) {        \
        return (ATX_Object*)self;                                            \
    }

#define ATX_GET_INTERFACE_ACCEPT(_class, _iface)                             \
    else if (ATX_INTERFACE_IDS_EQUAL(id, &ATX_INTERFACE_ID__##_iface)) {     \
        return (ATX_Object*)&(self->_iface##_Base);                          \
    }

#define ATX_GET_INTERFACE_ACCEPT_EX(_class, _base, _iface)                   \
    else if (ATX_INTERFACE_IDS_EQUAL(id, &ATX_INTERFACE_ID__##_iface)) {     \
        return (ATX_Object*)&(self->_base##_Base._iface##_Base);             \
    }

#define ATX_END_GET_INTERFACE_IMPLEMENTATION                                 \
    else {                                                                   \
        return NULL;                                                         \
    }                                                                        \
}                                                                            \

#define ATX_IMPLEMENT_GET_INTERFACE_ADAPTER(_class,_iface)                   \
static ATX_Object*                                                           \
_class##_##_iface##_GetInterface(_iface* _self, const ATX_InterfaceId* id)   \
{                                                                            \
    return _class##_GetInterface(ATX_SELF(_class,_iface), id);               \
}

#define ATX_IMPLEMENT_GET_INTERFACE_ADAPTER_EX(_class,_base,_iface)          \
static ATX_Object*                                                           \
_class##_##_iface##_GetInterface(_iface* _self, const ATX_InterfaceId* id)   \
{                                                                            \
    return _class##_GetInterface(ATX_SELF_EX(_class,_base,_iface), id);      \
}

#define ATX_GET_INTERFACE_ADAPTER(_class, _iface) \
_class##_##_iface##_GetInterface

#define ATX_INTERFACE_MAP(_class, _iface) \
static const _iface##Interface _class##_##_iface##Interface

#define ATX_DECLARE_INTERFACE_MAP(_class, _iface) \
ATX_INTERFACE_MAP(_class, _iface);

#define ATX_BEGIN_INTERFACE_MAP(_class,_iface)      \
ATX_IMPLEMENT_GET_INTERFACE_ADAPTER(_class,_iface)  \
ATX_INTERFACE_MAP(_class,_iface) = {                \
    ATX_GET_INTERFACE_ADAPTER(_class,_iface), 

#define ATX_BEGIN_INTERFACE_MAP_EX(_class, _base, _iface)   \
ATX_IMPLEMENT_GET_INTERFACE_ADAPTER_EX(_class,_base,_iface) \
ATX_INTERFACE_MAP(_class,_iface) = {                        \
    ATX_GET_INTERFACE_ADAPTER(_class,_iface), 

#define ATX_END_INTERFACE_MAP    };
#define ATX_END_INTERFACE_MAP_EX };

#define ATX_SET_INTERFACE(_object, _class, _iface) \
(_object)->_iface##_Base.interface = & _class##_##_iface##Interface

#define ATX_SET_INTERFACE_EX(_object, _class, _base, _iface) \
(_object)->_base##_Base._iface##_Base.interface = & _class##_##_iface##Interface

/*----------------------------------------------------------------------
|       ATX_Object interface
+---------------------------------------------------------------------*/
/**
* Basic interface implemented by all objects in the framework.
*
* The ATX_Object interface is implemented by all objects
* in the framework and allows the client of an object to query the object
* and obtain an interface pointer to any interface implemented by that 
* object. This interface has a single method called GetInterface().
*/
ATX_DECLARE_INTERFACE(ATX_Object)
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Object)
ATX_END_INTERFACE_DEFINITION

#endif /* _ATX_INTERFACES_H_ */




