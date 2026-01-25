#include "precompiled.h"

#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/tree.h"
#include "semantics/objects/destinations.h"
#include "syntax/objects/string_object.h"

#include "vanillapdf/semantics/c_destination_name_tree.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

// Type aliases for convenience
using DestinationNameTree = NameTree<DestinationPtr>;
using DestinationNameTreePtr = NameTreePtr<DestinationPtr>;

/**
 * Wrapper class for DestinationNameTree iterator that provides
 * First/Second/IsValid/Next interface compatible with C API pattern.
 */
class DestinationNameTreeIteratorImpl : public IUnknown {
public:
    using TreeIterator = DestinationNameTree::Iterator;

    DestinationNameTreeIteratorImpl(
        DestinationNameTreePtr tree,
        TreeIterator current,
        TreeIterator end)
        : m_tree(std::move(tree))
        , m_current(current)
        , m_end(end)
    {}

    syntax::StringObjectPtr First() {
        auto pair = *m_current;
        return pair.first;
    }

    DestinationPtr Second() {
        auto pair = *m_current;
        return pair.second;
    }

    bool IsValid() const {
        return m_current != m_end;
    }

    void Next() {
        ++m_current;
    }

private:
    DestinationNameTreePtr m_tree;  // Keep tree alive
    TreeIterator m_current;
    TreeIterator m_end;
};

// DestinationNameTree functions

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Create(DestinationNameTreeHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto tree = NameDictionary::CreateDestinationNameTree();
        auto ptr = tree.AddRefGet();
        *result = reinterpret_cast<DestinationNameTreeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Contains(DestinationNameTreeHandle* handle, StringObjectHandle* name, boolean_type* result)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    syntax::StringObjectBase* name_obj = reinterpret_cast<syntax::StringObjectBase*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        bool contains = obj->Contains(name_obj);
        *result = contains ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Find(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle** result)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    syntax::StringObjectBase* name_obj = reinterpret_cast<syntax::StringObjectBase*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto dest = obj->Find(name_obj);
        auto ptr = dest.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_TryFind(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle** result, boolean_type* found)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    syntax::StringObjectBase* name_obj = reinterpret_cast<syntax::StringObjectBase*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(found);

    try
    {
        OutputDestinationPtr dest;
        if (!obj->TryFind(name_obj, dest)) {
            *found = VANILLAPDF_RV_FALSE;
            return VANILLAPDF_ERROR_SUCCESS;
        }

        auto ptr = dest.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        *found = VANILLAPDF_RV_TRUE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Insert(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle* destination)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    syntax::StringObjectBase* name_obj = reinterpret_cast<syntax::StringObjectBase*>(name);
    DestinationBase* dest_obj = reinterpret_cast<DestinationBase*>(destination);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dest_obj);

    try
    {
        obj->Insert(name_obj, dest_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Remove(DestinationNameTreeHandle* handle, StringObjectHandle* name, boolean_type* removed)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    syntax::StringObjectBase* name_obj = reinterpret_cast<syntax::StringObjectBase*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(removed);

    try
    {
        bool result = obj->Remove(name_obj);
        *removed = result ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_GetIterator(DestinationNameTreeHandle* handle, DestinationNameTreeIteratorHandle** result)
{
    DestinationNameTree* obj = reinterpret_cast<DestinationNameTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // Create a Deferred pointer to the tree to keep it alive
        DestinationNameTreePtr tree_ptr = obj;

        auto it = make_deferred<DestinationNameTreeIteratorImpl>(
            tree_ptr,
            obj->begin(),
            obj->end()
        );

        auto ptr = it.AddRefGet();
        *result = reinterpret_cast<DestinationNameTreeIteratorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_ToUnknown(DestinationNameTreeHandle* handle, IUnknownHandle** result)
{
    return SafeObjectConvert<DestinationNameTree, IUnknown, DestinationNameTreeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_FromUnknown(IUnknownHandle* handle, DestinationNameTreeHandle** result)
{
    return SafeObjectConvert<IUnknown, DestinationNameTree, IUnknownHandle, DestinationNameTreeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Release(DestinationNameTreeHandle* handle)
{
    return ObjectRelease<DestinationNameTree, DestinationNameTreeHandle>(handle);
}

// DestinationNameTreeIterator functions

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_GetKey(DestinationNameTreeIteratorHandle* handle, StringObjectHandle** result)
{
    DestinationNameTreeIteratorImpl* obj = reinterpret_cast<DestinationNameTreeIteratorImpl*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto key = obj->First();
        auto ptr = key.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_GetValue(DestinationNameTreeIteratorHandle* handle, DestinationHandle** result)
{
    DestinationNameTreeIteratorImpl* obj = reinterpret_cast<DestinationNameTreeIteratorImpl*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto value = obj->Second();
        auto ptr = value.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_IsValid(DestinationNameTreeIteratorHandle* handle, boolean_type* result)
{
    DestinationNameTreeIteratorImpl* obj = reinterpret_cast<DestinationNameTreeIteratorImpl*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->IsValid() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_Next(DestinationNameTreeIteratorHandle* handle)
{
    DestinationNameTreeIteratorImpl* obj = reinterpret_cast<DestinationNameTreeIteratorImpl*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->Next();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_Release(DestinationNameTreeIteratorHandle* handle)
{
    return ObjectRelease<DestinationNameTreeIteratorImpl, DestinationNameTreeIteratorHandle>(handle);
}
