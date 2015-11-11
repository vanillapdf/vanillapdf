#include "precompiled.h"
#include "content_stream_grammar.h"
//#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ContentStreamOperandGrammar::ContentStreamOperandGrammar() :
			base_type(start, "Content stream operand grammar")
		{
			start %=
				array_object(qi::_r1)
				| dictionary_object(qi::_r1)
				| boolean_object
				| function_object
				| real_object
				| integer_object
				| name_object
				| null_object
				| literal_string_object
				| hexadecimal_string_object;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContentStreamOperatorGrammar::ContentStreamOperatorGrammar() :
			base_type(start, "Content stream operator grammar")
		{
			//start %=
			//	// General graphics state
			//	qi::lit("w")[qi::_val = phoenix::construct<LineWidthOperatorPtr>()]
			//	| qi::lit("J")[qi::_val = phoenix::construct<LineCapOperatorPtr>()]
			//	| qi::lit("j")[qi::_val = phoenix::construct<LineJoinOperatorPtr>()]
			//	| qi::lit("M")[qi::_val = phoenix::construct<MiterLimitOperatorPtr>()]
			//	| qi::lit("d")[qi::_val = phoenix::construct<DashPatternOperatorPtr>()]
			//	| qi::lit("ri")[qi::_val = phoenix::construct<ColourRenderingIntentOperatorPtr>()]
			//	| qi::lit("i")[qi::_val = phoenix::construct<FlatnessOperatorPtr>()]
			//	| qi::lit("gs")[qi::_val = phoenix::construct<GraphicsStateOperatorPtr>()]

			//	// Special graphics state
			//	| qi::lit("q")[qi::_val = phoenix::construct<SaveGraphicsStateOperatorPtr>()]
			//	| qi::lit("Q")[qi::_val = phoenix::construct<RestoreGraphicsStateOperatorPtr>()]
			//	| qi::lit("cm")[qi::_val = phoenix::construct<TransformationMatrixOperatorPtr>()]

			//	// Path construction
			//	| qi::lit("m")[qi::_val = phoenix::construct<BeginSubpathOperatorPtr>()]
			//	| qi::lit("l")[qi::_val = phoenix::construct<LineOperatorPtr>()]
			//	| qi::lit("c")[qi::_val = phoenix::construct<FullCurveOperatorPtr>()]
			//	| qi::lit("v")[qi::_val = phoenix::construct<FinalCurveOperatorPtr>()]
			//	| qi::lit("y")[qi::_val = phoenix::construct<InitialCurveOperatorPtr>()]
			//	| qi::lit("h")[qi::_val = phoenix::construct<CloseSubpathOperatorPtr>()]
			//	| qi::lit("re")[qi::_val = phoenix::construct<RectangleOperatorPtr>()]

			//	// Path painting
			//	| qi::lit("S")[qi::_val = phoenix::construct<StrokeOperatorPtr>()]
			//	| qi::lit("s")[qi::_val = phoenix::construct<CloseAndStrokeOperatorPtr>()]
			//	| qi::lit("f")[qi::_val = phoenix::construct<FillPathNonzeroOperatorPtr>()]
			//	| qi::lit("F")[qi::_val = phoenix::construct<FillPathCompatibilityOperatorPtr>()]
			//	| qi::lit("f*")[qi::_val = phoenix::construct<FillPathEvenOddOperatorPtr>()]
			//	| qi::lit("B")[qi::_val = phoenix::construct<FillStrokeNonzeroOperatorPtr>()]
			//	| qi::lit("B*")[qi::_val = phoenix::construct<FillStrokeEvenOddOperatorPtr>()]
			//	| qi::lit("b")[qi::_val = phoenix::construct<CloseFillStrokeNonzeroOperatorPtr>()]
			//	| qi::lit("b*")[qi::_val = phoenix::construct<CloseFillStrokeEvenOddOperatorPtr>()]
			//	| qi::lit("n")[qi::_val = phoenix::construct<EndPathOperatorPtr>()]

			//	// Clipping paths
			//	| qi::lit("W")[qi::_val = phoenix::construct<ClipPathNonzeroOperatorPtr>()]
			//	| qi::lit("W*")[qi::_val = phoenix::construct<ClipPathEvenOddOperatorPtr>()]

			//	// Text objects
			//	| qi::lit("BT")[qi::_val = phoenix::construct<BeginTextOperatorPtr>()]
			//	| qi::lit("ET")[qi::_val = phoenix::construct<EndTextOperatorPtr>()]

			//	// Text state
			//	| qi::lit("Tc")[qi::_val = phoenix::construct<CharacterSpacingOperatorPtr>()]
			//	| qi::lit("Tw")[qi::_val = phoenix::construct<WordSpacingOperatorPtr>()]
			//	| qi::lit("Tz")[qi::_val = phoenix::construct<HorizontalScalingOperatorPtr>()]
			//	| qi::lit("TL")[qi::_val = phoenix::construct<LeadingOperatorPtr>()]
			//	| qi::lit("Tf")[qi::_val = phoenix::construct<TextFontOperatorPtr>()]
			//	| qi::lit("Tr")[qi::_val = phoenix::construct<TextRenderingModeOperatorPtr>()]
			//	| qi::lit("Ts")[qi::_val = phoenix::construct<TextRiseOperatorPtr>()]

			//	// Text positioning
			//	| qi::lit("Td")[qi::_val = phoenix::construct<TextTranslateOperatorPtr>()]
			//	| qi::lit("TD")[qi::_val = phoenix::construct<TextTranslateLeadingOperatorPtr>()]
			//	| qi::lit("Tm")[qi::_val = phoenix::construct<TextMatrixOperatorPtr>()]
			//	| qi::lit("T*")[qi::_val = phoenix::construct<TextNextLineOperatorPtr>()]

			//	// Text showing
			//	| qi::lit("Tj")[qi::_val = phoenix::construct<TextShowOperatorPtr>()]
			//	| qi::lit("TJ")[qi::_val = phoenix::construct<TextShowArrayOperatorPtr>()]
			//	| qi::lit("'")[qi::_val = phoenix::construct<TextNextLineShowOperatorPtr>()]
			//	| qi::lit("\"")[qi::_val = phoenix::construct<TextNextLineShowSpacingOperatorPtr>()]

			//	// Type 3 fonts
			//	| qi::lit("d0")[qi::_val = phoenix::construct<SetCharWidthOperatorPtr>()]
			//	| qi::lit("d1")[qi::_val = phoenix::construct<SetCacheDeviceOperatorPtr>()]

			//	// Color
			//	// CS, cs, SC, SCN, sc, scn, G, g, RG, rg, K, k

			//	// Shading patterns
			//	| qi::lit("sh")[qi::_val = phoenix::construct<ShadingPaintOperatorPtr>()]

			//	// Inline images
			//	| qi::lit("BI")[qi::_val = phoenix::construct<BeginInlineImageObjectOperatorPtr>()]
			//	| qi::lit("ID")[qi::_val = phoenix::construct<BeginInlineImageDataOperatorPtr>()]
			//	| qi::lit("EI")[qi::_val = phoenix::construct<EndInlineImageObjectOperatorPtr>()]

			//	// XObjects
			//	| qi::lit("Do")[qi::_val = phoenix::construct<InvokeXObjectOperatorPtr>()]

			//	// Marked content
			//	| qi::lit("MP")[qi::_val = phoenix::construct<DefineMarkedContentPointOperatorPtr>()]
			//	| qi::lit("DP")[qi::_val = phoenix::construct<DefineMarkedContentPointWithPropertyListOperatorPtr>()]
			//	| qi::lit("BMC")[qi::_val = phoenix::construct<BeginMarkedContentSequenceOperatorPtr>()]
			//	| qi::lit("BDC")[qi::_val = phoenix::construct<BeginMarkedContentSequenceWithPropertyListOperatorPtr>()]
			//	| qi::lit("EMC")[qi::_val = phoenix::construct<EndMarkedContentSequenceOperatorPtr>()]

			//	// Compatibility
			//	// TODO: enter compatibility section - suppress errors from unknown operators
			//	| qi::lit("BX")[qi::_val = phoenix::construct<BeginCompatibilitySectionOperatorPtr>()]
			//	| qi::lit("EX")[qi::_val = phoenix::construct<EndCompatibilitySectionOperatorPtr>()];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContentStreamGrammar::ContentStreamGrammar() :
			base_type(start, "Content stream grammar")
		{
			start %=
				*(operation(qi::_r1) >> eol);

			operation %=
				*(_operand(qi::_r1) >> whitespace)
				> _operator;

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(operation);
		}
	}
}
