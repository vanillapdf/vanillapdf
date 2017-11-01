#include "precompiled.h"

#include "syntax/filters/filter.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/filters/flate_decode_filter.h"
#include "syntax/filters/ascii_85_decode_filter.h"
#include "syntax/filters/ascii_hex_decode_filter.h"

#include "vanillapdf/c_filter.h"
#include "c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// FlateDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Create(FlateDecodeFilterHandle* result) {

	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto filter = make_deferred<FlateDecodeFilter>();
		auto ptr = filter.AddRefGet();
		*result = reinterpret_cast<FlateDecodeFilterHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Encode(FlateDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto encoded = filter->Encode(data);
		auto ptr = encoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Decode(FlateDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	FlateDecodeFilter* filter = reinterpret_cast<FlateDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto decoded = filter->Decode(data);
		auto ptr = decoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Release(FlateDecodeFilterHandle handle) {
	return ObjectRelease<FlateDecodeFilter, FlateDecodeFilterHandle>(handle);
}

// DCTDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Create(DCTDecodeFilterHandle* result) {

	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto filter = make_deferred<DCTDecodeFilter>();
		auto ptr = filter.AddRefGet();
		*result = reinterpret_cast<DCTDecodeFilterHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Encode(DCTDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	DCTDecodeFilter* filter = reinterpret_cast<DCTDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto encoded = filter->Encode(data);
		auto ptr = encoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Decode(DCTDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	DCTDecodeFilter* filter = reinterpret_cast<DCTDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto decoded = filter->Decode(data);
		auto ptr = decoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Release(DCTDecodeFilterHandle handle) {
	return ObjectRelease<DCTDecodeFilter, DCTDecodeFilterHandle>(handle);
}

// ASCII85DecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Create(ASCII85DecodeFilterHandle* result) {

	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto filter = make_deferred<ASCII85DecodeFilter>();
		auto ptr = filter.AddRefGet();
		*result = reinterpret_cast<ASCII85DecodeFilterHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Encode(ASCII85DecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	ASCII85DecodeFilter* filter = reinterpret_cast<ASCII85DecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto encoded = filter->Encode(data);
		auto ptr = encoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Decode(ASCII85DecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	ASCII85DecodeFilter* filter = reinterpret_cast<ASCII85DecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto decoded = filter->Decode(data);
		auto ptr = decoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Release(ASCII85DecodeFilterHandle handle) {
	return ObjectRelease<ASCII85DecodeFilter, ASCII85DecodeFilterHandle>(handle);
}

// ASCIIHexDecodeFilter

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Create(ASCIIHexDecodeFilterHandle* result) {

	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto filter = make_deferred<ASCIIHexDecodeFilter>();
		auto ptr = filter.AddRefGet();
		*result = reinterpret_cast<ASCIIHexDecodeFilterHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Encode(ASCIIHexDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	ASCIIHexDecodeFilter* filter = reinterpret_cast<ASCIIHexDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto encoded = filter->Encode(data);
		auto ptr = encoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Decode(ASCIIHexDecodeFilterHandle handle, BufferHandle data_handle, BufferHandle* result) {
	ASCIIHexDecodeFilter* filter = reinterpret_cast<ASCIIHexDecodeFilter*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(filter);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto decoded = filter->Decode(data);
		auto ptr = decoded.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Release(ASCIIHexDecodeFilterHandle handle) {
	return ObjectRelease<ASCIIHexDecodeFilter, ASCIIHexDecodeFilterHandle>(handle);
}
