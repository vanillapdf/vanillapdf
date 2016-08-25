#ifndef _C_CONTENTS_H
#define _C_CONTENTS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		ContentOperatorType_Unknown = 0,
		ContentOperatorType_LineWidth,
		ContentOperatorType_LineCap,
		ContentOperatorType_LineJoin,
		ContentOperatorType_MiterLimit,
		ContentOperatorType_DashPattern,
		ContentOperatorType_ColorRenderingIntent,
		ContentOperatorType_Flatness,
		ContentOperatorType_GraphicsState,
		ContentOperatorType_SaveGraphicsState,
		ContentOperatorType_RestoreGraphicsState,
		ContentOperatorType_TransformationMatrix,
		ContentOperatorType_BeginSubpath,
		ContentOperatorType_Line,
		ContentOperatorType_FullCurve,
		ContentOperatorType_FinalCurve,
		ContentOperatorType_InitialCurve,
		ContentOperatorType_CloseSubpath,
		ContentOperatorType_Rectangle,
		ContentOperatorType_Stroke,
		ContentOperatorType_CloseAndStroke,
		ContentOperatorType_FillPathNonzero,
		ContentOperatorType_FillPathCompatibility,
		ContentOperatorType_FillPathEvenOdd,
		ContentOperatorType_FillStrokeNonzero,
		ContentOperatorType_FillStrokeEvenOdd,
		ContentOperatorType_CloseFillStrokeNonzero,
		ContentOperatorType_CloseFillStrokeEvenOdd,
		ContentOperatorType_EndPath,
		ContentOperatorType_ClipPathNonzero,
		ContentOperatorType_ClipPathEvenOdd,
		ContentOperatorType_BeginText,
		ContentOperatorType_EndText,
		ContentOperatorType_CharacterSpacing,
		ContentOperatorType_WordSpacing,
		ContentOperatorType_HorizontalScaling,
		ContentOperatorType_Leading,
		ContentOperatorType_TextFont,
		ContentOperatorType_TextRenderingMode,
		ContentOperatorType_TextRise,
		ContentOperatorType_TextTranslate,
		ContentOperatorType_TextTranslateLeading,
		ContentOperatorType_TextMatrix,
		ContentOperatorType_TextNextLine,
		ContentOperatorType_TextShow,
		ContentOperatorType_TextShowArray,
		ContentOperatorType_TextNextLineShow,
		ContentOperatorType_TextNextLineShowSpacing,
		ContentOperatorType_SetCharWidth,
		ContentOperatorType_SetCacheDevice,
		ContentOperatorType_ColorSpaceStroke,
		ContentOperatorType_ColorSpaceNonstroke,
		ContentOperatorType_SetColorStroke,
		ContentOperatorType_SetColorStrokeExtended,
		ContentOperatorType_SetColorNonstroke,
		ContentOperatorType_SetColorNonstrokeExtended,
		ContentOperatorType_SetStrokingColorSpaceGray,
		ContentOperatorType_SetNonstrokingColorSpaceGray,
		ContentOperatorType_SetStrokingColorSpaceRGB,
		ContentOperatorType_SetNonstrokingColorSpaceRGB,
		ContentOperatorType_SetStrokingColorSpaceCMYK,
		ContentOperatorType_SetNonstrokingColorSpaceCMYK,
		ContentOperatorType_ShadingPaint,
		ContentOperatorType_BeginInlineImageObject,
		ContentOperatorType_BeginInlineImageData,
		ContentOperatorType_EndInlineImageObject,
		ContentOperatorType_InvokeXObject,
		ContentOperatorType_DefineMarkedContentPoint,
		ContentOperatorType_DefineMarkedContentPointWithPropertyList,
		ContentOperatorType_BeginMarkedContentSequence,
		ContentOperatorType_BeginMarkedContentSequenceWithPropertyList,
		ContentOperatorType_EndMarkedContentSequence,
		ContentOperatorType_BeginCompatibilitySection,
		ContentOperatorType_EndCompatibilitySection
	} ContentOperatorType, *PContentOperatorType;

	typedef enum
	{
		ContentOperationType_Generic = 0,
		ContentOperationType_LineWidth,
		ContentOperationType_LineCap,
		ContentOperationType_LineJoin,
		ContentOperationType_MiterLimit,
		ContentOperationType_DashPattern,
		ContentOperationType_ColorRenderingIntent,
		ContentOperationType_Flatness,
		ContentOperationType_GraphicsState,
		ContentOperationType_SaveGraphicsState,
		ContentOperationType_RestoreGraphicsState,
		ContentOperationType_TransformationMatrix,
		ContentOperationType_BeginSubpath,
		ContentOperationType_Line,
		ContentOperationType_FullCurve,
		ContentOperationType_FinalCurve,
		ContentOperationType_InitialCurve,
		ContentOperationType_CloseSubpath,
		ContentOperationType_Rectangle,
		ContentOperationType_Stroke,
		ContentOperationType_CloseAndStroke,
		ContentOperationType_FillPathNonzero,
		ContentOperationType_FillPathCompatibility,
		ContentOperationType_FillPathEvenOdd,
		ContentOperationType_FillStrokeNonzero,
		ContentOperationType_FillStrokeEvenOdd,
		ContentOperationType_CloseFillStrokeNonzero,
		ContentOperationType_CloseFillStrokeEvenOdd,
		ContentOperationType_EndPath,
		ContentOperationType_ClipPathNonzero,
		ContentOperationType_ClipPathEvenOdd,
		ContentOperationType_BeginText,
		ContentOperationType_EndText,
		ContentOperationType_CharacterSpacing,
		ContentOperationType_WordSpacing,
		ContentOperationType_HorizontalScaling,
		ContentOperationType_Leading,
		ContentOperationType_TextFont,
		ContentOperationType_TextRenderingMode,
		ContentOperationType_TextRise,
		ContentOperationType_TextTranslate,
		ContentOperationType_TextTranslateLeading,
		ContentOperationType_TextMatrix,
		ContentOperationType_TextNextLine,
		ContentOperationType_TextShow,
		ContentOperationType_TextShowArray,
		ContentOperationType_TextNextLineShow,
		ContentOperationType_TextNextLineShowSpacing,
		ContentOperationType_SetCharWidth,
		ContentOperationType_SetCacheDevice,
		ContentOperationType_ColorSpaceStroke,
		ContentOperationType_ColorSpaceNonstroke,
		ContentOperationType_SetColorStroke,
		ContentOperationType_SetColorStrokeExtended,
		ContentOperationType_SetColorNonstroke,
		ContentOperationType_SetColorNonstrokeExtended,
		ContentOperationType_SetStrokingColorSpaceGray,
		ContentOperationType_SetNonstrokingColorSpaceGray,
		ContentOperationType_SetStrokingColorSpaceRGB,
		ContentOperationType_SetNonstrokingColorSpaceRGB,
		ContentOperationType_SetStrokingColorSpaceCMYK,
		ContentOperationType_SetNonstrokingColorSpaceCMYK,
		ContentOperationType_ShadingPaint,
		ContentOperationType_BeginInlineImageObject,
		ContentOperationType_BeginInlineImageData,
		ContentOperationType_EndInlineImageObject,
		ContentOperationType_InvokeXObject,
		ContentOperationType_DefineMarkedContentPoint,
		ContentOperationType_DefineMarkedContentPointWithPropertyList,
		ContentOperationType_BeginMarkedContentSequence,
		ContentOperationType_BeginMarkedContentSequenceWithPropertyList,
		ContentOperationType_EndMarkedContentSequence,
		ContentOperationType_BeginCompatibilitySection,
		ContentOperationType_EndCompatibilitySection
	} ContentOperationType, *PContentOperationType;

	typedef enum
	{
		ContentObjectType_Text,
		ContentObjectType_InlineImage
	} ContentObjectType, *PContentObjectType;

	typedef enum
	{
		ContentInstructionType_Operation,
		ContentInstructionType_Object
	} ContentInstructionType, *PContentInstructionType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle handle, integer_type at, PContentInstructionHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle handle, PContentInstructionType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle handle, PContentOperationHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle handle, PContentObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle handle, PContentObjectType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle handle, PContentObjectTextHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle handle, PContentObjectInlineImageHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle handle, integer_type at, PContentOperationHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle handle, PBufferHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetType(ContentOperationHandle handle, PContentOperationType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToGeneric(ContentOperationHandle handle, PContentOperationGenericHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShow(ContentOperationHandle handle, PContentOperationTextShowHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShowArray(ContentOperationHandle handle, PContentOperationTextShowArrayHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToEndText(ContentOperationHandle handle, PContentOperationEndTextHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperator(ContentOperationGenericHandle handle, PContentOperatorHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandsSize(ContentOperationGenericHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandAt(ContentOperationGenericHandle handle, integer_type at, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_Release(ContentOperationGenericHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_GetValue(ContentOperationTextShowHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_SetValue(ContentOperationTextShowHandle handle, StringHandle data);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_Release(ContentOperationTextShowHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_GetValue(ContentOperationTextShowArrayHandle handle, PArrayHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_SetValue(ContentOperationTextShowArrayHandle handle, ArrayHandle data);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_Release(ContentOperationTextShowArrayHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetType(ContentOperatorHandle handle, PContentOperatorType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_Release(ContentOperatorHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENTS_H */
