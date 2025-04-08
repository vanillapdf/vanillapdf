#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/document.h"

#include "vanillapdf/semantics/c_page_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle* handle, PageContentsHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputPageContentsPtr contents;
        auto contains = obj->GetContents(contents);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = contents.AddRefGet();
        *result = reinterpret_cast<PageContentsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle* handle, PageObjectHandle** result)
{
    Document* obj = reinterpret_cast<Document*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto page = PageObject::Create(obj);
        auto ptr = page.AddRefGet();
        *result = reinterpret_cast<PageObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle* handle, PageObjectHandle** result)
{
    syntax::DictionaryObject* obj = reinterpret_cast<syntax::DictionaryObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto page = PageObject::Create(obj);
        auto ptr = page.AddRefGet();
        *result = reinterpret_cast<PageObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle* handle, PageTreeHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto parent = obj->GetParent();
        auto ptr = parent.AddRefGet();
        *result = reinterpret_cast<PageTreeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle* handle, ResourceDictionaryHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
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

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle* handle, PageAnnotationsHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputPageAnnotationsPtr annots;
        bool contains = obj->GetAnnotations(annots);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = annots.AddRefGet();
        *result = reinterpret_cast<PageAnnotationsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle* handle, RectangleHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr media_box;
        bool contains = obj->GetMediaBox(media_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = media_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetMediaBox(PageObjectHandle* handle, RectangleHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    Rectangle* media_box = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(media_box);

    try
    {
        obj->SetMediaBox(media_box);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetBaseObject(PageObjectHandle* handle, DictionaryObjectHandle** result) {
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto base_object = obj->GetObject();
        auto ptr = base_object.AddRefGet();
        *result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_ToUnknown(PageObjectHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<PageObject, IUnknown, PageObjectHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_FromUnknown(IUnknownHandle* handle, PageObjectHandle** result) {
    return SafeObjectConvert<IUnknown, PageObject, IUnknownHandle, PageObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle* handle) {
    return ObjectRelease<PageObject, PageObjectHandle>(handle);
}
