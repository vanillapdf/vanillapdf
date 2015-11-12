#ifndef _CONTENT_STREAM_OPERATOR_GRAMMAR_H
#define _CONTENT_STREAM_OPERATOR_GRAMMAR_H

#include "unknown_interface.h"
#include "deferred.h"
#include "qi_common.h"

#include <boost/spirit/include/qi_grammar.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		class OperatorBase : public IUnknown {};

		// General graphics state
		class LineWidthOperator : public OperatorBase {};				//w
		class LineCapOperator : public OperatorBase {};					//J
		class LineJoinOperator : public OperatorBase {};				//j
		class MiterLimitOperator : public OperatorBase {};				//M
		class DashPatternOperator : public OperatorBase {};				//d
		class ColourRenderingIntentOperator : public OperatorBase {};	//ri
		class FlatnessOperator : public OperatorBase {};				//i
		class GraphicsStateOperator : public OperatorBase {};			//gs

		// Special graphics state
		class SaveGraphicsStateOperator : public OperatorBase {};		//q
		class RestoreGraphicsStateOperator : public OperatorBase {};	//Q
		class TransformationMatrixOperator : public OperatorBase {};	//cm

		// Path construction
		class BeginSubpathOperator : public OperatorBase {};		//m
		class LineOperator : public OperatorBase {};				//l
		class FullCurveOperator : public OperatorBase {};		//c
		class FinalCurveOperator : public OperatorBase {};		//v
		class InitialCurveOperator : public OperatorBase {};		//y
		class CloseSubpathOperator : public OperatorBase {};		//h
		class RectangleOperator : public OperatorBase {};			//re

		// Path painting
		class StrokeOperator : public OperatorBase {};					//S
		class CloseAndStrokeOperator : public OperatorBase {};			//s
		class FillPathNonzeroOperator : public OperatorBase {};			//f
		class FillPathCompatibilityOperator : public OperatorBase {};	//F
		class FillPathEvenOddOperator : public OperatorBase {};			//f*
		class FillStrokeNonzeroOperator : public OperatorBase {};		//B
		class FillStrokeEvenOddOperator : public OperatorBase {};		//B*
		class CloseFillStrokeNonzeroOperator : public OperatorBase {};	//b
		class CloseFillStrokeEvenOddOperator : public OperatorBase {};	//b*
		class EndPathOperator : public OperatorBase {};					//n

		// Clipping paths
		class ClipPathNonzeroOperator : public OperatorBase {};		//W
		class ClipPathEvenOddOperator : public OperatorBase {};		//W*

		// Text objects
		class BeginTextOperator : public OperatorBase {};			//BT
		class EndTextOperator : public OperatorBase {};				//ET

		// Text state
		class CharacterSpacingOperator : public OperatorBase {};	//Tc
		class WordSpacingOperator : public OperatorBase {};			//Tw
		class HorizontalScalingOperator : public OperatorBase {};	//Tz
		class LeadingOperator : public OperatorBase {};				//TL
		class TextFontOperator : public OperatorBase {};			//Tf
		class TextRenderingModeOperator : public OperatorBase {};	//Tr
		class TextRiseOperator : public OperatorBase {};			//Ts

		// Text positioning
		class TextTranslateOperator : public OperatorBase {};				//Td
		class TextTranslateLeadingOperator : public OperatorBase {};	//TD
		class TextMatrixOperator : public OperatorBase {};				//Tm
		class TextNextLineOperator : public OperatorBase {};			//T*

		// Text showing
		class TextShowOperator : public OperatorBase {};					//Tj
		class TextShowArrayOperator : public OperatorBase {};				//TJ
		class TextNextLineShowOperator : public OperatorBase {};			//'
		class TextNextLineShowSpacingOperator : public OperatorBase {};		//"

		// Type 3 fonts
		class SetCharWidthOperator : public OperatorBase {};		//d0
		class SetCacheDeviceOperator : public OperatorBase {};		//d1

		// Color
		// CS, cs, SC, SCN, sc, scn, G, g, RG, rg, K, k

		// Shading patterns
		class ShadingPaintOperator : public OperatorBase {};				//sh

		// Inline images
		class BeginInlineImageObjectOperator : public OperatorBase {};		//BI
		class BeginInlineImageDataOperator : public OperatorBase {};		//ID
		class EndInlineImageObjectOperator : public OperatorBase {};		//EI

		// XObjects
		class InvokeXObjectOperator : public OperatorBase {};								//Do

		// Marked content
		class DefineMarkedContentPointOperator : public OperatorBase {};					//MP
		class DefineMarkedContentPointWithPropertyListOperator : public OperatorBase {};	//DP
		class BeginMarkedContentSequenceOperator : public OperatorBase {};					//BMC
		class BeginMarkedContentSequenceWithPropertyListOperator : public OperatorBase {};	//BDC
		class EndMarkedContentSequenceOperator : public OperatorBase {};					//EMC

		// Compatibility
		class BeginCompatibilitySectionOperator : public OperatorBase {};					//BX
		class EndCompatibilitySectionOperator : public OperatorBase {};						//EX

		typedef Deferred<LineWidthOperator> LineWidthOperatorPtr;
		typedef Deferred<LineCapOperator> LineCapOperatorPtr;
		typedef Deferred<LineJoinOperator> LineJoinOperatorPtr;
		typedef Deferred<MiterLimitOperator> MiterLimitOperatorPtr;
		typedef Deferred<DashPatternOperator> DashPatternOperatorPtr;
		typedef Deferred<ColourRenderingIntentOperator> ColourRenderingIntentOperatorPtr;
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

		typedef boost::variant<
			LineWidthOperatorPtr,
			LineCapOperatorPtr,
			LineJoinOperatorPtr,
			MiterLimitOperatorPtr,
			DashPatternOperatorPtr,
			ColourRenderingIntentOperatorPtr,
			FlatnessOperatorPtr,
			GraphicsStateOperatorPtr,
			SaveGraphicsStateOperatorPtr,
			RestoreGraphicsStateOperatorPtr,
			TransformationMatrixOperatorPtr,
			BeginSubpathOperatorPtr,
			LineOperatorPtr,
			FullCurveOperatorPtr,
			FinalCurveOperatorPtr,
			InitialCurveOperatorPtr,
			CloseSubpathOperatorPtr,
			RectangleOperatorPtr,
			StrokeOperatorPtr,
			CloseAndStrokeOperatorPtr,
			FillPathNonzeroOperatorPtr,
			FillPathCompatibilityOperatorPtr,
			FillPathEvenOddOperatorPtr,
			FillStrokeNonzeroOperatorPtr,
			FillStrokeEvenOddOperatorPtr,
			CloseFillStrokeNonzeroOperatorPtr,
			CloseFillStrokeEvenOddOperatorPtr,
			EndPathOperatorPtr,
			ClipPathNonzeroOperatorPtr,
			ClipPathEvenOddOperatorPtr,
			BeginTextOperatorPtr,
			EndTextOperatorPtr,
			CharacterSpacingOperatorPtr,
			WordSpacingOperatorPtr,
			HorizontalScalingOperatorPtr,
			LeadingOperatorPtr,
			TextFontOperatorPtr,
			TextRenderingModeOperatorPtr,
			TextRiseOperatorPtr,
			TextTranslateOperatorPtr,
			TextTranslateLeadingOperatorPtr,
			TextMatrixOperatorPtr,
			TextNextLineOperatorPtr,
			TextShowOperatorPtr,
			TextShowArrayOperatorPtr,
			TextNextLineShowOperatorPtr,
			TextNextLineShowSpacingOperatorPtr,
			SetCharWidthOperatorPtr,
			SetCacheDeviceOperatorPtr,
			ShadingPaintOperatorPtr,
			BeginInlineImageObjectOperatorPtr,
			BeginInlineImageDataOperatorPtr,
			EndInlineImageObjectOperatorPtr,
			InvokeXObjectOperatorPtr,
			DefineMarkedContentPointOperatorPtr,
			DefineMarkedContentPointWithPropertyListOperatorPtr,
			BeginMarkedContentSequenceOperatorPtr,
			BeginMarkedContentSequenceWithPropertyListOperatorPtr,
			EndMarkedContentSequenceOperatorPtr,
			BeginCompatibilitySectionOperatorPtr,
			EndCompatibilitySectionOperatorPtr
		> ContentStreamOperator;

		template <typename T>
		class IsContentStreamOperatorVisitor : public boost::static_visitor<bool>
		{
		public:
			inline bool operator()(const T&) const { return true; }

			template <typename U>
			inline bool operator()(const U&) const { return false; }
		};

		class ContentStreamOperatorGrammar : public qi::grammar<pos_iterator_type,
			ContentStreamOperator()>
		{
		public:
			ContentStreamOperatorGrammar();

		private:
			qi::rule<pos_iterator_type, ContentStreamOperator()> start;
		};
	}
}

#endif /* _CONTENT_STREAM_OPERATOR_GRAMMAR_H */
