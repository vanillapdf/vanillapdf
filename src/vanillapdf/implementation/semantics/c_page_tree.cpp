#include "precompiled.h"

#include "semantics/objects/page_tree.h"

#include "vanillapdf/semantics/c_page_tree.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle* handle, size_type* result)
{
    PageTree* obj = reinterpret_cast<PageTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->PageCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle* handle, size_type at, PageObjectHandle** result)
{
    PageTree* obj = reinterpret_cast<PageTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto page = obj->Page(at);
        auto ptr = page.AddRefGet();
        *result = reinterpret_cast<PageObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_InsertPage(PageTreeHandle* handle, size_type at, PageObjectHandle* page_handle)
{
    PageTree* obj = reinterpret_cast<PageTree*>(handle);
    PageObject* page = reinterpret_cast<PageObject*>(page_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(page);

    try
    {
        obj->Insert(page, at);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_AppendPage(PageTreeHandle* handle, PageObjectHandle* page_handle)
{
    PageTree* obj = reinterpret_cast<PageTree*>(handle);
    PageObject* page = reinterpret_cast<PageObject*>(page_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(page);

    try
    {
        obj->Append(page);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_RemovePage(PageTreeHandle* handle, size_type at)
{
    PageTree* obj = reinterpret_cast<PageTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->Remove(at);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_ToUnknown(PageTreeHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<PageTree, IUnknown, PageTreeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_FromUnknown(IUnknownHandle* handle, PageTreeHandle** result) {
    return SafeObjectConvert<IUnknown, PageTree, IUnknownHandle, PageTreeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle* handle)
{
    return ObjectRelease<PageTree, PageTreeHandle>(handle);
}
