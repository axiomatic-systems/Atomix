/*****************************************************************
|
|   Atomix - JSON
|
|   (c) 2002-2009 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _ATX_JSON_H_
#define _ATX_JSON_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "AtxTypes.h"
#include "AtxDefs.h"
#include "AtxResults.h"
#include "AtxUtils.h"
#include "AtxInterfaces.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
typedef struct ATX_Json ATX_Json;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
typedef enum {
    ATX_JSON_TYPE_OBJECT = 0,
    ATX_JSON_TYPE_STRING,
    ATX_JSON_TYPE_NUMBER,
    ATX_JSON_TYPE_ARRAY,
    ATX_JSON_TYPE_BOOLEAN,
    ATX_JSON_TYPE_NULL
} ATX_JsonType;

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ATX_Json*         ATX_Json_CreateArray(void);
ATX_Json*         ATX_Json_CreateObject(void);
ATX_Json*         ATX_Json_CreateString(const char* value);
ATX_Json*         ATX_Json_CreateNumber(double number);
ATX_Json*         ATX_Json_CreateBoolean(ATX_Boolean value);
ATX_Json*         ATX_Json_CreateNull(void);
void              ATX_Json_Destroy(ATX_Json* self);
ATX_Json*         ATX_Json_GetChild(ATX_Json* self, const char* name);
ATX_Json*         ATX_Json_GetChildAt(ATX_Json* self, ATX_Ordinal indx);
ATX_Cardinal      ATX_Json_GetChildCount(ATX_Json* self);
ATX_Json*         ATX_Json_GetParent(ATX_Json* self);
ATX_Result        ATX_Json_AddChild(ATX_Json* self, ATX_Json* child);
ATX_JsonType      ATX_Json_GetType(ATX_Json* self);
ATX_Int32         ATX_Json_AsInteger(ATX_Json* self);
double            ATX_Json_AsDouble(ATX_Json* self);
ATX_Boolean       ATX_Json_AsBoolean(ATX_Json* self);
const ATX_String* ATX_Json_AsString(ATX_Json* self);

ATX_Result        ATX_Json_Parse(const char* serialized, ATX_Json** json);
ATX_Result        ATX_Json_ParseBuffer(const char* serialized, ATX_Size size, ATX_Json** json);
ATX_Result        ATX_Json_Serialize(ATX_Json* self, ATX_String* buffer, ATX_Boolean pretty);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ATX_JSON_H_ */
