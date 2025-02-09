#include "precompiled.h"

#include "contents/content_stream_instruction_base.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_objects.h"

#include "vanillapdf/contents/c_content_instruction.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_GetInstructionType(ContentInstructionHandle* handle, ContentInstructionType* result) {
    InstructionBase* obj = reinterpret_cast<InstructionBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetInstructionType()) {
        case InstructionBase::Type::Object:
            *result = ContentInstructionType_Object; break;
        case InstructionBase::Type::Operation:
            *result = ContentInstructionType_Operation; break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle* handle, ContentOperationHandle** result) {
    return SafeObjectConvert<InstructionBase, OperationBase, ContentInstructionHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToUnknown(ContentInstructionHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<InstructionBase, IUnknown, ContentInstructionHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_FromUnknown(IUnknownHandle* handle, ContentInstructionHandle** result) {
    return SafeObjectConvert<IUnknown, InstructionBase, IUnknownHandle, ContentInstructionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle* handle) {
    return ObjectRelease<InstructionBase, ContentInstructionHandle>(handle);
}

// ContentInstructionCollection

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_GetSize(ContentInstructionCollectionHandle* handle, size_type* result) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->size();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_At(ContentInstructionCollectionHandle* handle, size_type at, ContentInstructionHandle** result) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto instruction = obj->at(at);
        auto base = instruction.AddRefGet();
        *result = reinterpret_cast<ContentInstructionHandle*>(base);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Append(ContentInstructionCollectionHandle* handle, ContentInstructionHandle* value) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    InstructionBase* instruction = reinterpret_cast<InstructionBase*>(value);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(instruction);

    try
    {
        obj->push_back(instruction);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Insert(ContentInstructionCollectionHandle* handle, size_type at, ContentInstructionHandle* value) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    InstructionBase* instruction = reinterpret_cast<InstructionBase*>(value);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(instruction);

    try
    {
        InstructionBasePtr instruction_ptr(instruction);
        obj->insert(obj->begin() + at, instruction_ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Remove(ContentInstructionCollectionHandle* handle, size_type at) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->erase(obj->begin() + at);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Clear(ContentInstructionCollectionHandle* handle) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->clear();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_GetIterator(ContentInstructionCollectionHandle* handle, ContentInstructionCollectionIteratorHandle** result) {
    BaseInstructionCollection* obj = reinterpret_cast<BaseInstructionCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto begin = make_deferred<BaseInstructionCollection::Iterator>(obj->begin(), obj->end());
        auto ptr = begin.AddRefGet();
        *result = reinterpret_cast<ContentInstructionCollectionIteratorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_ToUnknown(ContentInstructionCollectionHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<BaseInstructionCollection, IUnknown, ContentInstructionCollectionHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_FromUnknown(IUnknownHandle* handle, ContentInstructionCollectionHandle** result) {
    return SafeObjectConvert<IUnknown, BaseInstructionCollection, IUnknownHandle, ContentInstructionCollectionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollection_Release(ContentInstructionCollectionHandle* handle) {
    return ObjectRelease<BaseInstructionCollection, ContentInstructionCollectionHandle>(handle);
}

// ContentInstructionCollectionIterator

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_GetValue(ContentInstructionCollectionIteratorHandle* handle, ContentInstructionHandle** result) {
    BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto instruction = iterator->Value();
        auto ptr = instruction.AddRefGet();
        *result = reinterpret_cast<ContentInstructionHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_IsValid(ContentInstructionCollectionIteratorHandle* handle, boolean_type* result) {
    BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        if (iterator->IsValid()) {
            *result = VANILLAPDF_RV_TRUE;
        }
        else {
            *result = VANILLAPDF_RV_FALSE;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_Next(ContentInstructionCollectionIteratorHandle* handle) {
    BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

    try
    {
        ++(*iterator);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_ToUnknown(ContentInstructionCollectionIteratorHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<BaseInstructionCollection::Iterator, IUnknown, ContentInstructionCollectionIteratorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_FromUnknown(IUnknownHandle* handle, ContentInstructionCollectionIteratorHandle** result) {
    return SafeObjectConvert<IUnknown, BaseInstructionCollection::Iterator, IUnknownHandle, ContentInstructionCollectionIteratorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstructionCollectionIterator_Release(ContentInstructionCollectionIteratorHandle* handle) {
    return ObjectRelease<BaseInstructionCollection::Iterator, ContentInstructionCollectionIteratorHandle>(handle);
}
