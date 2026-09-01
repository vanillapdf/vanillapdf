#include "precompiled.h"
#include "semantics/objects/transparency_group.h"

#include "vanillapdf/semantics/c_transparency_group.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_Create(TransparencyGroupHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        TransparencyGroupPtr group;
        auto ptr = group.AddRefGet();
        *result = reinterpret_cast<TransparencyGroupHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetIsolated(TransparencyGroupHandle* handle, boolean_type* result) {
    TransparencyGroup* obj = reinterpret_cast<TransparencyGroup*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        bool isolated = false;
        bool contains = obj->GetIsolated(isolated);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        *result = isolated ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_SetIsolated(TransparencyGroupHandle* handle, boolean_type value) {
    TransparencyGroup* obj = reinterpret_cast<TransparencyGroup*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try {
        obj->SetIsolated(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetKnockout(TransparencyGroupHandle* handle, boolean_type* result) {
    TransparencyGroup* obj = reinterpret_cast<TransparencyGroup*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        bool knockout = false;
        bool contains = obj->GetKnockout(knockout);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        *result = knockout ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_SetKnockout(TransparencyGroupHandle* handle, boolean_type value) {
    TransparencyGroup* obj = reinterpret_cast<TransparencyGroup*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try {
        obj->SetKnockout(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_GetBaseObject(TransparencyGroupHandle* handle, DictionaryObjectHandle** result) {
    TransparencyGroup* obj = reinterpret_cast<TransparencyGroup*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto base_object = obj->GetObject();
        auto ptr = base_object.AddRefGet();
        *result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_ToUnknown(TransparencyGroupHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<TransparencyGroup, IUnknown, TransparencyGroupHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_FromUnknown(IUnknownHandle* handle, TransparencyGroupHandle** result) {
    return SafeObjectConvert<IUnknown, TransparencyGroup, IUnknownHandle, TransparencyGroupHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TransparencyGroup_Release(TransparencyGroupHandle* handle) {
    return ObjectRelease<TransparencyGroup, TransparencyGroupHandle>(handle);
}
