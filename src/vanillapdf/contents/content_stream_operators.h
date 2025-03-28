#ifndef _CONTENT_STREAM_OPERATORS_H
#define _CONTENT_STREAM_OPERATORS_H

#include "contents/content_fwd.h"
#include "contents/content_stream_operator_base.h"

namespace vanillapdf {
namespace contents {

// Unknown operator from compatibility section
class UnknownOperator : public OperatorBase {
public:
    UnknownOperator() = default;
    explicit UnknownOperator(BufferPtr data) : _data(data) {}

    virtual Type GetOperatorType(void) const noexcept override { return Type::Unknown; }
    virtual BufferPtr Value(void) const override { return _data; }

private:
    BufferPtr _data;
};

#define GENERIC_OPERATOR_DEFINITION(Name, Val) \
class Name##Operator : public OperatorBase \
{ \
public: \
    virtual Type GetOperatorType(void) const noexcept override { return Type::Name; } \
    virtual BufferPtr Value(void) const override { return make_deferred_container<Buffer>(Val); } \
};

// General graphics state
GENERIC_OPERATOR_DEFINITION(LineWidth, "w");
GENERIC_OPERATOR_DEFINITION(LineCap, "J");
GENERIC_OPERATOR_DEFINITION(LineJoin, "j");
GENERIC_OPERATOR_DEFINITION(MiterLimit, "M");
GENERIC_OPERATOR_DEFINITION(DashPattern, "d");
GENERIC_OPERATOR_DEFINITION(ColorRenderingIntent, "ri");
GENERIC_OPERATOR_DEFINITION(Flatness, "i");
GENERIC_OPERATOR_DEFINITION(GraphicsState, "gs");

// Special graphics state
GENERIC_OPERATOR_DEFINITION(SaveGraphicsState, "q");
GENERIC_OPERATOR_DEFINITION(RestoreGraphicsState, "Q");
GENERIC_OPERATOR_DEFINITION(TransformationMatrix, "cm");

// Path construction
GENERIC_OPERATOR_DEFINITION(BeginSubpath, "m");
GENERIC_OPERATOR_DEFINITION(Line, "l");
GENERIC_OPERATOR_DEFINITION(FullCurve, "c");
GENERIC_OPERATOR_DEFINITION(FinalCurve, "v");
GENERIC_OPERATOR_DEFINITION(InitialCurve, "y");
GENERIC_OPERATOR_DEFINITION(CloseSubpath, "h");
GENERIC_OPERATOR_DEFINITION(Rectangle, "re");

// Path painting
GENERIC_OPERATOR_DEFINITION(Stroke, "S");
GENERIC_OPERATOR_DEFINITION(CloseAndStroke, "s");
GENERIC_OPERATOR_DEFINITION(FillPathNonzero, "f");
GENERIC_OPERATOR_DEFINITION(FillPathCompatibility, "F");
GENERIC_OPERATOR_DEFINITION(FillPathEvenOdd, "f*");
GENERIC_OPERATOR_DEFINITION(FillStrokeNonzero, "B");
GENERIC_OPERATOR_DEFINITION(FillStrokeEvenOdd, "B*");
GENERIC_OPERATOR_DEFINITION(CloseFillStrokeNonzero, "b");
GENERIC_OPERATOR_DEFINITION(CloseFillStrokeEvenOdd, "b*");
GENERIC_OPERATOR_DEFINITION(EndPath, "n");

// Clipping paths
GENERIC_OPERATOR_DEFINITION(ClipPathNonzero, "W");
GENERIC_OPERATOR_DEFINITION(ClipPathEvenOdd, "W*");

// Text objects
GENERIC_OPERATOR_DEFINITION(BeginText, "BT");
GENERIC_OPERATOR_DEFINITION(EndText, "ET");

// Text state
GENERIC_OPERATOR_DEFINITION(CharacterSpacing, "Tc");
GENERIC_OPERATOR_DEFINITION(WordSpacing, "Tw");
GENERIC_OPERATOR_DEFINITION(HorizontalScaling, "Tz");
GENERIC_OPERATOR_DEFINITION(Leading, "TL");
GENERIC_OPERATOR_DEFINITION(TextFont, "Tf");
GENERIC_OPERATOR_DEFINITION(TextRenderingMode, "Tr");
GENERIC_OPERATOR_DEFINITION(TextRise, "Ts");

// Text positioning
GENERIC_OPERATOR_DEFINITION(TextTranslate, "Td");
GENERIC_OPERATOR_DEFINITION(TextTranslateLeading, "TD");
GENERIC_OPERATOR_DEFINITION(TextMatrix, "Tm");
GENERIC_OPERATOR_DEFINITION(TextNextLine, "T*");

// Text showing
GENERIC_OPERATOR_DEFINITION(TextShow, "Tj");
GENERIC_OPERATOR_DEFINITION(TextShowArray, "TJ");
GENERIC_OPERATOR_DEFINITION(TextNextLineShow, "'");
GENERIC_OPERATOR_DEFINITION(TextNextLineShowSpacing, "\"");

// Type 3 fonts
GENERIC_OPERATOR_DEFINITION(SetCharWidth, "d0");
GENERIC_OPERATOR_DEFINITION(SetCacheDevice, "d1");

// Color
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceName, "CS");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceName, "cs");
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceDevice, "SC");
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceDeviceExtended, "SCN");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceDevice, "sc");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceDeviceExtended, "scn");
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceGray, "G");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceGray, "g");
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceRGB, "RG");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceRGB, "rg");
GENERIC_OPERATOR_DEFINITION(SetStrokingColorSpaceCMYK, "K");
GENERIC_OPERATOR_DEFINITION(SetNonstrokingColorSpaceCMYK, "k");

// Shading patterns
GENERIC_OPERATOR_DEFINITION(ShadingPaint, "sh");

// Inline images
GENERIC_OPERATOR_DEFINITION(BeginInlineImageObject, "BI");
GENERIC_OPERATOR_DEFINITION(BeginInlineImageData, "ID");
GENERIC_OPERATOR_DEFINITION(EndInlineImageObject, "EI");

// XObjects
GENERIC_OPERATOR_DEFINITION(InvokeXObject, "Do");

// Marked content
GENERIC_OPERATOR_DEFINITION(DefineMarkedContentPoint, "MP");
GENERIC_OPERATOR_DEFINITION(DefineMarkedContentPointWithPropertyList, "DP");
GENERIC_OPERATOR_DEFINITION(BeginMarkedContentSequence, "BMC");
GENERIC_OPERATOR_DEFINITION(BeginMarkedContentSequenceWithPropertyList, "BDC");
GENERIC_OPERATOR_DEFINITION(EndMarkedContentSequence, "EMC");

// Compatibility
GENERIC_OPERATOR_DEFINITION(BeginCompatibilitySection, "BX");
GENERIC_OPERATOR_DEFINITION(EndCompatibilitySection, "EX");

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATORS_H */
