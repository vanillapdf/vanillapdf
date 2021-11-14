#include "precompiled.h"
#include "contents/character_map_data.h"

#include "vanillapdf/contents/c_base_font_range.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Create(BaseFontRangeHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		BaseFontRangePtr font_range;
		auto ptr = font_range.AddRefGet();
		*result = reinterpret_cast<BaseFontRangeHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto range_low = obj->GetRangeLow();
		auto ptr = range_low.AddRefGet();
		*result = reinterpret_cast<HexadecimalStringObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	HexadecimalStringObject* value = reinterpret_cast<HexadecimalStringObject*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value);

	try {
		obj->SetRangeLow(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto range_low = obj->GetRangeHigh();
		auto ptr = range_low.AddRefGet();
		*result = reinterpret_cast<HexadecimalStringObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	HexadecimalStringObject* value = reinterpret_cast<HexadecimalStringObject*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value);

	try {
		obj->SetRangeHigh(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetDestination(BaseFontRangeHandle* handle, ObjectHandle** result) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto destination = obj->GetDestination();
		auto ptr = destination.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetDestination(BaseFontRangeHandle* handle, ObjectHandle* data) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	Object* value = reinterpret_cast<Object*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value);

	try {
		obj->SetDestination(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Contains(BaseFontRangeHandle* handle, BufferHandle* data, boolean_type* result) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	Buffer* value = reinterpret_cast<Buffer*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->Contains(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetMappedValue(BaseFontRangeHandle* handle, BufferHandle* data, BufferHandle** result) {
	BaseFontRange* obj = reinterpret_cast<BaseFontRange*>(handle);
	Buffer* value = reinterpret_cast<Buffer*>(data);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto mapped_value = obj->GetMappedValue(value);
		auto ptr = mapped_value.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_ToUnknown(BaseFontRangeHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<BaseFontRange, IUnknown, BaseFontRangeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_FromUnknown(IUnknownHandle* handle, BaseFontRangeHandle** result) {
	return SafeObjectConvert<IUnknown, BaseFontRange, IUnknownHandle, BaseFontRangeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Release(BaseFontRangeHandle* handle) {
	return ObjectRelease<BaseFontRange, BaseFontRangeHandle>(handle);
}
