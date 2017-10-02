#ifndef _C_CONTENTS_H
#define _C_CONTENTS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_contents.h
	* This file contains class definitions for working with compressed page contents
	*/

	/**
	* \class ContentsHandle
	* \extends IUnknownHandle
	* \ingroup Contents
	* \brief
	* A content stream is a PDF stream object whose data consists of
	* a sequence of instructions describing the graphical elements
	* to be painted on a page.
	* \see PageObjectHandle
	*/

	/**
	* \class ContentInstructionHandle
	* \extends IUnknownHandle
	* \ingroup Contents
	* \brief Base class for all content objects and operations.
	*/

	/**
	* \class ContentObjectHandle
	* \extends ContentInstructionHandle
	* \ingroup Contents
	* \brief Composed non-atomic content object
	*/

	/**
	* \class ContentObjectTextHandle
	* \extends ContentObjectHandle
	* \ingroup Contents
	* \brief
	* A PDF text object consists of operators that may show text strings,
	* move the text position, and set text state and certain other parameters.
	*/

	/**
	* \class ContentObjectInlineImageHandle
	* \extends ContentObjectHandle
	* \ingroup Contents
	* \brief
	* As an alternative to the image XObjects described in 8.9.5,
	* "Image Dictionaries", a sampled image may be specified in
	* the form of an inline image.
	*/

	/**
	* \class ContentOperationHandle
	* \extends ContentInstructionHandle
	* \ingroup Contents
	* \brief Atomic content operation.
	*/

	/**
	* \class ContentOperationGenericHandle
	* \extends ContentOperationHandle
	* \ingroup Contents
	* \brief Unresolved operation often containing unknown operator.
	*/

	/**
	* \class ContentOperationTextShowHandle
	* \extends ContentOperationHandle
	* \ingroup Contents
	* \brief Show a text string.
	*/

	/**
	* \class ContentOperationTextShowArrayHandle
	* \extends ContentOperationHandle
	* \ingroup Contents
	* \brief
	* Show one or more text strings,
	* allowing individual glyph positioning.
	*/

	/**
	* \class ContentOperationTextFontHandle
	* \extends ContentOperationHandle
	* \ingroup Contents
	* \brief Set the text font and the text font size.
	*/

	/**
	* \class ContentOperatorHandle
	* \extends IUnknownHandle
	* \ingroup Contents
	* \brief
	* An operator is a PDF keyword specifying some action
	* that shall be performed, such as painting a graphical
	* shape on the page.
	*/

	/**
	* \brief Available content operator types
	*/
	typedef enum {
		/**
		* \brief Unknown operator
		*/
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

		/**
		* \copydoc ContentOperationTextFontHandle
		* \see ContentOperationTextFontHandle
		*/
		ContentOperatorType_TextFont,
		ContentOperatorType_TextRenderingMode,
		ContentOperatorType_TextRise,
		ContentOperatorType_TextTranslate,
		ContentOperatorType_TextTranslateLeading,
		ContentOperatorType_TextMatrix,
		ContentOperatorType_TextNextLine,

		/**
		* \copydoc ContentOperationTextShowHandle
		* \see ContentOperationTextShowHandle
		*/
		ContentOperatorType_TextShow,

		/**
		* \copydoc ContentOperationTextShowArrayHandle
		* \see ContentOperationTextShowArrayHandle
		*/
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
	} ContentOperatorType;

	/**
	* \brief Available content operation types
	*/
	typedef enum {
		/**
		* \copydoc ContentOperationGenericHandle
		* \see ContentOperationGenericHandle
		*/
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

		/**
		* \copydoc ContentOperationTextFontHandle
		* \see ContentOperationTextFontHandle
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
		* \see ContentOperationTextShowHandle
		*/
		ContentOperationType_TextShow,

		/**
		* \copydoc ContentOperationTextShowArrayHandle
		* \see ContentOperationTextShowArrayHandle
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
	* \brief Pointer to ContentOperationType
	*/
	typedef ContentOperationType *PContentOperationType;

	/**
	* \brief Available content object types
	*/
	typedef enum {
		/**
		* \copydoc ContentObjectTextHandle
		* \see ContentObjectTextHandle
		*/
		ContentObjectType_Text,

		/**
		* \copydoc ContentObjectInlineImageHandle
		* \see ContentObjectInlineImageHandle
		*/
		ContentObjectType_InlineImage
	} ContentObjectType;

	/**
	* \brief Pointer to ContentObjectType
	*/
	typedef ContentObjectType *PContentObjectType;


	/**
	* \brief Available content instruction types
	*/
	typedef enum {
		/**
		* \copydoc ContentOperationHandle
		* \see ContentOperationHandle
		*/
		ContentInstructionType_Operation,

		/**
		* \copydoc ContentObjectHandle
		* \see ContentObjectHandle
		*/
		ContentInstructionType_Object
	} ContentInstructionType;

	/**
	* \memberof ContentsHandle
	* @{
	*/

	/**
	* \brief Return size of a collection
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionsSize(ContentsHandle handle, size_type* result);

	/**
	* \brief Get instruction at location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetInstructionAt(ContentsHandle handle, size_type at, ContentInstructionHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle);

	/** @} */

	/**
	* \memberof ContentInstructionHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_GetType(ContentInstructionHandle handle, ContentInstructionType* result);

	/**
	* \brief Reinterpret current object as ContentOperationHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToOperation(ContentInstructionHandle handle, ContentOperationHandle* result);

	/**
	* \brief Reinterpret current object as ContentObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_ToObject(ContentInstructionHandle handle, ContentObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentInstruction_Release(ContentInstructionHandle handle);

	/** @} */

	/**
	* \memberof ContentObjectHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_GetType(ContentObjectHandle handle, PContentObjectType result);

	/**
	* \brief Reinterpret current object as ContentObjectTextHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_ToText(ContentObjectHandle handle, ContentObjectTextHandle* result);

	/**
	* \brief Reinterpret current object as ContentObjectInlineImageHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_ToInlineImage(ContentObjectHandle handle, ContentObjectInlineImageHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObject_Release(ContentObjectHandle handle);

	/** @} */

	/**
	* \memberof ContentObjectTextHandle
	* @{
	*/

	/**
	* \brief Return size of a collection
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationsSize(ContentObjectTextHandle handle, size_type* result);

	/**
	* \brief Get operation at location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_GetOperationAt(ContentObjectTextHandle handle, size_type at, ContentOperationHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectText_Release(ContentObjectTextHandle handle);

	/** @} */

	/**
	* \memberof ContentObjectInlineImageHandle
	* @{
	*/

	/**
	* \brief Get meta-data dictionary for inline image
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetDictionary(ContentObjectInlineImageHandle handle, DictionaryObjectHandle* result);

	/**
	* \brief Get raw image data
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_GetData(ContentObjectInlineImageHandle handle, BufferHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentObjectInlineImage_Release(ContentObjectInlineImageHandle handle);

	/** @} */

	/**
	* \memberof ContentOperationHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_GetType(ContentOperationHandle handle, PContentOperationType result);

	/**
	* \brief Reinterpret current object as ContentOperationGenericHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToGeneric(ContentOperationHandle handle, ContentOperationGenericHandle* result);

	/**
	* \brief Reinterpret current object as ContentOperationTextFontHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToTextFont(ContentOperationHandle handle, ContentOperationTextFontHandle* result);

	/**
	* \brief Reinterpret current object as ContentOperationTextShowHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShow(ContentOperationHandle handle, ContentOperationTextShowHandle* result);

	/**
	* \brief Reinterpret current object as ContentOperationTextShowArrayHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToTextShowArray(ContentOperationHandle handle, ContentOperationTextShowArrayHandle* result);

	/**
	* \brief Reinterpret current object as ContentOperationEndTextHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_ToEndText(ContentOperationHandle handle, ContentOperationEndTextHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperation_Release(ContentOperationHandle handle);

	/** @} */

	/**
	* \memberof ContentOperationGenericHandle
	* @{
	*/

	/**
	* \brief Get operator from unknown operation
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperator(ContentOperationGenericHandle handle, ContentOperatorHandle* result);

	/**
	* \brief Get number of operands belonging to the operator
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandsSize(ContentOperationGenericHandle handle, size_type* result);

	/**
	* \brief Get operand at location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_GetOperandAt(ContentOperationGenericHandle handle, size_type at, ObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationGeneric_Release(ContentOperationGenericHandle handle);

	/** @} */

	/**
	* \memberof ContentOperationTextShowHandle
	* @{
	*/

	/**
	* \brief A text string to be shown.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_GetValue(ContentOperationTextShowHandle handle, StringObjectHandle* result);

	/**
	* \brief Set a new text string to be shown.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_SetValue(ContentOperationTextShowHandle handle, StringObjectHandle data);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShow_Release(ContentOperationTextShowHandle handle);

	/** @} */

	/**
	* \memberof ContentOperationTextShowArrayHandle
	* @{
	*/


	/**
	* \brief One or more text strings to be shown.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_GetValue(ContentOperationTextShowArrayHandle handle, ArrayObjectHandle* result);

	/**
	* \brief Set new text strings to be shown.
	*
	* Each element of array shall be either a string or a number.
	*
	* If the element is a string, this operator shall show the string.
	*
	* If it is a number, the operator shall adjust the text position by that amount.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_SetValue(ContentOperationTextShowArrayHandle handle, ArrayObjectHandle data);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextShowArray_Release(ContentOperationTextShowArrayHandle handle);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetName(ContentOperationTextFontHandle handle, NameObjectHandle* result);

	/**
	* \brief Set new font name.
	*
	* Font shall be the name of a font resource in the Font
	* subdictionary of the current resource dictionary.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetName(ContentOperationTextFontHandle handle, NameObjectHandle data);

	/**
	* \brief Get current font scaling.
	*
	* Scale shall be a number representing a scale factor.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextFont_GetScale(ContentOperationTextFontHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Set new font scaling.
	*
	* Scale shall be a number representing a scale factor.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextFont_SetScale(ContentOperationTextFontHandle handle, IntegerObjectHandle data);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperationTextFont_Release(ContentOperationTextFontHandle handle);

	/** @} */

	/**
	* \memberof ContentOperatorHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetType(ContentOperatorHandle handle, ContentOperatorType* result);

	/**
	* \brief Get byte representation of content operator
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle handle, BufferHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ContentOperator_Release(ContentOperatorHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENTS_H */
