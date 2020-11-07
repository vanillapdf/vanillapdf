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
