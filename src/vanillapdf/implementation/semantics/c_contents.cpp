#include "precompiled.h"
#include "semantics/objects/contents.h"

#include "contents/content_stream_instruction_base.h"

#include "vanillapdf/semantics/c_contents.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle* handle, size_type* result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetInstructionsSize();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle* handle, size_type at, ContentInstructionHandle** result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto instruction = obj->GetInstructionAt(at);
		auto base = instruction.AddRefGet();
		*result = reinterpret_cast<ContentInstructionHandle*>(base);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Contents_ToUnknown(ContentsHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<Contents, IUnknown, ContentsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Contents_FromUnknown(IUnknownHandle* handle, ContentsHandle** result) {
	return SafeObjectConvert<IUnknown, Contents, IUnknownHandle, ContentsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle* handle) {
	return ObjectRelease<Contents, ContentsHandle>(handle);
}
