#ifndef _CONTENT_STREAM_OPERATOR_BASE_H
#define _CONTENT_STREAM_OPERATOR_BASE_H

#include "contents/content_fwd.h"

#include "utils/unknown_interface.h"
#include "utils/buffer.h"

namespace gotchangpdf {
namespace contents {

class OperatorBase : public virtual IUnknown, public IWeakReferenceable<OperationBase> {
public:
	enum class Type {
		Unknown = 0,
		LineWidth,
		LineCap,
		LineJoin,
		MiterLimit,
		DashPattern,
		ColorRenderingIntent,
		Flatness,
		GraphicsState,
		SaveGraphicsState,
		RestoreGraphicsState,
		TransformationMatrix,
		BeginSubpath,
		Line,
		FullCurve,
		FinalCurve,
		InitialCurve,
		CloseSubpath,
		Rectangle,
		Stroke,
		CloseAndStroke,
		FillPathNonzero,
		FillPathCompatibility,
		FillPathEvenOdd,
		FillStrokeNonzero,
		FillStrokeEvenOdd,
		CloseFillStrokeNonzero,
		CloseFillStrokeEvenOdd,
		EndPath,
		ClipPathNonzero,
		ClipPathEvenOdd,
		BeginText,
		EndText,
		CharacterSpacing,
		WordSpacing,
		HorizontalScaling,
		Leading,
		TextFont,
		TextRenderingMode,
		TextRise,
		TextTranslate,
		TextTranslateLeading,
		TextMatrix,
		TextNextLine,
		TextShow,
		TextShowArray,
		TextNextLineShow,
		TextNextLineShowSpacing,
		SetCharWidth,
		SetCacheDevice,
		ColorSpaceStroke,
		ColorSpaceNonstroke,
		SetColorStroke,
		SetColorStrokeExtended,
		SetColorNonstroke,
		SetColorNonstrokeExtended,
		SetStrokingColorSpaceGray,
		SetNonstrokingColorSpaceGray,
		SetStrokingColorSpaceRGB,
		SetNonstrokingColorSpaceRGB,
		SetStrokingColorSpaceCMYK,
		SetNonstrokingColorSpaceCMYK,
		ShadingPaint,
		BeginInlineImageObject,
		BeginInlineImageData,
		EndInlineImageObject,
		InvokeXObject,
		DefineMarkedContentPoint,
		DefineMarkedContentPointWithPropertyList,
		BeginMarkedContentSequence,
		BeginMarkedContentSequenceWithPropertyList,
		EndMarkedContentSequence,
		BeginCompatibilitySection,
		EndCompatibilitySection
	};

	virtual Type GetType(void) const noexcept = 0;
	virtual BufferPtr Value(void) const = 0;
};

} // contents
} // gotchangpdf

#endif /* _CONTENT_STREAM_OPERATOR_BASE_H */
