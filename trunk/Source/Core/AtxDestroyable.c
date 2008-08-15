/*****************************************************************
|
|   Atomix - Destroyable Interface
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*-----------------------------------------------------------------
|   includes
+-----------------------------------------------------------------*/
#include "AtxDestroyable.h"

/*-----------------------------------------------------------------
|   interface stubs
+-----------------------------------------------------------------*/
ATX_Result 
ATX_Destroyable_Destroy(ATX_Destroyable* self)
{
    return ATX_INTERFACE(self)->Destroy(self);
}
