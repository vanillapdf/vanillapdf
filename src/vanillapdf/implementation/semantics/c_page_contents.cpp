#include "precompiled.h"
#include "semantics/objects/page_contents.h"

#include "contents/content_stream_instruction_base.h"

#include "vanillapdf/semantics/c_page_contents.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_GetIterator(PageContentsHandle* handle, PageContentsIteratorHandle** result) {

	PageContents* obj = reinterpret_cast<PageContents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto instruction_iterator = obj->GetInstructionIterator();
		auto base = instruction_iterator.AddRefGet();
		*result = reinterpret_cast<PageContentsIteratorHandle*>(base);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_ToUnknown(PageContentsHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<PageContents, IUnknown, PageContentsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_FromUnknown(IUnknownHandle* handle, PageContentsHandle** result) {
	return SafeObjectConvert<IUnknown, PageContents, IUnknownHandle, PageContentsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_Release(PageContentsHandle* handle) {
	return ObjectRelease<PageContents, PageContentsHandle>(handle);
}

// Iterator

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_GetValue(PageContentsIteratorHandle* handle, ContentInstructionHandle** result) {

	BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto entry = iterator->Value();
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<ContentInstructionHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_Next(PageContentsIteratorHandle* handle) {

	BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_IsValid(PageContentsIteratorHandle* handle, boolean_type* result) {

	BaseInstructionCollection::Iterator* iterator = reinterpret_cast<BaseInstructionCollection::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		if (iterator->IsValid()) {
			*result = VANILLAPDF_RV_TRUE;
		} else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_ToUnknown(PageContentsIteratorHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<BaseInstructionCollection::Iterator, IUnknown, PageContentsIteratorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_FromUnknown(IUnknownHandle* handle, PageContentsIteratorHandle** result) {
	return SafeObjectConvert<IUnknown, BaseInstructionCollection::Iterator, IUnknownHandle, PageContentsIteratorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContentsIterator_Release(PageContentsIteratorHandle* handle) {
	return ObjectRelease<BaseInstructionCollection::Iterator, PageContentsIteratorHandle>(handle);
}
