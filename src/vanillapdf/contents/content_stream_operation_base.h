#ifndef _CONTENT_STREAM_OPERATION_BASE_H
#define _CONTENT_STREAM_OPERATION_BASE_H

#include "contents/content_stream_instruction_base.h"

#include <vector>

namespace vanillapdf {
namespace contents {

class OperationBase : public InstructionBase {
public:
	enum class Type {
		Undefined = 0,
		Generic,
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

public:
	virtual InstructionBase::Type GetInstructionType(void) const noexcept override { return InstructionBase::Type::Operation; }
	virtual Type GetOperationType(void) const noexcept = 0;
};

typedef std::vector<OperationBasePtr> BaseOperationCollection;

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATION_BASE_H */