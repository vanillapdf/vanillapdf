#include "precompiled.h"

#include "semantics/utils/byte_range.h"

#include "vanillapdf/semantics/c_byte_range.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Create(ByteRangeHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto range = make_deferred<ByteRange>();
        auto ptr = range.AddRefGet();
        *result = reinterpret_cast<ByteRangeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_CreateFromData(IntegerObjectHandle* offset, IntegerObjectHandle* length, ByteRangeHandle** result) {
    IntegerObject* offset_obj = reinterpret_cast<IntegerObject*>(offset);
    IntegerObject* length_obj = reinterpret_cast<IntegerObject*>(length);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(offset_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(length_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto range = make_deferred<ByteRange>(offset_obj, length_obj);
        auto ptr = range.AddRefGet();
        *result = reinterpret_cast<ByteRangeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetOffset(ByteRangeHandle* handle, IntegerObjectHandle** result) {
    ByteRange* range = reinterpret_cast<ByteRange*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto direct = range->ByteOffset();
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<IntegerObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetLength(ByteRangeHandle* handle, IntegerObjectHandle** result) {
    ByteRange* range = reinterpret_cast<ByteRange*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto direct = range->Length();
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<IntegerObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_SetOffset(ByteRangeHandle* handle, IntegerObjectHandle* offset) {
    ByteRange* range = reinterpret_cast<ByteRange*>(handle);
    IntegerObject* offset_obj = reinterpret_cast<IntegerObject*>(offset);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(offset_obj);

    try {
        range->SetByteOffset(offset_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_SetLength(ByteRangeHandle* handle, IntegerObjectHandle* length) {
    ByteRange* range = reinterpret_cast<ByteRange*>(handle);
    IntegerObject* length_obj = reinterpret_cast<IntegerObject*>(length);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(length_obj);

    try {
        range->SetLength(length_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Create(ByteRangeCollectionHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto collection = make_deferred<ByteRangeCollection>();
        auto ptr = collection.AddRefGet();
        *result = reinterpret_cast<ByteRangeCollectionHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Append(ByteRangeCollectionHandle* handle, ByteRangeHandle* byte_range) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
    ByteRange* range = reinterpret_cast<ByteRange*>(byte_range);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);

    try {
        collection->Append(range);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Insert(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle* byte_range) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
    ByteRange* range = reinterpret_cast<ByteRange*>(byte_range);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(range);

    try {
        collection->Insert(at, range);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Remove(ByteRangeCollectionHandle* handle, size_type at) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);

    try {
        collection->Remove(at);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Clear(ByteRangeCollectionHandle* handle) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);

    try {
        collection->Clear();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_GetSize(ByteRangeCollectionHandle* handle, size_type* result) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = collection->GetSize();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_GetValue(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle** result) {
    ByteRangeCollection* collection = reinterpret_cast<ByteRangeCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto direct = collection->GetValue(at);
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ByteRangeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Release(ByteRangeHandle* handle) {
    return ObjectRelease<ByteRange, ByteRangeHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Release(ByteRangeCollectionHandle* handle) {
    return ObjectRelease<ByteRangeCollection, ByteRangeCollectionHandle>(handle);
}
