#ifndef _C_CONTENT_OPERATION_H
#define _C_CONTENT_OPERATION_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_content_operation.h
	* \brief This file contains class definitions for \ref ContentOperationHandle
	*/

	/**
	* \class ContentOperationHandle
	* \extends ContentInstructionHandle
	* \ingroup group_contents
	* \brief Atomic operation modifying graphics state parameters.
	*/

	/**
	* \class ContentOperationGenericHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief Unresolved operation often containing unknown operator.
	*/

	/**
	* \class ContentOperationTextShowHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief Stream operation for showing a simple text string.
	*/

	/**
	* \class ContentOperationTextShowArrayHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief
	* Show one or more text strings, allowing individual glyph positioning.
	*/

	/**
	* \class ContentOperationTextFontHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief Set the text font and the text font size.
	*/

	/**
	* \class ContentOperationBeginTextHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief Starts a new \ref ContentObjectTextHandle section
	*/

	/**
	* \class ContentOperationEndTextHandle
	* \extends ContentOperationHandle
	* \ingroup group_contents
	* \brief Terminates the \ref ContentObjectTextHandle started with \ref ContentOperationBeginTextHandle
	*/

	/**
	* \brief Derived types of \ref ContentOperationHandle
	* \ingroup group_contents
	*/
	typedef enum {
		ContentOperationType_Undefined = 0,
		/**
		* \copydoc ContentOperationGenericHandle
		* \see \ref ContentOperationGenericHandle
		*/
		ContentOperationType_Generic,
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

		/**
		* \copydoc ContentOperationTextFontHandle
		* \see \ref ContentOperationTextFontHandle
		*/
		ContentOperationType_TextFont,
		ContentOperationType_TextRenderingMode,
		ContentOperationType_TextRise,
		ContentOperationType_TextTranslate,
		ContentOperationType_TextTranslateLeading,
		ContentOperationType_TextMatrix,
		ContentOperationType_TextNextLine,

		/**
		* \copydoc ContentOperationTextShowHandle
		* \see \ref ContentOperationTextShowHandle
		*/
		ContentOperationType_TextShow,

		/**
		* \copydoc ContentOperationTextShowArrayHandle
		* \see \ref ContentOperationTextShowArrayHandle
		*/
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
	} ContentOperationType;


	/**
	* \memberof ContentOperationHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_GetOperationType(ContentOperationHandle* handle, ContentOperationType* result);

	/**
	* \brief Reinterpret current object as \ref ContentOperationBeginTextHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToBeginText(ContentOperationHandle* handle, ContentOperationBeginTextHandle** result);

	/**
	* \brief Reinterpret current object as \ref ContentOperationEndTextHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToEndText(ContentOperationHandle* handle, ContentOperationEndTextHandle** result);

	/**
	* \brief Reinterpret current object as \ref ContentInstructionHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_ToInstruction(ContentOperationHandle* handle, ContentInstructionHandle** result);

	/**
	* \brief Convert \ref ContentInstructionHandle to \ref ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_FromInstruction(ContentInstructionHandle* handle, ContentOperationHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle* handle);

	/** @} */

	/**
	* \memberof ContentOperationGenericHandle
	* @{
	*/

	/**
	* \brief Get operator from unknown operation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperator(ContentOperationGenericHandle* handle, ContentOperatorHandle** result);

	/**
	* \brief Get number of operands belonging to the operator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandsSize(ContentOperationGenericHandle* handle, size_type* result);

	/**
	* \brief Get operand at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandAt(ContentOperationGenericHandle* handle, size_type at, ObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_ToContentOperation(ContentOperationGenericHandle* handle, ContentOperationHandle** result);

	/**
	* \brief Convert \ref ContentOperationHandle to \ref ContentOperationGenericHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_FromContentOperation(ContentOperationHandle* handle, ContentOperationGenericHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationGeneric_Release(ContentOperationGenericHandle* handle);

	/** @} */

	/**
	* \memberof ContentOperationTextShowHandle
	* @{
	*/

	/**
	* \brief A text string to be shown.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_GetValue(ContentOperationTextShowHandle* handle, StringObjectHandle** result);

	/**
	* \brief Set a new text string to be shown.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_SetValue(ContentOperationTextShowHandle* handle, StringObjectHandle* data);

	/**
	* \brief Reinterpret current object as \ref ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_ToContentOperation(ContentOperationTextShowHandle* handle, ContentOperationHandle** result);

	/**
	* \brief Convert \ref ContentOperationHandle to \ref ContentOperationTextShowHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextShowHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShow_Release(ContentOperationTextShowHandle* handle);

	/** @} */

	/**
	* \memberof ContentOperationTextShowArrayHandle
	* @{
	*/

	/**
	* \brief One or more text strings to be shown.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_GetValue(ContentOperationTextShowArrayHandle* handle, ArrayObjectHandle** result);

	/**
	* \brief Set new text strings to be shown.
	*
	* Each element of array shall be either a string or a number.
	*
	* If the element is a string, this operator shall show the string.
	*
	* If it is a number, the operator shall adjust the text position by that amount.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_SetValue(ContentOperationTextShowArrayHandle* handle, ArrayObjectHandle* data);

	/**
	* \brief Reinterpret current object as \ref ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_ToContentOperation(ContentOperationTextShowArrayHandle* handle, ContentOperationHandle** result);

	/**
	* \brief Convert \ref ContentOperationHandle to \ref ContentOperationTextShowArrayHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextShowArrayHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_Release(ContentOperationTextShowArrayHandle* handle);

	/** @} */

	/**
	* \memberof ContentOperationTextFontHandle
	* @{
	*/

	/**
	* \brief Get name of the font to be set.
	*
	* Font shall be the name of a font resource in the Font
	* subdictionary of the current resource dictionary.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetName(ContentOperationTextFontHandle* handle, NameObjectHandle** result);

	/**
	* \brief Set new font name.
	*
	* Font shall be the name of a font resource in the Font
	* subdictionary of the current resource dictionary.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetName(ContentOperationTextFontHandle* handle, NameObjectHandle* data);

	/**
	* \brief Get current font scaling.
	*
	* Scale shall be a number representing a scale factor.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetScale(ContentOperationTextFontHandle* handle, IntegerObjectHandle** result);

	/**
	* \brief Set new font scaling.
	*
	* Scale shall be a number representing a scale factor.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetScale(ContentOperationTextFontHandle* handle, IntegerObjectHandle* data);

	/**
	* \brief Reinterpret current object as \ref ContentOperationHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_ToContentOperation(ContentOperationTextFontHandle* handle, ContentOperationHandle** result);

	/**
	* \brief Convert \ref ContentOperationHandle to \ref ContentOperationTextFontHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_FromContentOperation(ContentOperationHandle* handle, ContentOperationTextFontHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationTextFont_Release(ContentOperationTextFontHandle* handle);

	/** @} */

	/**
	* \memberof ContentOperationEndTextHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperationEndText_Release(ContentOperationEndTextHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_OPERATION_H */
