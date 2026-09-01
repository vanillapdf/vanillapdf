#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/transparency_group.h"
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

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetContents(PageObjectHandle* handle, PageContentsHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    PageContents* contents = reinterpret_cast<PageContents*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(contents);

    try
    {
        obj->SetContents(contents);
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

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetResources(PageObjectHandle* handle, ResourceDictionaryHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    ResourceDictionary* resources = reinterpret_cast<ResourceDictionary*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(resources);

    try
    {
        obj->SetResources(resources);
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

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetAnnotations(PageObjectHandle* handle, PageAnnotationsHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    PageAnnotations* annots = reinterpret_cast<PageAnnotations*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(annots);

    try
    {
        obj->SetAnnotations(annots);
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

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetCropBox(PageObjectHandle* handle, RectangleHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr crop_box;
        bool contains = obj->GetCropBox(crop_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = crop_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetCropBox(PageObjectHandle* handle, RectangleHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    Rectangle* crop_box = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(crop_box);

    try
    {
        obj->SetCropBox(crop_box);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetBleedBox(PageObjectHandle* handle, RectangleHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr bleed_box;
        bool contains = obj->GetBleedBox(bleed_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = bleed_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetBleedBox(PageObjectHandle* handle, RectangleHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    Rectangle* bleed_box = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(bleed_box);

    try
    {
        obj->SetBleedBox(bleed_box);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetTrimBox(PageObjectHandle* handle, RectangleHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr trim_box;
        bool contains = obj->GetTrimBox(trim_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = trim_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetTrimBox(PageObjectHandle* handle, RectangleHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    Rectangle* trim_box = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(trim_box);

    try
    {
        obj->SetTrimBox(trim_box);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetArtBox(PageObjectHandle* handle, RectangleHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr art_box;
        bool contains = obj->GetArtBox(art_box);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = art_box.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetArtBox(PageObjectHandle* handle, RectangleHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    Rectangle* art_box = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(art_box);

    try
    {
        obj->SetArtBox(art_box);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetRotate(PageObjectHandle* handle, bigint_type* result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        types::big_int rotate = 0;
        bool contains = obj->GetRotate(rotate);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        *result = rotate;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetRotate(PageObjectHandle* handle, bigint_type data)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetRotate(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetUserUnit(PageObjectHandle* handle, real_type* result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        types::real user_unit = 0;
        bool contains = obj->GetUserUnit(user_unit);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        *result = user_unit;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetUserUnit(PageObjectHandle* handle, real_type data)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetUserUnit(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetGroup(PageObjectHandle* handle, TransparencyGroupHandle** result)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputTransparencyGroupPtr group;
        bool contains = obj->GetGroup(group);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = group.AddRefGet();
        *result = reinterpret_cast<TransparencyGroupHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetGroup(PageObjectHandle* handle, TransparencyGroupHandle* value)
{
    PageObject* obj = reinterpret_cast<PageObject*>(handle);
    TransparencyGroup* group = reinterpret_cast<TransparencyGroup*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(group);

    try
    {
        obj->SetGroup(group);
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
