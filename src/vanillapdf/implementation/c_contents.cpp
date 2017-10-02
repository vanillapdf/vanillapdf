#include "precompiled.h"
#include "semantics/objects/contents.h"

#include "contents/content_stream_operator_base.h"
#include "contents/content_stream_operation_generic.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_objects.h"

#include "vanillapdf/c_contents.h"
#include "c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;
using namespace vanillapdf::syntax;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle handle, size_type* result)
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

VANILLAPDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle handle, size_type at, ContentInstructionHandle* result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto instruction = obj->GetInstructionAt(at);
		auto base = instruction.AddRefGet();
		*result = reinterpret_cast<ContentInstructionHandle>(base);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle)
{
	return ObjectRelease<Contents, ContentsHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle handle, ContentInstructionType* result)
{
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

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle handle, ContentOperationHandle* result)
{
	return SafeObjectConvert<InstructionBase, OperationBase, ContentInstructionHandle, ContentOperationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle handle, ContentObjectHandle* result)
{
	return SafeObjectConvert<InstructionBase, ContentObjectBase, ContentInstructionHandle, ContentObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle handle)
{
	return ObjectRelease<InstructionBase, ContentInstructionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle handle, PContentObjectType result)
{
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

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle handle, ContentObjectTextHandle* result)
{
	return SafeObjectConvert<ContentObjectBase, TextObject, ContentObjectHandle, ContentObjectTextHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle handle, ContentObjectInlineImageHandle* result)
{
	return SafeObjectConvert<ContentObjectBase, InlineImageObject, ContentObjectHandle, ContentObjectInlineImageHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle handle)
{
	return ObjectRelease<ContentObjectBase, ContentObjectHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle handle, DictionaryObjectHandle* result)
{
	InlineImageObject* obj = reinterpret_cast<InlineImageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto item = obj->GetDictionary();
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle handle, BufferHandle* result)
{
	InlineImageObject* obj = reinterpret_cast<InlineImageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto item = obj->GetData();
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_Release(ContentObjectInlineImageHandle handle)
{
	return ObjectRelease<InlineImageObject, ContentObjectInlineImageHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle handle, size_type* result)
{
	TextObject* obj = reinterpret_cast<TextObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetOperationsSize();
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle handle, size_type at, ContentOperationHandle* result)
{
	TextObject* obj = reinterpret_cast<TextObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto item = obj->GetOperationAt(at);
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<ContentOperationHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentObjectText_Release(ContentObjectTextHandle handle)
{
	return ObjectRelease<TextObject, ContentObjectTextHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_GetType(ContentOperationHandle handle, PContentOperationType result)
{
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

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToGeneric(ContentOperationHandle handle, ContentOperationGenericHandle* result)
{
	return SafeObjectConvert<OperationBase, OperationGeneric, ContentOperationHandle, ContentOperationGenericHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToBeginText(ContentOperationHandle handle, ContentOperationBeginTextHandle* result) {
	return SafeObjectConvert<OperationBase, OperationBeginText, ContentOperationHandle, ContentOperationBeginTextHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToEndText(ContentOperationHandle handle, ContentOperationEndTextHandle* result)
{
	return SafeObjectConvert<OperationBase, OperationEndText, ContentOperationHandle, ContentOperationEndTextHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToTextFont(ContentOperationHandle handle, ContentOperationTextFontHandle* result)
{
	return SafeObjectConvert<OperationBase, OperationTextFont, ContentOperationHandle, ContentOperationTextFontHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShow(ContentOperationHandle handle, ContentOperationTextShowHandle* result)
{
	return SafeObjectConvert<OperationBase, OperationTextShow, ContentOperationHandle, ContentOperationTextShowHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShowArray(ContentOperationHandle handle, ContentOperationTextShowArrayHandle* result)
{
	return SafeObjectConvert<OperationBase, OperationTextShowArray, ContentOperationHandle, ContentOperationTextShowArrayHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle handle)
{
	return ObjectRelease<OperationBase, ContentOperationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperator(ContentOperationGenericHandle handle, ContentOperatorHandle* result)
{
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto oper = obj->GetOperator();
		auto base = oper.AddRefGet();
		*result = reinterpret_cast<ContentOperatorHandle>(base);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandsSize(ContentOperationGenericHandle handle, size_type* result)
{
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetOperandsSize();
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandAt(ContentOperationGenericHandle handle, size_type at, ObjectHandle* result)
{
	OperationGeneric* obj = reinterpret_cast<OperationGeneric*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto item = obj->GetOperandAt(at);
		auto ptr = item.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_Release(ContentOperationGenericHandle handle)
{
	return ObjectRelease<OperationGeneric, ContentOperationGenericHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetScale(ContentOperationTextFontHandle handle, IntegerObjectHandle* result)
{
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->GetScale();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetScale(ContentOperationTextFontHandle handle, IntegerObjectHandle data)
{
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	IntegerObject* name = reinterpret_cast<IntegerObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try
	{
		obj->SetScale(name);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetName(ContentOperationTextFontHandle handle, NameObjectHandle* result)
{
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->GetName();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetName(ContentOperationTextFontHandle handle, NameObjectHandle data)
{
	OperationTextFont* obj = reinterpret_cast<OperationTextFont*>(handle);
	NameObject* name = reinterpret_cast<NameObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try
	{
		obj->SetName(name);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_Release(ContentOperationTextFontHandle handle)
{
	return ObjectRelease<OperationTextFont, ContentOperationTextFontHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_SetValue(ContentOperationTextShowHandle handle, StringObjectHandle data)
{
	OperationTextShow* obj = reinterpret_cast<OperationTextShow*>(handle);
	StringObjectBase* str = reinterpret_cast<StringObjectBase*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

	try
	{
		obj->SetValue(str);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_GetValue(ContentOperationTextShowHandle handle, StringObjectHandle* result)
{
	OperationTextShow* obj = reinterpret_cast<OperationTextShow*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->GetValue();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_Release(ContentOperationTextShowHandle handle)
{
	return ObjectRelease<OperationTextShow, ContentOperationTextShowHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_GetValue(ContentOperationTextShowArrayHandle handle, ArrayObjectHandle* result)
{
	OperationTextShowArray* obj = reinterpret_cast<OperationTextShowArray*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->GetValue();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<ArrayObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_SetValue(ContentOperationTextShowArrayHandle handle, ArrayObjectHandle data)
{
	OperationTextShowArray* obj = reinterpret_cast<OperationTextShowArray*>(handle);
	MixedArrayObject* str = reinterpret_cast<MixedArrayObject*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

	try
	{
		obj->SetValue(str);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_Release(ContentOperationTextShowArrayHandle handle)
{
	return ObjectRelease<OperationTextShowArray, ContentOperationTextShowArrayHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetType(ContentOperatorHandle handle, ContentOperatorType* result)
{
	OperatorBase* obj = reinterpret_cast<OperatorBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
	case OperatorBase::Type::Unknown:
		*result = ContentOperatorType_Unknown; break;
	case OperatorBase::Type::LineWidth:
		*result = ContentOperatorType_LineWidth; break;
	case OperatorBase::Type::LineCap:
		*result = ContentOperatorType_LineCap; break;
	case OperatorBase::Type::LineJoin:
		*result = ContentOperatorType_LineJoin; break;
	case OperatorBase::Type::MiterLimit:
		*result = ContentOperatorType_MiterLimit; break;
	case OperatorBase::Type::DashPattern:
		*result = ContentOperatorType_DashPattern; break;
	case OperatorBase::Type::ColorRenderingIntent:
		*result = ContentOperatorType_ColorRenderingIntent; break;
	case OperatorBase::Type::Flatness:
		*result = ContentOperatorType_Flatness; break;
	case OperatorBase::Type::GraphicsState:
		*result = ContentOperatorType_GraphicsState; break;
	case OperatorBase::Type::SaveGraphicsState:
		*result = ContentOperatorType_SaveGraphicsState; break;
	case OperatorBase::Type::RestoreGraphicsState:
		*result = ContentOperatorType_RestoreGraphicsState; break;
	case OperatorBase::Type::TransformationMatrix:
		*result = ContentOperatorType_TransformationMatrix; break;
	case OperatorBase::Type::BeginSubpath:
		*result = ContentOperatorType_BeginSubpath; break;
	case OperatorBase::Type::Line:
		*result = ContentOperatorType_Line; break;
	case OperatorBase::Type::FullCurve:
		*result = ContentOperatorType_FullCurve; break;
	case OperatorBase::Type::FinalCurve:
		*result = ContentOperatorType_FinalCurve; break;
	case OperatorBase::Type::InitialCurve:
		*result = ContentOperatorType_InitialCurve; break;
	case OperatorBase::Type::CloseSubpath:
		*result = ContentOperatorType_CloseSubpath; break;
	case OperatorBase::Type::Rectangle:
		*result = ContentOperatorType_Rectangle; break;
	case OperatorBase::Type::Stroke:
		*result = ContentOperatorType_Stroke; break;
	case OperatorBase::Type::CloseAndStroke:
		*result = ContentOperatorType_CloseAndStroke; break;
	case OperatorBase::Type::FillPathNonzero:
		*result = ContentOperatorType_FillPathNonzero; break;
	case OperatorBase::Type::FillPathCompatibility:
		*result = ContentOperatorType_FillPathCompatibility; break;
	case OperatorBase::Type::FillPathEvenOdd:
		*result = ContentOperatorType_FillPathEvenOdd; break;
	case OperatorBase::Type::FillStrokeNonzero:
		*result = ContentOperatorType_FillStrokeNonzero; break;
	case OperatorBase::Type::FillStrokeEvenOdd:
		*result = ContentOperatorType_FillStrokeEvenOdd; break;
	case OperatorBase::Type::CloseFillStrokeNonzero:
		*result = ContentOperatorType_CloseFillStrokeNonzero; break;
	case OperatorBase::Type::CloseFillStrokeEvenOdd:
		*result = ContentOperatorType_CloseFillStrokeEvenOdd; break;
	case OperatorBase::Type::EndPath:
		*result = ContentOperatorType_EndPath; break;
	case OperatorBase::Type::ClipPathNonzero:
		*result = ContentOperatorType_ClipPathNonzero; break;
	case OperatorBase::Type::ClipPathEvenOdd:
		*result = ContentOperatorType_ClipPathEvenOdd; break;
	case OperatorBase::Type::BeginText:
		*result = ContentOperatorType_BeginText; break;
	case OperatorBase::Type::EndText:
		*result = ContentOperatorType_EndText; break;
	case OperatorBase::Type::CharacterSpacing:
		*result = ContentOperatorType_CharacterSpacing; break;
	case OperatorBase::Type::WordSpacing:
		*result = ContentOperatorType_WordSpacing; break;
	case OperatorBase::Type::HorizontalScaling:
		*result = ContentOperatorType_HorizontalScaling; break;
	case OperatorBase::Type::Leading:
		*result = ContentOperatorType_Leading; break;
	case OperatorBase::Type::TextFont:
		*result = ContentOperatorType_TextFont; break;
	case OperatorBase::Type::TextRenderingMode:
		*result = ContentOperatorType_TextRenderingMode; break;
	case OperatorBase::Type::TextRise:
		*result = ContentOperatorType_TextRise; break;
	case OperatorBase::Type::TextTranslate:
		*result = ContentOperatorType_TextTranslate; break;
	case OperatorBase::Type::TextTranslateLeading:
		*result = ContentOperatorType_TextTranslateLeading; break;
	case OperatorBase::Type::TextMatrix:
		*result = ContentOperatorType_TextMatrix; break;
	case OperatorBase::Type::TextNextLine:
		*result = ContentOperatorType_TextNextLine; break;
	case OperatorBase::Type::TextShow:
		*result = ContentOperatorType_TextShow; break;
	case OperatorBase::Type::TextShowArray:
		*result = ContentOperatorType_TextShowArray; break;
	case OperatorBase::Type::TextNextLineShow:
		*result = ContentOperatorType_TextNextLineShow; break;
	case OperatorBase::Type::TextNextLineShowSpacing:
		*result = ContentOperatorType_TextNextLineShowSpacing; break;
	case OperatorBase::Type::SetCharWidth:
		*result = ContentOperatorType_SetCharWidth; break;
	case OperatorBase::Type::SetCacheDevice:
		*result = ContentOperatorType_SetCacheDevice; break;
	case OperatorBase::Type::ColorSpaceStroke:
		*result = ContentOperatorType_ColorSpaceStroke; break;
	case OperatorBase::Type::ColorSpaceNonstroke:
		*result = ContentOperatorType_ColorSpaceNonstroke; break;
	case OperatorBase::Type::SetColorStroke:
		*result = ContentOperatorType_SetColorStroke; break;
	case OperatorBase::Type::SetColorStrokeExtended:
		*result = ContentOperatorType_SetColorStrokeExtended; break;
	case OperatorBase::Type::SetColorNonstroke:
		*result = ContentOperatorType_SetColorNonstroke; break;
	case OperatorBase::Type::SetColorNonstrokeExtended:
		*result = ContentOperatorType_SetColorNonstrokeExtended; break;
	case OperatorBase::Type::SetStrokingColorSpaceGray:
		*result = ContentOperatorType_SetStrokingColorSpaceGray; break;
	case OperatorBase::Type::SetNonstrokingColorSpaceGray:
		*result = ContentOperatorType_SetNonstrokingColorSpaceGray; break;
	case OperatorBase::Type::SetStrokingColorSpaceRGB:
		*result = ContentOperatorType_SetStrokingColorSpaceRGB; break;
	case OperatorBase::Type::SetNonstrokingColorSpaceRGB:
		*result = ContentOperatorType_SetNonstrokingColorSpaceRGB; break;
	case OperatorBase::Type::SetStrokingColorSpaceCMYK:
		*result = ContentOperatorType_SetStrokingColorSpaceCMYK; break;
	case OperatorBase::Type::SetNonstrokingColorSpaceCMYK:
		*result = ContentOperatorType_SetNonstrokingColorSpaceCMYK; break;
	case OperatorBase::Type::ShadingPaint:
		*result = ContentOperatorType_ShadingPaint; break;
	case OperatorBase::Type::BeginInlineImageObject:
		*result = ContentOperatorType_BeginInlineImageObject; break;
	case OperatorBase::Type::BeginInlineImageData:
		*result = ContentOperatorType_BeginInlineImageData; break;
	case OperatorBase::Type::EndInlineImageObject:
		*result = ContentOperatorType_EndInlineImageObject; break;
	case OperatorBase::Type::InvokeXObject:
		*result = ContentOperatorType_InvokeXObject; break;
	case OperatorBase::Type::DefineMarkedContentPoint:
		*result = ContentOperatorType_DefineMarkedContentPoint; break;
	case OperatorBase::Type::DefineMarkedContentPointWithPropertyList:
		*result = ContentOperatorType_DefineMarkedContentPointWithPropertyList; break;
	case OperatorBase::Type::BeginMarkedContentSequence:
		*result = ContentOperatorType_BeginMarkedContentSequence; break;
	case OperatorBase::Type::BeginMarkedContentSequenceWithPropertyList:
		*result = ContentOperatorType_BeginMarkedContentSequenceWithPropertyList; break;
	case OperatorBase::Type::EndMarkedContentSequence:
		*result = ContentOperatorType_EndMarkedContentSequence; break;
	case OperatorBase::Type::BeginCompatibilitySection:
		*result = ContentOperatorType_BeginCompatibilitySection; break;
	case OperatorBase::Type::EndCompatibilitySection:
		*result = ContentOperatorType_EndCompatibilitySection; break;
	default:
		assert(!"Unknown enum value");
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle handle, BufferHandle* result)
{
	OperatorBase* obj = reinterpret_cast<OperatorBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->Value();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_Release(ContentOperatorHandle handle)
{
	return ObjectRelease<OperatorBase, ContentOperatorHandle>(handle);
}
