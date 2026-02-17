#include "precompiled.h"
#include "semantics/utils/pdf_text_string.h"
#include "semantics/objects/actions.h"

#include "vanillapdf/semantics/c_actions.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Action_GetActionType(ActionHandle* handle, ActionType* result)
{
    ActionBase* obj = reinterpret_cast<ActionBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetActionType()) {
    case ActionBase::Type::GoTo:
        *result = ActionType_GoTo; break;
    case ActionBase::Type::GoToRemote:
        *result = ActionType_GoToRemote; break;
    case ActionBase::Type::URI:
        *result = ActionType_URI; break;
    case ActionBase::Type::Launch:
        *result = ActionType_Launch; break;
    case ActionBase::Type::Named:
        *result = ActionType_Named; break;
    case ActionBase::Type::JavaScript:
        *result = ActionType_JavaScript; break;
    default:
        return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Action_ToUnknown(ActionHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<ActionBase, IUnknown, ActionHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Action_FromUnknown(IUnknownHandle* handle, ActionHandle** result) {
    return SafeObjectConvert<IUnknown, ActionBase, IUnknownHandle, ActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Action_CreateFromDictionary(DictionaryObjectHandle* handle, ActionHandle** result)
{
    DictionaryObject* dict_obj = reinterpret_cast<DictionaryObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dict_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto action = ActionBase::Create(dict_obj);
        auto ptr = action.AddRefGet();
        *result = reinterpret_cast<ActionHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Action_Release(ActionHandle* handle) {
    return ObjectRelease<ActionBase, ActionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_GetDestination(GoToActionHandle* handle, DestinationHandle** result)
{
    GoToAction* obj = reinterpret_cast<GoToAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDestinationPtr destination;
        bool contains = obj->Destination(destination);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = destination.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_ToAction(GoToActionHandle* handle, ActionHandle** result) {
    return SafeObjectConvert<GoToAction, ActionBase, GoToActionHandle, ActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_FromAction(ActionHandle* handle, GoToActionHandle** result) {
    return SafeObjectConvert<ActionBase, GoToAction, ActionHandle, GoToActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToAction_Release(GoToActionHandle* handle) {
    return ObjectRelease<GoToAction, GoToActionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION URIAction_GetURI(URIActionHandle* handle, LiteralStringObjectHandle** result)
{
    URIAction* obj = reinterpret_cast<URIAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto value = obj->GetURI();
        auto ptr = value.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION URIAction_GetURIText(URIActionHandle* handle, PdfTextStringHandle** result)
{
    URIAction* obj = reinterpret_cast<URIAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        PdfTextStringPtr text_string;
        if (!obj->GetURIText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION URIAction_ToAction(URIActionHandle* handle, ActionHandle** result) {
    return SafeObjectConvert<URIAction, ActionBase, URIActionHandle, ActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION URIAction_FromAction(ActionHandle* handle, URIActionHandle** result) {
    return SafeObjectConvert<ActionBase, URIAction, ActionHandle, URIActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION URIAction_Release(URIActionHandle* handle) {
    return ObjectRelease<URIAction, URIActionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_GetDestination(GoToRemoteActionHandle* handle, DestinationHandle** result)
{
    GoToRemoteAction* obj = reinterpret_cast<GoToRemoteAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDestinationPtr destination;
        bool contains = obj->Destination(destination);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = destination.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_GetFile(GoToRemoteActionHandle* handle, ObjectHandle** result)
{
    GoToRemoteAction* obj = reinterpret_cast<GoToRemoteAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto value = obj->GetFile();
        auto ptr = value.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_ToAction(GoToRemoteActionHandle* handle, ActionHandle** result) {
    return SafeObjectConvert<GoToRemoteAction, ActionBase, GoToRemoteActionHandle, ActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_FromAction(ActionHandle* handle, GoToRemoteActionHandle** result) {
    return SafeObjectConvert<ActionBase, GoToRemoteAction, ActionHandle, GoToRemoteActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION GoToRemoteAction_Release(GoToRemoteActionHandle* handle) {
    return ObjectRelease<GoToRemoteAction, GoToRemoteActionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_GetName(NamedActionHandle* handle, NameObjectHandle** result)
{
    NamedAction* obj = reinterpret_cast<NamedAction*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto value = obj->GetName();
        auto ptr = value.AddRefGet();
        *result = reinterpret_cast<NameObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_ToAction(NamedActionHandle* handle, ActionHandle** result) {
    return SafeObjectConvert<NamedAction, ActionBase, NamedActionHandle, ActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_FromAction(ActionHandle* handle, NamedActionHandle** result) {
    return SafeObjectConvert<ActionBase, NamedAction, ActionHandle, NamedActionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedAction_Release(NamedActionHandle* handle) {
    return ObjectRelease<NamedAction, NamedActionHandle>(handle);
}
