#include "precompiled.h"

#include "syntax/filters/filter.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/filters/lzw_decode_filter.h"
#include "syntax/filters/jpx_decode_filter.h"
#include "syntax/filters/flate_decode_filter.h"
#include "syntax/filters/ascii_85_decode_filter.h"
#include "syntax/filters/ascii_hex_decode_filter.h"

#include "vanillapdf/syntax/c_filter.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// FilterBase

VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Encode(FilterBaseHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    FilterBase* filter = reinterpret_cast<FilterBase*>(handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto encoded = filter->Encode(data);
        auto ptr = encoded.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_EncodeParams(FilterBaseHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    FilterBase* filter = reinterpret_cast<FilterBase*>(handle);
    DictionaryObject* params = reinterpret_cast<DictionaryObject*>(parameters_handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(params);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto encoded = filter->Encode(data, params);
        auto ptr = encoded.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Decode(FilterBaseHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto decoded = filter->Decode(data);
        auto ptr = decoded.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Release(FilterBaseHandle* handle) {
    return ObjectRelease<FilterBase, FilterBaseHandle>(handle);
}

// FlateDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Create(FlateDecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<FlateDecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<FlateDecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Encode(FlateDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_EncodeParams(FlateDecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Decode(FlateDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Release(FlateDecodeFilterHandle* handle) {
    return ObjectRelease<FlateDecodeFilter, FlateDecodeFilterHandle>(handle);
}

// DCTDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Create(DCTDecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<DCTDecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<DCTDecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Encode(DCTDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    DCTDecodeFilter* filter = reinterpret_cast<DCTDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_EncodeParams(DCTDecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    DCTDecodeFilter* filter = reinterpret_cast<DCTDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Decode(DCTDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    DCTDecodeFilter* filter = reinterpret_cast<DCTDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Release(DCTDecodeFilterHandle* handle) {
    return ObjectRelease<DCTDecodeFilter, DCTDecodeFilterHandle>(handle);
}

// ASCII85DecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Create(ASCII85DecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<ASCII85DecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<ASCII85DecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Encode(ASCII85DecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    ASCII85DecodeFilter* filter = reinterpret_cast<ASCII85DecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_EncodeParams(ASCII85DecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    ASCII85DecodeFilter* filter = reinterpret_cast<ASCII85DecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Decode(ASCII85DecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    ASCII85DecodeFilter* filter = reinterpret_cast<ASCII85DecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Release(ASCII85DecodeFilterHandle* handle) {
    return ObjectRelease<ASCII85DecodeFilter, ASCII85DecodeFilterHandle>(handle);
}

// ASCIIHexDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Create(ASCIIHexDecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<ASCIIHexDecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<ASCIIHexDecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Encode(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    ASCIIHexDecodeFilter* filter = reinterpret_cast<ASCIIHexDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_EncodeParams(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    ASCIIHexDecodeFilter* filter = reinterpret_cast<ASCIIHexDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Decode(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    ASCIIHexDecodeFilter* filter = reinterpret_cast<ASCIIHexDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Release(ASCIIHexDecodeFilterHandle* handle) {
    return ObjectRelease<ASCIIHexDecodeFilter, ASCIIHexDecodeFilterHandle>(handle);
}

// LZWDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION LZWDecodeFilter_Create(LZWDecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<LZWDecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<LZWDecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LZWDecodeFilter_Encode(LZWDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    LZWDecodeFilter* filter = reinterpret_cast<LZWDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LZWDecodeFilter_EncodeParams(LZWDecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    LZWDecodeFilter* filter = reinterpret_cast<LZWDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LZWDecodeFilter_Decode(LZWDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    LZWDecodeFilter* filter = reinterpret_cast<LZWDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LZWDecodeFilter_Release(LZWDecodeFilterHandle* handle) {
    return ObjectRelease<LZWDecodeFilter, LZWDecodeFilterHandle>(handle);
}

// JPXDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION JPXDecodeFilter_Create(JPXDecodeFilterHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto filter = make_deferred<JPXDecodeFilter>();
        auto ptr = filter.AddRefGet();
        *result = reinterpret_cast<JPXDecodeFilterHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION JPXDecodeFilter_Encode(JPXDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    JPXDecodeFilter* filter = reinterpret_cast<JPXDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Encode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION JPXDecodeFilter_EncodeParams(JPXDecodeFilterHandle* handle, BufferHandle* data_handle, DictionaryObjectHandle* parameters_handle, BufferHandle** result) {
    JPXDecodeFilter* filter = reinterpret_cast<JPXDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_EncodeParams(base_filter_handle, data_handle, parameters_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION JPXDecodeFilter_Decode(JPXDecodeFilterHandle* handle, BufferHandle* data_handle, BufferHandle** result) {
    JPXDecodeFilter* filter = reinterpret_cast<JPXDecodeFilter*>(handle);
    FilterBase* base_filter = static_cast<FilterBase*>(filter);
    FilterBaseHandle* base_filter_handle = reinterpret_cast<FilterBaseHandle*>(base_filter);

    return FilterBase_Decode(base_filter_handle, data_handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION JPXDecodeFilter_Release(JPXDecodeFilterHandle* handle) {
    return ObjectRelease<JPXDecodeFilter, JPXDecodeFilterHandle>(handle);
}
