#ifndef _CONTENT_STREAM_OPERATOR_BASE_H
#define _CONTENT_STREAM_OPERATOR_BASE_H

#include "contents/content_fwd.h"

#include "utils/unknown_interface.h"
#include "utils/buffer.h"

namespace vanillapdf {
namespace contents {

class OperatorBase : public IUnknown {
public:
	enum class Type {
		Undefined = 0,
		Unknown,
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
		SetStrokingColorSpaceName,
		SetNonstrokingColorSpaceName,
		SetStrokingColorSpaceDevice,
		SetStrokingColorSpaceDeviceExtended,
		SetNonstrokingColorSpaceDevice,
		SetNonstrokingColorSpaceDeviceExtended,
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

	virtual Type GetOperatorType(void) const noexcept = 0;
	virtual BufferPtr Value(void) const = 0;
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATOR_BASE_H */
