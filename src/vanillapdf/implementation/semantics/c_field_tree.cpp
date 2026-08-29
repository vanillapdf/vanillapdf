#include "precompiled.h"

#include "semantics/objects/field_tree.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/document.h"

#include "utils/buffer.h"

#include "vanillapdf/semantics/c_field_tree.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_CreateFromDocument(DocumentHandle* handle, FieldTreeHandle** result) {
    Document* document = reinterpret_cast<Document*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto tree = FieldTree::Create(document);
        auto ptr = tree.AddRefGet();
        *result = reinterpret_cast<FieldTreeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetFieldCount(FieldTreeHandle* handle, size_type* result) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = tree->GetFieldCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetField(FieldTreeHandle* handle, size_type index, FieldHandle** result) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto field = tree->GetField(index);
        auto ptr = field.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_FindField(FieldTreeHandle* handle, string_type qualified_name, size_type size, FieldHandle** result) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(qualified_name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OuputFieldPtr field;
        bool found = tree->TryFindField(std::string_view(qualified_name, size), field);
        if (!found) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = field.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetRootChildCount(FieldTreeHandle* handle, size_type* result) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = tree->GetRootChildCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetRootChild(FieldTreeHandle* handle, size_type index, FieldHandle** result) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto child = tree->GetRootChild(index);
        auto ptr = child.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_AddRootChild(FieldTreeHandle* handle, FieldHandle* child) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    Field* child_field = reinterpret_cast<Field*>(child);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(child_field);

    try {
        tree->AddRootChild(child_field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_InsertRootChild(FieldTreeHandle* handle, size_type index, FieldHandle* child) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    Field* child_field = reinterpret_cast<Field*>(child);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(child_field);

    try {
        tree->InsertRootChild(index, child_field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_AddChild(FieldTreeHandle* handle, FieldHandle* parent, FieldHandle* child) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    Field* parent_field = reinterpret_cast<Field*>(parent);
    Field* child_field = reinterpret_cast<Field*>(child);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(parent_field);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(child_field);

    try {
        tree->AddChild(parent_field, child_field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_InsertChild(FieldTreeHandle* handle, FieldHandle* parent, size_type index, FieldHandle* child) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    Field* parent_field = reinterpret_cast<Field*>(parent);
    Field* child_field = reinterpret_cast<Field*>(child);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(parent_field);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(child_field);

    try {
        tree->InsertChild(parent_field, index, child_field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_RemoveChild(FieldTreeHandle* handle, FieldHandle* field) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    Field* removed_field = reinterpret_cast<Field*>(field);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(removed_field);

    try {
        tree->RemoveChild(removed_field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_Invalidate(FieldTreeHandle* handle) {
    FieldTree* tree = reinterpret_cast<FieldTree*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(tree);

    try {
        tree->Invalidate();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_ToUnknown(FieldTreeHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<FieldTree, IUnknown, FieldTreeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_FromUnknown(IUnknownHandle* handle, FieldTreeHandle** result) {
    return SafeObjectConvert<IUnknown, FieldTree, IUnknownHandle, FieldTreeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_Release(FieldTreeHandle* handle) {
    return ObjectRelease<FieldTree, FieldTreeHandle>(handle);
}
