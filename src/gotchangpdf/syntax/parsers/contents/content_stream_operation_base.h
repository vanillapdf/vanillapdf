#ifndef _CONTENT_STREAM_OPERATION_BASE_H
#define _CONTENT_STREAM_OPERATION_BASE_H

#include "syntax_fwd.h"
#include "content_stream_instruction_base.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			class OperationBase : public InstructionBase
			{
			public:
				enum class Type
				{
					Generic = 0,
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
				inline virtual InstructionBase::Type GetInstructionType(void) const _NOEXCEPT override { return InstructionBase::Type::Operation; }
				virtual Type GetOperationType(void) const _NOEXCEPT = 0;
			};

			typedef std::vector<OperationBasePtr> BaseOperationCollection;
		}
	}
}

#endif /* _CONTENT_STREAM_OPERATION_BASE_H */