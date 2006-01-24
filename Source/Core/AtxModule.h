/*****************************************************************
|
|      File: AtxModule.h
|
|      Atomix - Module Interface
|
|      (c) 2002-2003 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file 
 * Header file for the ATX_Module interface 
 */

#ifndef _ATX_MODULE_H_
#define _ATX_MODULE_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxDefs.h"
#include "AtxTypes.h"
#include "AtxInterfaces.h"
#include "AtxProperties.h"

/*----------------------------------------------------------------------
|       ATX_Module interface
+---------------------------------------------------------------------*/
ATX_DECLARE_INTERFACE(ATX_Module)
/**
 * @brief Interface implemented by objects that create other objects
 *  
 * A Module object is responsible for creating object instance of a certain 
 * class. Module objects implement the ATX_Module interface, and clients that
 * want to create instances of that module will call the CreateObject method.
 */
ATX_BEGIN_INTERFACE_DEFINITION(ATX_Module)
    /** create an instance of the module that implements a given interface
     * @param instance Instance pointer of the object on which the method 
     * is called 
     * @param parameters Generic parameters used for constructing the object
     * @param interface_id Interface ID that the object needs to implement
     * @param object address of an object reference where the created object
     * will be returned if the call succeeds
     * @atx_method_result
     */
    ATX_Result (*CreateInstance)(ATX_ModuleInstance*    instance,
                                 ATX_String             parameters,
                                 const ATX_InterfaceId* interface_id,
                                 ATX_Object*            object);
ATX_END_INTERFACE_DEFINITION(ATX_Module)

/*----------------------------------------------------------------------
|       convenience macros
+---------------------------------------------------------------------*/
/** 
 * Convenience macro for calling the CreateObject method of the 
 * ATX_Clas interface.
 */
#define ATX_Module_CreateObject(object, parameters, interface_id, result) \
ATX_INTERFACE(object)->CreateObject(ATX_INSTANCE(object),                 \
                                    parameters,                           \
                                    interface_id,                         \
                                    result)

/*----------------------------------------------------------------------
|       macros and templates
+---------------------------------------------------------------------*/
#define ATX_DEFINE_NULL_MODULE_INSTANCE(_prefix) \
static ATX_ModuleInstance* const _prefix##ModuleInstance = NULL;

#define ATX_IMPLEMENT_SIMPLE_MODULE_INTERFACE(_refix)                        \
static const ATX_ModuleInterface _prefix##ModuleATX_ModuleInterface = {      \
    _prefix##Module_GetInterface,                                            \
    _prefix##Module_CreateInstance                                           \
};                                                                            

#define ATX_IMPLEMENT_SIMPLE_MODULE_PROPERTIES(_prefix)                    \
ATX_DECLARE_SIMPLE_GET_INTERFACE_IMPLEMENTATION(_prefix##Module)           \
ATX_IMPLEMENT_SIMPLE_STATIC_PROPERTIES_INTERFACE(_prefix##Module,          \
                                                 _prefix##ModuleProperties)

#endif /* _ATX_MODULE_H_ */




