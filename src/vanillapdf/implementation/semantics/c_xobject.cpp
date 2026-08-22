#include "precompiled.h"
#include "semantics/objects/xobject.h"
#include "semantics/objects/document.h"
#include "semantics/objects/rectangle.h"
#include "semantics/objects/resource_dictionary.h"

#include "vanillapdf/semantics/c_xobject.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION XObject_GetXObjectType(XObjectHandle* handle, XObjectType* result)
{
    XObjectBase* obj = reinterpret_cast<XObjectBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetXObjectType()) {
    case XObjectBase::Type::Form:
        *result = XObjectType_Form; break;
    case XObjectBase::Type::Image:
        *result = XObjectType_Image; break;
    case XObjectBase::Type::PostScript:
        *result = XObjectType_PostScript; break;
    default:
        return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION XObject_GetBaseObject(XObjectHandle* handle, ObjectHandle** result)
{
    XObjectBase* obj = reinterpret_cast<XObjectBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto base_object = obj->GetObject();
        auto ptr = base_object.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XObject_ToUnknown(XObjectHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<XObjectBase, IUnknown, XObjectHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XObject_FromUnknown(IUnknownHandle* handle, XObjectHandle** result) {
    return SafeObjectConvert<IUnknown, XObjectBase, IUnknownHandle, XObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XObject_Release(XObjectHandle* handle) {
    return ObjectRelease<XObjectBase, XObjectHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_CreateFromDocument(DocumentHandle* handle, FormXObjectHandle** result)
{
    Document* document = reinterpret_cast<Document*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto form = FormXObject::Create(document);
        auto ptr = form.AddRefGet();
        *result = reinterpret_cast<FormXObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetBoundingBox(FormXObjectHandle* handle, RectangleHandle** result)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr bounding_box;
        bool contains = obj->GetBoundingBox(bounding_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = bounding_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetBoundingBox(FormXObjectHandle* handle, RectangleHandle* value)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    Rectangle* rect = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect);

    try
    {
        obj->SetBoundingBox(rect);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetMatrix(FormXObjectHandle* handle, ArrayObjectHandle** result)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr matrix;
        bool contains = obj->GetMatrix(matrix);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = matrix.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetMatrix(FormXObjectHandle* handle, ArrayObjectHandle* value)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    MixedArrayObject* matrix = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(matrix);

    try
    {
        obj->SetMatrix(matrix);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_GetResources(FormXObjectHandle* handle, ResourceDictionaryHandle** result)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputResourceDictionaryPtr resources;
        bool contains = obj->GetResources(resources);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = resources.AddRefGet();
        *result = reinterpret_cast<ResourceDictionaryHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_SetResources(FormXObjectHandle* handle, DictionaryObjectHandle* value)
{
    FormXObject* obj = reinterpret_cast<FormXObject*>(handle);
    DictionaryObject* resources = reinterpret_cast<DictionaryObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(resources);

    try
    {
        obj->SetResources(resources);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_ToBaseXObject(FormXObjectHandle* handle, XObjectHandle** result) {
    return SafeObjectConvert<FormXObject, XObjectBase, FormXObjectHandle, XObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_FromBaseXObject(XObjectHandle* handle, FormXObjectHandle** result) {
    return SafeObjectConvert<XObjectBase, FormXObject, XObjectHandle, FormXObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FormXObject_Release(FormXObjectHandle* handle) {
    return ObjectRelease<FormXObject, FormXObjectHandle>(handle);
}
