#include "precompiled.h"
#include "contents.h"

#include "c_contents.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::syntax::contents;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle handle, out_integer_type result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = obj->GetInstructionsSize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle handle, integer_type at, PContentInstructionHandle result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		ContentStreamInstructionBaseVisitor visitor;
		auto instruction = obj->GetInstructionAt(at);
		auto base = instruction.apply_visitor(visitor);
		base->AddRef();
		*result = reinterpret_cast<ContentInstructionHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle handle, PContentInstructionType result)
{
	contents::InstructionBase* obj = reinterpret_cast<contents::InstructionBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	switch (obj->GetInstructionType()) {
	case contents::InstructionBase::Type::Object:
		*result = ContentInstructionType_Object; break;
	case contents::InstructionBase::Type::Operation:
		*result = ContentInstructionType_Operation; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle handle, PContentOperationHandle result)
{
	contents::InstructionBase* obj = reinterpret_cast<contents::InstructionBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		contents::Operation* converted = dynamic_cast<contents::Operation*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<ContentOperationHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle handle, PContentObjectHandle result)
{
	contents::InstructionBase* obj = reinterpret_cast<contents::InstructionBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		ContentObjectBase* converted = dynamic_cast<ContentObjectBase*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<ContentObjectHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle handle)
{
	contents::InstructionBase* obj = reinterpret_cast<contents::InstructionBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle handle, PContentObjectType result)
{
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	switch (obj->GetType()) {
	case ContentObjectBase::Type::TextObject:
		*result = ContentObjectType_Text; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle handle, PContentObjectTextHandle result)
{
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		TextObject* converted = dynamic_cast<TextObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<ContentObjectTextHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle handle)
{
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle handle, out_integer_type result)
{
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		// TODO
		return GOTCHANG_PDF_ERROR_GENERAL;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle handle, integer_type at, PContentOperationHandle result)
{
	ContentObjectBase* obj = reinterpret_cast<ContentObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		// TODO
		return GOTCHANG_PDF_ERROR_GENERAL;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetType(ContentOperationHandle handle, PContentOperationType result)
{
	contents::Operation* obj = reinterpret_cast<contents::Operation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	switch (obj->GetOperationType()) {
	case contents::Operation::Type::Generic:
		*result = ContentOperationType_Generic; break;
	case contents::Operation::Type::LineWidth:
		*result = ContentOperationType_LineWidth; break;
	case contents::Operation::Type::LineCap:
		*result = ContentOperationType_LineCap; break;
	case contents::Operation::Type::LineJoin:
		*result = ContentOperationType_LineJoin; break;
	case contents::Operation::Type::MiterLimit:
		*result = ContentOperationType_MiterLimit; break;
	case contents::Operation::Type::DashPattern:
		*result = ContentOperationType_DashPattern; break;
	case contents::Operation::Type::ColorRenderingIntent:
		*result = ContentOperationType_ColorRenderingIntent; break;
	case contents::Operation::Type::Flatness:
		*result = ContentOperationType_Flatness; break;
	case contents::Operation::Type::GraphicsState:
		*result = ContentOperationType_GraphicsState; break;
	case contents::Operation::Type::SaveGraphicsState:
		*result = ContentOperationType_SaveGraphicsState; break;
	case contents::Operation::Type::RestoreGraphicsState:
		*result = ContentOperationType_RestoreGraphicsState; break;
	case contents::Operation::Type::TransformationMatrix:
		*result = ContentOperationType_TransformationMatrix; break;
	case contents::Operation::Type::BeginSubpath:
		*result = ContentOperationType_BeginSubpath; break;
	case contents::Operation::Type::Line:
		*result = ContentOperationType_Line; break;
	case contents::Operation::Type::FullCurve:
		*result = ContentOperationType_FullCurve; break;
	case contents::Operation::Type::FinalCurve:
		*result = ContentOperationType_FinalCurve; break;
	case contents::Operation::Type::InitialCurve:
		*result = ContentOperationType_InitialCurve; break;
	case contents::Operation::Type::CloseSubpath:
		*result = ContentOperationType_CloseSubpath; break;
	case contents::Operation::Type::Rectangle:
		*result = ContentOperationType_Rectangle; break;
	case contents::Operation::Type::Stroke:
		*result = ContentOperationType_Stroke; break;
	case contents::Operation::Type::CloseAndStroke:
		*result = ContentOperationType_CloseAndStroke; break;
	case contents::Operation::Type::FillPathNonzero:
		*result = ContentOperationType_FillPathNonzero; break;
	case contents::Operation::Type::FillPathCompatibility:
		*result = ContentOperationType_FillPathCompatibility; break;
	case contents::Operation::Type::FillPathEvenOdd:
		*result = ContentOperationType_FillPathEvenOdd; break;
	case contents::Operation::Type::FillStrokeNonzero:
		*result = ContentOperationType_FillStrokeNonzero; break;
	case contents::Operation::Type::FillStrokeEvenOdd:
		*result = ContentOperationType_FillStrokeEvenOdd; break;
	case contents::Operation::Type::CloseFillStrokeNonzero:
		*result = ContentOperationType_CloseFillStrokeNonzero; break;
	case contents::Operation::Type::CloseFillStrokeEvenOdd:
		*result = ContentOperationType_CloseFillStrokeEvenOdd; break;
	case contents::Operation::Type::EndPath:
		*result = ContentOperationType_EndPath; break;
	case contents::Operation::Type::ClipPathNonzero:
		*result = ContentOperationType_ClipPathNonzero; break;
	case contents::Operation::Type::ClipPathEvenOdd:
		*result = ContentOperationType_ClipPathEvenOdd; break;
	case contents::Operation::Type::BeginText:
		*result = ContentOperationType_BeginText; break;
	case contents::Operation::Type::EndText:
		*result = ContentOperationType_EndText; break;
	case contents::Operation::Type::CharacterSpacing:
		*result = ContentOperationType_CharacterSpacing; break;
	case contents::Operation::Type::WordSpacing:
		*result = ContentOperationType_WordSpacing; break;
	case contents::Operation::Type::HorizontalScaling:
		*result = ContentOperationType_HorizontalScaling; break;
	case contents::Operation::Type::Leading:
		*result = ContentOperationType_Leading; break;
	case contents::Operation::Type::TextFont:
		*result = ContentOperationType_TextFont; break;
	case contents::Operation::Type::TextRenderingMode:
		*result = ContentOperationType_TextRenderingMode; break;
	case contents::Operation::Type::TextRise:
		*result = ContentOperationType_TextRise; break;
	case contents::Operation::Type::TextTranslate:
		*result = ContentOperationType_TextTranslate; break;
	case contents::Operation::Type::TextTranslateLeading:
		*result = ContentOperationType_TextTranslateLeading; break;
	case contents::Operation::Type::TextMatrix:
		*result = ContentOperationType_TextMatrix; break;
	case contents::Operation::Type::TextNextLine:
		*result = ContentOperationType_TextNextLine; break;
	case contents::Operation::Type::TextShow:
		*result = ContentOperationType_TextShow; break;
	case contents::Operation::Type::TextShowArray:
		*result = ContentOperationType_TextShowArray; break;
	case contents::Operation::Type::TextNextLineShow:
		*result = ContentOperationType_TextNextLineShow; break;
	case contents::Operation::Type::TextNextLineShowSpacing:
		*result = ContentOperationType_TextNextLineShowSpacing; break;
	case contents::Operation::Type::SetCharWidth:
		*result = ContentOperationType_SetCharWidth; break;
	case contents::Operation::Type::SetCacheDevice:
		*result = ContentOperationType_SetCacheDevice; break;
	case contents::Operation::Type::ColorSpaceStroke:
		*result = ContentOperationType_ColorSpaceStroke; break;
	case contents::Operation::Type::ColorSpaceNonstroke:
		*result = ContentOperationType_ColorSpaceNonstroke; break;
	case contents::Operation::Type::SetColorStroke:
		*result = ContentOperationType_SetColorStroke; break;
	case contents::Operation::Type::SetColorStrokeExtended:
		*result = ContentOperationType_SetColorStrokeExtended; break;
	case contents::Operation::Type::SetColorNonstroke:
		*result = ContentOperationType_SetColorNonstroke; break;
	case contents::Operation::Type::SetColorNonstrokeExtended:
		*result = ContentOperationType_SetColorNonstrokeExtended; break;
	case contents::Operation::Type::SetStrokingColorSpaceGray:
		*result = ContentOperationType_SetStrokingColorSpaceGray; break;
	case contents::Operation::Type::SetNonstrokingColorSpaceGray:
		*result = ContentOperationType_SetNonstrokingColorSpaceGray; break;
	case contents::Operation::Type::SetStrokingColorSpaceRGB:
		*result = ContentOperationType_SetStrokingColorSpaceRGB; break;
	case contents::Operation::Type::SetNonstrokingColorSpaceRGB:
		*result = ContentOperationType_SetNonstrokingColorSpaceRGB; break;
	case contents::Operation::Type::SetStrokingColorSpaceCMYK:
		*result = ContentOperationType_SetStrokingColorSpaceCMYK; break;
	case contents::Operation::Type::SetNonstrokingColorSpaceCMYK:
		*result = ContentOperationType_SetNonstrokingColorSpaceCMYK; break;
	case contents::Operation::Type::ShadingPaint:
		*result = ContentOperationType_ShadingPaint; break;
	case contents::Operation::Type::BeginInlineImageObject:
		*result = ContentOperationType_BeginInlineImageObject; break;
	case contents::Operation::Type::BeginInlineImageData:
		*result = ContentOperationType_BeginInlineImageData; break;
	case contents::Operation::Type::EndInlineImageObject:
		*result = ContentOperationType_EndInlineImageObject; break;
	case contents::Operation::Type::InvokeXObject:
		*result = ContentOperationType_InvokeXObject; break;
	case contents::Operation::Type::DefineMarkedContentPoint:
		*result = ContentOperationType_DefineMarkedContentPoint; break;
	case contents::Operation::Type::DefineMarkedContentPointWithPropertyList:
		*result = ContentOperationType_DefineMarkedContentPointWithPropertyList; break;
	case contents::Operation::Type::BeginMarkedContentSequence:
		*result = ContentOperationType_BeginMarkedContentSequence; break;
	case contents::Operation::Type::BeginMarkedContentSequenceWithPropertyList:
		*result = ContentOperationType_BeginMarkedContentSequenceWithPropertyList; break;
	case contents::Operation::Type::EndMarkedContentSequence:
		*result = ContentOperationType_EndMarkedContentSequence; break;
	case contents::Operation::Type::BeginCompatibilitySection:
		*result = ContentOperationType_BeginCompatibilitySection; break;
	case contents::Operation::Type::EndCompatibilitySection:
		*result = ContentOperationType_EndCompatibilitySection; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle handle)
{
	contents::Operation* obj = reinterpret_cast<contents::Operation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetOperator(ContentOperationHandle handle, PContentOperatorHandle result)
{
	contents::Operation* obj = reinterpret_cast<contents::Operation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		contents::OperatorBaseVisitor visitor;
		auto oper = obj->GetOperator();
		auto base = oper.apply_visitor(visitor);
		base->AddRef();
		*result = reinterpret_cast<ContentOperatorHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetOperandsSize(ContentOperationHandle handle, out_integer_type result)
{
	contents::Operation* obj = reinterpret_cast<contents::Operation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = obj->GetOperandsSize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetOperandAt(ContentOperationHandle handle, integer_type at, PObjectHandle result)
{
	contents::Operation* obj = reinterpret_cast<contents::Operation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		ObjectBaseAddRefVisitor visitor;
		auto item = obj->GetOperandAt(at);
		auto ptr = item.apply_visitor(visitor);
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetType(ContentOperatorHandle handle, PContentOperatorType result)
{
	contents::OperatorBase* obj = reinterpret_cast<contents::OperatorBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

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
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle handle, out_string_type result)
{
	OperatorBase* obj = reinterpret_cast<OperatorBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		auto val = obj->Value();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
