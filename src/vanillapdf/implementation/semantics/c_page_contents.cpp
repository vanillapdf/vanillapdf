#include "precompiled.h"
#include "semantics/objects/page_contents.h"

#include "contents/content_stream_instruction_base.h"

#include "vanillapdf/semantics/c_page_contents.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_GetInstructionCollection(PageContentsHandle* handle, ContentInstructionCollectionHandle** result)
{
	PageContents* obj = reinterpret_cast<PageContents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto instructions = obj->Instructions();
		auto ptr = instructions.AddRefGet();
		*result = reinterpret_cast<ContentInstructionCollectionHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_RecalculateStreamData(PageContentsHandle* handle, boolean_type* result) {

	PageContents* obj = reinterpret_cast<PageContents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try {
		if (obj->RecalculateStreamData()) {
			*result = VANILLAPDF_RV_TRUE;
		} else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageContents_GetBaseObject(PageContentsHandle* handle, ObjectHandle** result) {
	PageContents* obj = reinterpret_cast<PageContents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto base_object = obj->GetObject();
		auto ptr = base_object.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
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
