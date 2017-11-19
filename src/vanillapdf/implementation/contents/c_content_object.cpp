#include "precompiled.h"

#include "contents/content_stream_objects.h"

#include "vanillapdf/contents/c_content_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle* handle, ContentObjectType* result) {
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
		case ContentObjectBase::Type::TextObject:
			*result = ContentObjectType_Text; break;
		case ContentObjectBase::Type::InlineImageObject:
			*result = ContentObjectType_InlineImage; break;
		default:
			assert(!"Unknown enum value");
			return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle* handle, ContentObjectTextHandle** result) {
	return SafeObjectConvert<ContentObjectBase, TextObject, ContentObjectHandle*, ContentObjectTextHandle*>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle* handle, ContentObjectInlineImageHandle** result) {
	return SafeObjectConvert<ContentObjectBase, InlineImageObject, ContentObjectHandle*, ContentObjectInlineImageHandle*>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle* handle) {
	return ObjectRelease<ContentObjectBase, ContentObjectHandle*>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle* handle, DictionaryObjectHandle** result) {
	InlineImageObject* obj = reinterpret_cast<InlineImageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto item = obj->GetDictionary();
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle* handle, BufferHandle** result) {
	InlineImageObject* obj = reinterpret_cast<InlineImageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto item = obj->GetData();
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_Release(ContentObjectInlineImageHandle* handle) {
	return ObjectRelease<InlineImageObject, ContentObjectInlineImageHandle*>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle* handle, size_type* result) {
	TextObject* obj = reinterpret_cast<TextObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->GetOperationsSize();
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle* handle, size_type at, ContentOperationHandle** result) {
	TextObject* obj = reinterpret_cast<TextObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto item = obj->GetOperationAt(at);
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<ContentOperationHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_Release(ContentObjectTextHandle* handle) {
	return ObjectRelease<TextObject, ContentObjectTextHandle*>(handle);
}