#ifndef _CONTENT_FWD_H
#define _CONTENT_FWD_H

#include "utils/utils_fwd.h"
#include "utils/deferred.h"

namespace vanillapdf {
namespace contents {

// instructions
class InstructionBase; using InstructionBasePtr = Deferred<InstructionBase>;
class BaseInstructionCollection; using BaseInstructionCollectionPtr = DeferredContainer<BaseInstructionCollection>;

// basic operations
class OperationBase; using OperationBasePtr = Deferred<OperationBase>;
class OperationGeneric; using OperationGenericPtr = Deferred<OperationGeneric>;

// specialized operations
class OperationBeginText; using OperationBeginTextPtr = Deferred<OperationBeginText>;
class OperationEndText; using OperationEndTextPtr = Deferred<OperationEndText>;
class OperationTextFont; using OperationTextFontPtr = Deferred<OperationTextFont>;
class OperationTextShow; using OperationTextShowPtr = Deferred<OperationTextShow>;
class OperationTextShowArray; using OperationTextShowArrayPtr = Deferred<OperationTextShowArray>;
class OperationBeginInlineImageObject; using OperationBeginInlineImageObjectPtr = Deferred<OperationBeginInlineImageObject>;
class OperationBeginInlineImageData; using OperationBeginInlineImageDataPtr = Deferred<OperationBeginInlineImageData>;
class OperationEndInlineImageObject; using OperationEndInlineImageObjectPtr = Deferred<OperationEndInlineImageObject>;

// content objects
class ContentObjectBase; using ContentObjectBasePtr = Deferred<ContentObjectBase>;
class TextObject; using TextObjectPtr = Deferred<TextObject>;
class InlineImageObject; using InlineImageObjectPtr = Deferred<InlineImageObject>;

class OperatorBase;
class UnknownOperator;

class LineWidthOperator;
class LineCapOperator;
class LineJoinOperator;
class MiterLimitOperator;
class DashPatternOperator;
class ColorRenderingIntentOperator;
class FlatnessOperator;
class GraphicsStateOperator;

class SaveGraphicsStateOperator;
class RestoreGraphicsStateOperator;
class TransformationMatrixOperator;

class BeginSubpathOperator;
class LineOperator;
class FullCurveOperator;
class FinalCurveOperator;
class InitialCurveOperator;
class CloseSubpathOperator;
class RectangleOperator;

class StrokeOperator;
class CloseAndStrokeOperator;
class FillPathNonzeroOperator;
class FillPathCompatibilityOperator;
class FillPathEvenOddOperator;
class FillStrokeNonzeroOperator;
class FillStrokeEvenOddOperator;
class CloseFillStrokeNonzeroOperator;
class CloseFillStrokeEvenOddOperator;
class EndPathOperator;

class ClipPathNonzeroOperator;
class ClipPathEvenOddOperator;

class BeginTextOperator;
class EndTextOperator;

class CharacterSpacingOperator;
class WordSpacingOperator;
class HorizontalScalingOperator;
class LeadingOperator;
class TextFontOperator;
class TextRenderingModeOperator;
class TextRiseOperator;

class TextTranslateOperator;
class TextTranslateLeadingOperator;
class TextMatrixOperator;
class TextNextLineOperator;
class TextShowOperator;
class TextShowArrayOperator;
class TextNextLineShowOperator;
class TextNextLineShowSpacingOperator;

class SetCharWidthOperator;
class SetCacheDeviceOperator;

class SetStrokingColorSpaceNameOperator;
class SetNonstrokingColorSpaceNameOperator;
class SetStrokingColorSpaceDeviceOperator;
class SetStrokingColorSpaceDeviceExtendedOperator;
class SetNonstrokingColorSpaceDeviceOperator;
class SetNonstrokingColorSpaceDeviceExtendedOperator;
class SetStrokingColorSpaceGrayOperator;
class SetNonstrokingColorSpaceGrayOperator;
class SetStrokingColorSpaceRGBOperator;
class SetNonstrokingColorSpaceRGBOperator;
class SetStrokingColorSpaceCMYKOperator;
class SetNonstrokingColorSpaceCMYKOperator;

class ShadingPaintOperator;

class BeginInlineImageObjectOperator;
class BeginInlineImageDataOperator;
class EndInlineImageObjectOperator;

class InvokeXObjectOperator;

class DefineMarkedContentPointOperator;
class DefineMarkedContentPointWithPropertyListOperator;
class BeginMarkedContentSequenceOperator;
class BeginMarkedContentSequenceWithPropertyListOperator;
class EndMarkedContentSequenceOperator;

class BeginCompatibilitySectionOperator;
class EndCompatibilitySectionOperator;

// CMap

class BaseFontRange; using BaseFontRangePtr = Deferred<BaseFontRange>;

typedef Deferred<OperationBase> OperationBasePtr;
typedef Deferred<OperationGeneric> OperationGenericPtr;

typedef Deferred<OperatorBase> OperatorBasePtr;
typedef Deferred<UnknownOperator> UnknownOperatorPtr;

typedef Deferred<LineWidthOperator> LineWidthOperatorPtr;
typedef Deferred<LineCapOperator> LineCapOperatorPtr;
typedef Deferred<LineJoinOperator> LineJoinOperatorPtr;
typedef Deferred<MiterLimitOperator> MiterLimitOperatorPtr;
typedef Deferred<DashPatternOperator> DashPatternOperatorPtr;
typedef Deferred<ColorRenderingIntentOperator> ColorRenderingIntentOperatorPtr;
typedef Deferred<FlatnessOperator> FlatnessOperatorPtr;
typedef Deferred<GraphicsStateOperator> GraphicsStateOperatorPtr;

typedef Deferred<SaveGraphicsStateOperator> SaveGraphicsStateOperatorPtr;
typedef Deferred<RestoreGraphicsStateOperator> RestoreGraphicsStateOperatorPtr;
typedef Deferred<TransformationMatrixOperator> TransformationMatrixOperatorPtr;

typedef Deferred<BeginSubpathOperator> BeginSubpathOperatorPtr;
typedef Deferred<LineOperator> LineOperatorPtr;
typedef Deferred<FullCurveOperator> FullCurveOperatorPtr;
typedef Deferred<FinalCurveOperator> FinalCurveOperatorPtr;
typedef Deferred<InitialCurveOperator> InitialCurveOperatorPtr;
typedef Deferred<CloseSubpathOperator> CloseSubpathOperatorPtr;
typedef Deferred<RectangleOperator> RectangleOperatorPtr;

typedef Deferred<StrokeOperator> StrokeOperatorPtr;
typedef Deferred<CloseAndStrokeOperator> CloseAndStrokeOperatorPtr;
typedef Deferred<FillPathNonzeroOperator> FillPathNonzeroOperatorPtr;
typedef Deferred<FillPathCompatibilityOperator> FillPathCompatibilityOperatorPtr;
typedef Deferred<FillPathEvenOddOperator> FillPathEvenOddOperatorPtr;
typedef Deferred<FillStrokeNonzeroOperator> FillStrokeNonzeroOperatorPtr;
typedef Deferred<FillStrokeEvenOddOperator> FillStrokeEvenOddOperatorPtr;
typedef Deferred<CloseFillStrokeNonzeroOperator> CloseFillStrokeNonzeroOperatorPtr;
typedef Deferred<CloseFillStrokeEvenOddOperator> CloseFillStrokeEvenOddOperatorPtr;
typedef Deferred<EndPathOperator> EndPathOperatorPtr;

typedef Deferred<ClipPathNonzeroOperator> ClipPathNonzeroOperatorPtr;
typedef Deferred<ClipPathEvenOddOperator> ClipPathEvenOddOperatorPtr;

typedef Deferred<BeginTextOperator> BeginTextOperatorPtr;
typedef Deferred<EndTextOperator> EndTextOperatorPtr;

typedef Deferred<CharacterSpacingOperator> CharacterSpacingOperatorPtr;
typedef Deferred<WordSpacingOperator> WordSpacingOperatorPtr;
typedef Deferred<HorizontalScalingOperator> HorizontalScalingOperatorPtr;
typedef Deferred<LeadingOperator> LeadingOperatorPtr;
typedef Deferred<TextFontOperator> TextFontOperatorPtr;
typedef Deferred<TextRenderingModeOperator> TextRenderingModeOperatorPtr;
typedef Deferred<TextRiseOperator> TextRiseOperatorPtr;

typedef Deferred<TextTranslateOperator> TextTranslateOperatorPtr;
typedef Deferred<TextTranslateLeadingOperator> TextTranslateLeadingOperatorPtr;
typedef Deferred<TextMatrixOperator> TextMatrixOperatorPtr;
typedef Deferred<TextNextLineOperator> TextNextLineOperatorPtr;

typedef Deferred<TextShowOperator> TextShowOperatorPtr;
typedef Deferred<TextShowArrayOperator> TextShowArrayOperatorPtr;
typedef Deferred<TextNextLineShowOperator> TextNextLineShowOperatorPtr;
typedef Deferred<TextNextLineShowSpacingOperator> TextNextLineShowSpacingOperatorPtr;

typedef Deferred<SetCharWidthOperator> SetCharWidthOperatorPtr;
typedef Deferred<SetCacheDeviceOperator> SetCacheDeviceOperatorPtr;

typedef Deferred<SetStrokingColorSpaceNameOperator> SetStrokingColorSpaceNameOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceNameOperator> SetNonstrokingColorSpaceNameOperatorPtr;
typedef Deferred<SetStrokingColorSpaceDeviceOperator> SetStrokingColorSpaceDeviceOperatorPtr;
typedef Deferred<SetStrokingColorSpaceDeviceExtendedOperator> SetStrokingColorSpaceDeviceExtendedOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceDeviceOperator> SetNonstrokingColorSpaceDeviceOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceDeviceExtendedOperator> SetNonstrokingColorSpaceDeviceExtendedOperatorPtr;
typedef Deferred<SetStrokingColorSpaceGrayOperator> SetStrokingColorSpaceGrayOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceGrayOperator> SetNonstrokingColorSpaceGrayOperatorPtr;
typedef Deferred<SetStrokingColorSpaceRGBOperator> SetStrokingColorSpaceRGBOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceRGBOperator> SetNonstrokingColorSpaceRGBOperatorPtr;
typedef Deferred<SetStrokingColorSpaceCMYKOperator> SetStrokingColorSpaceCMYKOperatorPtr;
typedef Deferred<SetNonstrokingColorSpaceCMYKOperator> SetNonstrokingColorSpaceCMYKOperatorPtr;

typedef Deferred<ShadingPaintOperator> ShadingPaintOperatorPtr;

typedef Deferred<BeginInlineImageObjectOperator> BeginInlineImageObjectOperatorPtr;
typedef Deferred<BeginInlineImageDataOperator> BeginInlineImageDataOperatorPtr;
typedef Deferred<EndInlineImageObjectOperator> EndInlineImageObjectOperatorPtr;

typedef Deferred<InvokeXObjectOperator> InvokeXObjectOperatorPtr;

typedef Deferred<DefineMarkedContentPointOperator> DefineMarkedContentPointOperatorPtr;
typedef Deferred<DefineMarkedContentPointWithPropertyListOperator> DefineMarkedContentPointWithPropertyListOperatorPtr;
typedef Deferred<BeginMarkedContentSequenceOperator> BeginMarkedContentSequenceOperatorPtr;
typedef Deferred<BeginMarkedContentSequenceWithPropertyListOperator> BeginMarkedContentSequenceWithPropertyListOperatorPtr;
typedef Deferred<EndMarkedContentSequenceOperator> EndMarkedContentSequenceOperatorPtr;

typedef Deferred<BeginCompatibilitySectionOperator> BeginCompatibilitySectionOperatorPtr;
typedef Deferred<EndCompatibilitySectionOperator> EndCompatibilitySectionOperatorPtr;

} // contents
} // vanillapdf

#endif /* _CONTENT_FWD_H */
