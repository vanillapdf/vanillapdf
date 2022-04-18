#include "precompiled.h"

#include "contents/content_stream_operations.h"
#include "contents/content_stream_operation_generic.h"

#include "vanillapdf/contents/c_content_operation.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_GetOperationType(ContentOperationHandle* handle, ContentOperationType* result) {
	OperationBase* obj = reinterpret_cast<OperationBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetOperationType()) {
		case OperationBase::Type::Generic:
			*result = ContentOperationType_Generic; break;
		case OperationBase::Type::LineWidth:
			*result = ContentOperationType_LineWidth; break;
		case OperationBase::Type::LineCap:
			*result = ContentOperationType_LineCap; break;
		case OperationBase::Type::LineJoin:
			*result = ContentOperationType_LineJoin; break;
		case OperationBase::Type::MiterLimit:
			*result = ContentOperationType_MiterLimit; break;
		case OperationBase::Type::DashPattern:
			*result = ContentOperationType_DashPattern; break;
		case OperationBase::Type::ColorRenderingIntent:
			*result = ContentOperationType_ColorRenderingIntent; break;
		case OperationBase::Type::Flatness:
			*result = ContentOperationType_Flatness; break;
		case OperationBase::Type::GraphicsState:
			*result = ContentOperationType_GraphicsState; break;
		case OperationBase::Type::SaveGraphicsState:
			*result = ContentOperationType_SaveGraphicsState; break;
		case OperationBase::Type::RestoreGraphicsState:
			*result = ContentOperationType_RestoreGraphicsState; break;
		case OperationBase::Type::TransformationMatrix:
			*result = ContentOperationType_TransformationMatrix; break;
		case OperationBase::Type::BeginSubpath:
			*result = ContentOperationType_BeginSubpath; break;
		case OperationBase::Type::Line:
			*result = ContentOperationType_Line; break;
		case OperationBase::Type::FullCurve:
			*result = ContentOperationType_FullCurve; break;
		case OperationBase::Type::FinalCurve:
			*result = ContentOperationType_FinalCurve; break;
		case OperationBase::Type::InitialCurve:
			*result = ContentOperationType_InitialCurve; break;
		case OperationBase::Type::CloseSubpath:
			*result = ContentOperationType_CloseSubpath; break;
		case OperationBase::Type::Rectangle:
			*result = ContentOperationType_Rectangle; break;
		case OperationBase::Type::Stroke:
			*result = ContentOperationType_Stroke; break;
		case OperationBase::Type::CloseAndStroke:
			*result = ContentOperationType_CloseAndStroke; break;
		case OperationBase::Type::FillPathNonzero:
			*result = ContentOperationType_FillPathNonzero; break;
		case OperationBase::Type::FillPathCompatibility:
			*result = ContentOperationType_FillPathCompatibility; break;
		case OperationBase::Type::FillPathEvenOdd:
			*result = ContentOperationType_FillPathEvenOdd; break;
		case OperationBase::Type::FillStrokeNonzero:
			*result = ContentOperationType_FillStrokeNonzero; break;
		case OperationBase::Type::FillStrokeEvenOdd:
			*result = ContentOperationType_FillStrokeEvenOdd; break;
		case OperationBase::Type::CloseFillStrokeNonzero:
			*result = ContentOperationType_CloseFillStrokeNonzero; break;
		case OperationBase::Type::CloseFillStrokeEvenOdd:
			*result = ContentOperationType_CloseFillStrokeEvenOdd; break;
		case OperationBase::Type::EndPath:
			*result = ContentOperationType_EndPath; break;
		case OperationBase::Type::ClipPathNonzero:
			*result = ContentOperationType_ClipPathNonzero; break;
		case OperationBase::Type::ClipPathEvenOdd:
			*result = ContentOperationType_ClipPathEvenOdd; break;
		case OperationBase::Type::BeginText:
			*result = ContentOperationType_BeginText; break;
		case OperationBase::Type::EndText:
			*result = ContentOperationType_EndText; break;
		case OperationBase::Type::CharacterSpacing:
			*result = ContentOperationType_CharacterSpacing; break;
		case OperationBase::Type::WordSpacing:
			*result = ContentOperationType_WordSpacing; break;
		case OperationBase::Type::HorizontalScaling:
			*result = ContentOperationType_HorizontalScaling; break;
		case OperationBase::Type::Leading:
			*result = ContentOperationType_Leading; break;
		case OperationBase::Type::TextFont:
			*result = ContentOperationType_TextFont; break;
		case OperationBase::Type::TextRenderingMode:
			*result = ContentOperationType_TextRenderingMode; break;
		case OperationBase::Type::TextRise:
			*result = ContentOperationType_TextRise; break;
		case OperationBase::Type::TextTranslate:
			*result = ContentOperationType_TextTranslate; break;
		case OperationBase::Type::TextTranslateLeading:
			*result = ContentOperationType_TextTranslateLeading; break;
		case OperationBase::Type::TextMatrix:
			*result = ContentOperationType_TextMatrix; break;
		case OperationBase::Type::TextNextLine:
			*result = ContentOperationType_TextNextLine; break;
		case OperationBase::Type::TextShow:
			*result = ContentOperationType_TextShow; break;
		case OperationBase::Type::TextShowArray:
			*result = ContentOperationType_TextShowArray; break;
		case OperationBase::Type::TextNextLineShow:
			*result = ContentOperationType_TextNextLineShow; break;
		case OperationBase::Type::TextNextLineShowSpacing:
			*result = ContentOperationType_TextNextLineShowSpacing; break;
		case OperationBase::Type::SetCharWidth:
			*result = ContentOperationType_SetCharWidth; break;
		case OperationBase::Type::SetCacheDevice:
			*result = ContentOperationType_SetCacheDevice; break;
		case OperationBase::Type::ColorSpaceStroke:
			*result = ContentOperationType_ColorSpaceStroke; break;
		case OperationBase::Type::ColorSpaceNonstroke:
			*result = ContentOperationType_ColorSpaceNonstroke; break;
		case OperationBase::Type::SetColorStroke:
			*result = ContentOperationType_SetColorStroke; break;
		case OperationBase::Type::SetColorStrokeExtended:
			*result = ContentOperationType_SetColorStrokeExtended; break;
		case OperationBase::Type::SetColorNonstroke:
			*result = ContentOperationType_SetColorNonstroke; break;
		case OperationBase::Type::SetColorNonstrokeExtended:
			*result = ContentOperationType_SetColorNonstrokeExtended; break;
		case OperationBase::Type::SetStrokingColorSpaceGray:
			*result = ContentOperationType_SetStrokingColorSpaceGray; break;
		case OperationBase::Type::SetNonstrokingColorSpaceGray:
			*result = ContentOperationType_SetNonstrokingColorSpaceGray; break;
		case OperationBase::Type::SetStrokingColorSpaceRGB:
			*result = ContentOperationType_SetStrokingColorSpaceRGB; break;
		case OperationBase::Type::SetNonstrokingColorSpaceRGB:
			*result = ContentOperationType_SetNonstrokingColorSpaceRGB; break;
		case OperationBase::Type::SetStrokingColorSpaceCMYK:
			*result = ContentOperationType_SetStrokingColorSpaceCMYK; break;
		case OperationBase::Type::SetNonstrokingColorSpaceCMYK:
			*result = ContentOperationType_SetNonstrokingColorSpaceCMYK; break;
		case OperationBase::Type::ShadingPaint:
			*result = ContentOperationType_ShadingPaint; break;
		case OperationBase::Type::BeginInlineImageObject:
			*result = ContentOperationType_BeginInlineImageObject; break;
		case OperationBase::Type::BeginInlineImageData:
			*result = ContentOperationType_BeginInlineImageData; break;
		case OperationBase::Type::EndInlineImageObject:
			*result = ContentOperationType_EndInlineImageObject; break;
		case OperationBase::Type::InvokeXObject:
			*result = ContentOperationType_InvokeXObject; break;
		case OperationBase::Type::DefineMarkedContentPoint:
			*result = ContentOperationType_DefineMarkedContentPoint; break;
		case OperationBase::Type::DefineMarkedContentPointWithPropertyList:
			*result = ContentOperationType_DefineMarkedContentPointWithPropertyList; break;
		case OperationBase::Type::BeginMarkedContentSequence:
			*result = ContentOperationType_BeginMarkedContentSequence; break;
		case OperationBase::Type::BeginMarkedContentSequenceWithPropertyList:
			*result = ContentOperationType_BeginMarkedContentSequenceWithPropertyList; break;
		case OperationBase::Type::EndMarkedContentSequence:
			*result = ContentOperationType_EndMarkedContentSequence; break;
		case OperationBase::Type::BeginCompatibilitySection:
			*result = ContentOperationType_BeginCompatibilitySection; break;
		case OperationBase::Type::EndCompatibilitySection:
			*result = ContentOperationType_EndCompatibilitySection; break;
		default:
			assert(!"Unknown enum value");
			return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToBeginText(ContentOperationHandle* handle, ContentOperationBeginTextHandle** result) {
	return SafeObjectConvert<OperationBase, OperationBeginText, ContentOperationHandle, ContentOperationBeginTextHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToEndText(ContentOperationHandle* handle, ContentOperationEndTextHandle** result) {
	return SafeObjectConvert<OperationBase, OperationEndText, ContentOperationHandle, ContentOperationEndTextHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToInstruction(ContentOperationHandle* handle, ContentInstructionHandle** result) {
	return SafeObjectConvert<OperationBase, InstructionBase, ContentOperationHandle, ContentInstructionHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_FromInstruction(ContentInstructionHandle* handle, ContentOperationHandle** result) {
	return SafeObjectConvert<InstructionBase, OperationBase, ContentInstructionHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle* handle) {
	return ObjectRelease<OperationBase, ContentOperationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperator(ContentOperationGenericHandle* handle, ContentOperatorHandle** result) {
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto oper = obj->GetOperator();
		auto base = oper.AddRefGet();
		*result = reinterpret_cast<ContentOperatorHandle*>(base);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandsSize(ContentOperationGenericHandle* handle, size_type* result) {
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->GetOperandsSize();
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandAt(ContentOperationGenericHandle* handle, size_type at, ObjectHandle** result) {
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto item = obj->GetOperandAt(at);
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_ToContentOperation(ContentOperationGenericHandle* handle, ContentOperationHandle** result) {
	return SafeObjectConvert<OperationGeneric, OperationBase, ContentOperationGenericHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_FromContentOperation(ContentOperationHandle* handle, ContentOperationGenericHandle** result) {
	return SafeObjectConvert<OperationBase, OperationGeneric, ContentOperationHandle, ContentOperationGenericHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_Release(ContentOperationGenericHandle* handle) {
	return ObjectRelease<OperationGeneric, ContentOperationGenericHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetScale(ContentOperationTextFontHandle* handle, IntegerObjectHandle** result) {
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto value = obj->GetScale();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetScale(ContentOperationTextFontHandle* handle, IntegerObjectHandle* data) {
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	IntegerObject* name = reinterpret_cast<IntegerObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try {
		obj->SetScale(name);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetName(ContentOperationTextFontHandle* handle, NameObjectHandle** result) {
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto value = obj->GetName();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetName(ContentOperationTextFontHandle* handle, NameObjectHandle* data) {
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	NameObject* name = reinterpret_cast<NameObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try {
		obj->SetName(name);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_ToContentOperation(ContentOperationTextFontHandle* handle, ContentOperationHandle** result) {
	return SafeObjectConvert<OperationTextFont, OperationBase, ContentOperationTextFontHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextFontHandle** result) {
	return SafeObjectConvert<OperationBase, OperationTextFont, ContentOperationHandle, ContentOperationTextFontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_Release(ContentOperationTextFontHandle* handle) {
	return ObjectRelease<OperationTextFont, ContentOperationTextFontHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_SetValue(ContentOperationTextShowHandle* handle, StringObjectHandle* data) {
	OperationTextShow* obj = reinterpret_cast<OperationTextShow*>(handle);
	StringObjectBase* str = reinterpret_cast<StringObjectBase*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

	try {
		obj->SetValue(str);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_GetValue(ContentOperationTextShowHandle* handle, StringObjectHandle** result) {
	OperationTextShow* obj = reinterpret_cast<OperationTextShow*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto value = obj->GetValue();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_ToContentOperation(ContentOperationTextShowHandle* handle, ContentOperationHandle** result) {
	return SafeObjectConvert<OperationTextShow, OperationBase, ContentOperationTextShowHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextShowHandle** result) {
	return SafeObjectConvert<OperationBase, OperationTextShow, ContentOperationHandle, ContentOperationTextShowHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_Release(ContentOperationTextShowHandle* handle) {
	return ObjectRelease<OperationTextShow, ContentOperationTextShowHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_GetValue(ContentOperationTextShowArrayHandle* handle, ArrayObjectHandle** result) {
	OperationTextShowArray* obj = reinterpret_cast<OperationTextShowArray*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto value = obj->GetValue();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<ArrayObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_SetValue(ContentOperationTextShowArrayHandle* handle, ArrayObjectHandle* data) {
	OperationTextShowArray* obj = reinterpret_cast<OperationTextShowArray*>(handle);
	MixedArrayObject* str = reinterpret_cast<MixedArrayObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

	try {
		obj->SetValue(str);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_ToContentOperation(ContentOperationTextShowArrayHandle* handle, ContentOperationHandle** result) {
	return SafeObjectConvert<OperationTextShowArray, OperationBase, ContentOperationTextShowArrayHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextShowArrayHandle** result) {
	return SafeObjectConvert<OperationBase, OperationTextShowArray, ContentOperationHandle, ContentOperationTextShowArrayHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_Release(ContentOperationTextShowArrayHandle* handle) {
	return ObjectRelease<OperationTextShowArray, ContentOperationTextShowArrayHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationEndText_Release(ContentOperationEndTextHandle* handle) {
	return ObjectRelease<OperationEndText, ContentOperationEndTextHandle>(handle);
}
