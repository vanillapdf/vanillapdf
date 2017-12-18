#ifndef _C_CONTENT_OPERATOR_H
#define _C_CONTENT_OPERATOR_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_content_operator.h
	* \brief This file contains class definitions for \ref ContentOperatorHandle
	*/

	/**
	* \class ContentOperatorHandle
	* \extends IUnknownHandle
	* \ingroup group_contents
	* \brief
	* An operator is a PDF keyword specifying some action
	* that shall be performed, such as painting a graphical
	* shape on the page.
	*/

	/**
	* \brief Available content operator types
	* \ingroup group_contents
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

		/**
		* \copydoc ContentOperationBeginTextHandle
		* \see \ref ContentOperationBeginTextHandle
		*/
		ContentOperatorType_BeginText,

		/**
		* \copydoc ContentOperationEndTextHandle
		* \see \ref ContentOperationEndTextHandle
		*/
		ContentOperatorType_EndText,
		ContentOperatorType_CharacterSpacing,
		ContentOperatorType_WordSpacing,
		ContentOperatorType_HorizontalScaling,
		ContentOperatorType_Leading,

		/**
		* \copydoc ContentOperationTextFontHandle
		* \see \ref ContentOperationTextFontHandle
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
		* \see \ref ContentOperationTextShowHandle
		*/
		ContentOperatorType_TextShow,

		/**
		* \copydoc ContentOperationTextShowArrayHandle
		* \see \ref ContentOperationTextShowArrayHandle
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
	* \memberof ContentOperatorHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetType(ContentOperatorHandle* handle, ContentOperatorType* result);

	/**
	* \brief Get byte representation of content operator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_GetValue(ContentOperatorHandle* handle, BufferHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ContentOperator_Release(ContentOperatorHandle* handle);

	/** @} */


#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENT_OPERATOR_H */
