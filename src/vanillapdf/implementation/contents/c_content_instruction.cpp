#include "precompiled.h"

#include "contents/content_stream_instruction_base.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_objects.h"

#include "vanillapdf/contents/c_content_instruction.h"
#include "implementation/c_helper.h"

using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle* handle, ContentInstructionType* result) {
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
	return SafeObjectConvert<InstructionBase, OperationBase, ContentInstructionHandle*, ContentOperationHandle*>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle* handle, ContentObjectHandle** result) {
	return SafeObjectConvert<InstructionBase, ContentObjectBase, ContentInstructionHandle*, ContentObjectHandle*>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle* handle) {
	return ObjectRelease<InstructionBase, ContentInstructionHandle*>(handle);
}