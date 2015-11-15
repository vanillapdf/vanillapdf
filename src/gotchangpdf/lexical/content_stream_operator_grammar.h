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
		class EmptyOperator : public OperatorBase {};

		// General graphics state
		class LineWidthOperator : public OperatorBase {};				//w
		class LineCapOperator : public OperatorBase {};					//J
		class LineJoinOperator : public OperatorBase {};				//j
		class MiterLimitOperator : public OperatorBase {};				//M
		class DashPatternOperator : public OperatorBase {};				//d
		class ColorRenderingIntentOperator : public OperatorBase {};	//ri
		class FlatnessOperator : public OperatorBase {};				//i
		class GraphicsStateOperator : public OperatorBase {};			//gs

		// Special graphics state
		class SaveGraphicsStateOperator : public OperatorBase {};		//q
		class RestoreGraphicsStateOperator : public OperatorBase {};	//Q
		class TransformationMatrixOperator : public OperatorBase {};	//cm

		// Path construction
		class BeginSubpathOperator : public OperatorBase {};		//m
		class LineOperator : public OperatorBase {};				//l
		class FullCurveOperator : public OperatorBase {};			//c
		class FinalCurveOperator : public OperatorBase {};			//v
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
		class TextTranslateOperator : public OperatorBase {};			//Td
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
		class ColorSpaceStrokeOperator : public OperatorBase {};					//CS
		class ColorSpaceNonstrokeOperator : public OperatorBase {};					//cs
		class SetColorStrokeOperator : public OperatorBase {};						//SC
		class SetColorStrokeExtendedOperator : public OperatorBase {};				//SCN
		class SetColorNonstrokeOperator : public OperatorBase {};					//sc
		class SetColorNonstrokeExtendedOperator : public OperatorBase {};			//scn
		class SetStrokingColorSpaceGrayOperator : public OperatorBase {};			//G
		class SetNonstrokingColorSpaceGrayOperator : public OperatorBase {};		//g
		class SetStrokingColorSpaceRGBOperator : public OperatorBase {};			//RG
		class SetNonstrokingColorSpaceRGBOperator : public OperatorBase {};			//rg
		class SetStrokingColorSpaceCMYKOperator : public OperatorBase {};			//K
		class SetNonstrokingColorSpaceCMYKOperator : public OperatorBase {};		//k

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

		typedef boost::variant<
			EmptyOperatorPtr,
			LineWidthOperatorPtr,
			LineCapOperatorPtr,
			LineJoinOperatorPtr,
			MiterLimitOperatorPtr,
			DashPatternOperatorPtr,
			ColorRenderingIntentOperatorPtr,
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
			ColorSpaceStrokeOperatorPtr,
			ColorSpaceNonstrokeOperatorPtr,
			SetColorStrokeOperatorPtr,
			SetColorStrokeExtendedOperatorPtr,
			SetColorNonstrokeOperatorPtr,
			SetColorNonstrokeExtendedOperatorPtr,
			SetStrokingColorSpaceGrayOperatorPtr,
			SetNonstrokingColorSpaceGrayOperatorPtr,
			SetStrokingColorSpaceRGBOperatorPtr,
			SetNonstrokingColorSpaceRGBOperatorPtr,
			SetStrokingColorSpaceCMYKOperatorPtr,
			SetNonstrokingColorSpaceCMYKOperatorPtr,
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

		typedef boost::variant<
			LineWidthOperatorPtr,
			LineCapOperatorPtr,
			LineJoinOperatorPtr,
			MiterLimitOperatorPtr,
			DashPatternOperatorPtr,
			ColorRenderingIntentOperatorPtr,
			FlatnessOperatorPtr,
			GraphicsStateOperatorPtr
		> GeneralGraphicsStateOperators;

		typedef boost::variant<
			SaveGraphicsStateOperatorPtr,
			RestoreGraphicsStateOperatorPtr,
			TransformationMatrixOperatorPtr
		> SpecialGraphicsStateOperators;

		typedef boost::variant<
			BeginSubpathOperatorPtr,
			LineOperatorPtr,
			FullCurveOperatorPtr,
			FinalCurveOperatorPtr,
			InitialCurveOperatorPtr,
			CloseSubpathOperatorPtr,
			RectangleOperatorPtr
		> PathConstructionOperators;

		typedef boost::variant<
			StrokeOperatorPtr,
			CloseAndStrokeOperatorPtr,
			FillPathNonzeroOperatorPtr,
			FillPathCompatibilityOperatorPtr,
			FillPathEvenOddOperatorPtr,
			FillStrokeNonzeroOperatorPtr,
			FillStrokeEvenOddOperatorPtr,
			CloseFillStrokeNonzeroOperatorPtr,
			CloseFillStrokeEvenOddOperatorPtr,
			EndPathOperatorPtr
		> PathPaintingOperators;

		typedef boost::variant<
			ClipPathNonzeroOperatorPtr,
			ClipPathEvenOddOperatorPtr
		> ClippingPathOperators;

		typedef boost::variant<
			BeginTextOperatorPtr,
			EndTextOperatorPtr
		> TextObjectOperators;

		typedef boost::variant<
			CharacterSpacingOperatorPtr,
			WordSpacingOperatorPtr,
			HorizontalScalingOperatorPtr,
			LeadingOperatorPtr,
			TextFontOperatorPtr,
			TextRenderingModeOperatorPtr,
			TextRiseOperatorPtr
		> TextStateOperators;

		typedef boost::variant<
			TextTranslateOperatorPtr,
			TextTranslateLeadingOperatorPtr,
			TextMatrixOperatorPtr,
			TextNextLineOperatorPtr
		> TextPositioningOperators;

		typedef boost::variant<
			TextShowOperatorPtr,
			TextShowArrayOperatorPtr,
			TextNextLineShowOperatorPtr,
			TextNextLineShowSpacingOperatorPtr
		> TextShowingOperators;

		typedef boost::variant<
			SetCharWidthOperatorPtr,
			SetCacheDeviceOperatorPtr
		> Type3FontOperators;

		typedef boost::variant<
			ColorSpaceStrokeOperatorPtr,
			ColorSpaceNonstrokeOperatorPtr,
			SetColorStrokeOperatorPtr,
			SetColorStrokeExtendedOperatorPtr,
			SetColorNonstrokeOperatorPtr,
			SetColorNonstrokeExtendedOperatorPtr,
			SetStrokingColorSpaceGrayOperatorPtr,
			SetNonstrokingColorSpaceGrayOperatorPtr,
			SetStrokingColorSpaceRGBOperatorPtr,
			SetNonstrokingColorSpaceRGBOperatorPtr,
			SetStrokingColorSpaceCMYKOperatorPtr,
			SetNonstrokingColorSpaceCMYKOperatorPtr
		> ColorOperators;

		typedef boost::variant<
			ShadingPaintOperatorPtr
		> ShadingPatternOperators;

		typedef boost::variant<
			BeginInlineImageObjectOperatorPtr,
			BeginInlineImageDataOperatorPtr,
			EndInlineImageObjectOperatorPtr
		> InlineImageOperators;

		typedef boost::variant<
			InvokeXObjectOperatorPtr
		> XObjectOperators;

		typedef boost::variant<
			DefineMarkedContentPointOperatorPtr,
			DefineMarkedContentPointWithPropertyListOperatorPtr,
			BeginMarkedContentSequenceOperatorPtr,
			BeginMarkedContentSequenceWithPropertyListOperatorPtr,
			EndMarkedContentSequenceOperatorPtr
		> MarkedContentOperators;

		typedef boost::variant<
			BeginCompatibilitySectionOperatorPtr,
			EndCompatibilitySectionOperatorPtr
		> CompatibilityOperators;

		template <typename T>
		class IsContentStreamOperatorVisitor : public boost::static_visitor<bool>
		{
		public:
			inline bool operator()(const T&) const { return true; }

			template <typename U>
			inline bool operator()(const U&) const { return false; }
		};

		class GeneralGraphicsStateGrammar : public qi::grammar<pos_iterator_type,
			GeneralGraphicsStateOperators()>
		{
		public:
			GeneralGraphicsStateGrammar();

		private:
			qi::rule<pos_iterator_type, GeneralGraphicsStateOperators()> start;
			qi::rule<pos_iterator_type, LineWidthOperatorPtr()> line_width;
			qi::rule<pos_iterator_type, LineCapOperatorPtr()> line_cap;
			qi::rule<pos_iterator_type, LineJoinOperatorPtr()> line_join;
			qi::rule<pos_iterator_type, MiterLimitOperatorPtr()> miter_limit;
			qi::rule<pos_iterator_type, DashPatternOperatorPtr()> dash_pattern;
			qi::rule<pos_iterator_type, ColorRenderingIntentOperatorPtr()> color_rendering_intent;
			qi::rule<pos_iterator_type, FlatnessOperatorPtr()> flatness;
			qi::rule<pos_iterator_type, GraphicsStateOperatorPtr()> graphics_state;
		};

		class SpecialGraphicsStateGrammar : public qi::grammar<pos_iterator_type,
			SpecialGraphicsStateOperators()>
		{
		public:
			SpecialGraphicsStateGrammar();

		private:
			qi::rule<pos_iterator_type, SpecialGraphicsStateOperators()> start;
			qi::rule<pos_iterator_type, SaveGraphicsStateOperatorPtr()> save;
			qi::rule<pos_iterator_type, RestoreGraphicsStateOperatorPtr()> restore;
			qi::rule<pos_iterator_type, TransformationMatrixOperatorPtr()> matrix;
		};

		class PathConstructionGrammar : public qi::grammar<pos_iterator_type,
			PathConstructionOperators()>
		{
		public:
			PathConstructionGrammar();

		private:
			qi::rule<pos_iterator_type, PathConstructionOperators()> start;
			qi::rule<pos_iterator_type, BeginSubpathOperatorPtr()> begin;
			qi::rule<pos_iterator_type, LineOperatorPtr()> line;
			qi::rule<pos_iterator_type, FullCurveOperatorPtr()> full_curve;
			qi::rule<pos_iterator_type, FinalCurveOperatorPtr()> final_curve;
			qi::rule<pos_iterator_type, InitialCurveOperatorPtr()> initial_curve;
			qi::rule<pos_iterator_type, CloseSubpathOperatorPtr()> close;
			qi::rule<pos_iterator_type, RectangleOperatorPtr()> rectangle;
		};

		class PathPaintingGrammar : public qi::grammar<pos_iterator_type,
			PathPaintingOperators()>
		{
		public:
			PathPaintingGrammar();

		private:
			qi::rule<pos_iterator_type, PathPaintingOperators()> start;
			qi::rule<pos_iterator_type, StrokeOperatorPtr()> stroke;
			qi::rule<pos_iterator_type, CloseAndStrokeOperatorPtr()> close_stroke;
			qi::rule<pos_iterator_type, FillPathNonzeroOperatorPtr()> fill_nz;
			qi::rule<pos_iterator_type, FillPathCompatibilityOperatorPtr()> fill_comp;
			qi::rule<pos_iterator_type, FillPathEvenOddOperatorPtr()> fill_eo;
			qi::rule<pos_iterator_type, FillStrokeNonzeroOperatorPtr()> fill_stroke_nz;
			qi::rule<pos_iterator_type, FillStrokeEvenOddOperatorPtr()> fill_stroke_eo;
			qi::rule<pos_iterator_type, CloseFillStrokeNonzeroOperatorPtr()> close_fill_stroke_nz;
			qi::rule<pos_iterator_type, CloseFillStrokeEvenOddOperatorPtr()> close_fill_stroke_eo;
			qi::rule<pos_iterator_type, EndPathOperatorPtr()> end_path;
		};

		class ClippingPathGrammar : public qi::grammar<pos_iterator_type,
			ClippingPathOperators()>
		{
		public:
			ClippingPathGrammar();

		private:
			qi::rule<pos_iterator_type, ClippingPathOperators()> start;
			qi::rule<pos_iterator_type, ClipPathNonzeroOperatorPtr()> non_zero;
			qi::rule<pos_iterator_type, ClipPathEvenOddOperatorPtr()> even_odd;
		};

		class TextObjectGrammar : public qi::grammar<pos_iterator_type,
			TextObjectOperators()>
		{
		public:
			TextObjectGrammar();

		private:
			qi::rule<pos_iterator_type, TextObjectOperators()> start;
			qi::rule<pos_iterator_type, BeginTextOperatorPtr()> begin_text;
			qi::rule<pos_iterator_type, EndTextOperatorPtr()> end_text;
		};

		class TextStateGrammar : public qi::grammar<pos_iterator_type,
			TextStateOperators()>
		{
		public:
			TextStateGrammar();

		private:
			qi::rule<pos_iterator_type, TextStateOperators()> start;
			qi::rule<pos_iterator_type, CharacterSpacingOperatorPtr()> char_spacing;
			qi::rule<pos_iterator_type, WordSpacingOperatorPtr()> word_spacing;
			qi::rule<pos_iterator_type, HorizontalScalingOperatorPtr()> hscaling;
			qi::rule<pos_iterator_type, LeadingOperatorPtr()> leading;
			qi::rule<pos_iterator_type, TextFontOperatorPtr()> font;
			qi::rule<pos_iterator_type, TextRenderingModeOperatorPtr()> rendering_mode;
			qi::rule<pos_iterator_type, TextRiseOperatorPtr()> rise;
		};

		class TextPositioningGrammar : public qi::grammar<pos_iterator_type,
			TextPositioningOperators()>
		{
		public:
			TextPositioningGrammar();

		private:
			qi::rule<pos_iterator_type, TextPositioningOperators()> start;
			qi::rule<pos_iterator_type, TextTranslateOperatorPtr()> translate;
			qi::rule<pos_iterator_type, TextTranslateLeadingOperatorPtr()> translate_leading;
			qi::rule<pos_iterator_type, TextMatrixOperatorPtr()> matrix;
			qi::rule<pos_iterator_type, TextNextLineOperatorPtr()> next_line;
		};

		class TextShowingGrammar : public qi::grammar<pos_iterator_type,
			TextShowingOperators()>
		{
		public:
			TextShowingGrammar();

		private:
			qi::rule<pos_iterator_type, TextShowingOperators()> start;
			qi::rule<pos_iterator_type, TextShowOperatorPtr()> show;
			qi::rule<pos_iterator_type, TextShowArrayOperatorPtr()> show_array;
			qi::rule<pos_iterator_type, TextNextLineShowOperatorPtr()> next_line_show;
			qi::rule<pos_iterator_type, TextNextLineShowSpacingOperatorPtr()> next_line_show_spacing;
		};

		class Type3FontGrammar : public qi::grammar<pos_iterator_type,
			Type3FontOperators()>
		{
		public:
			Type3FontGrammar();

		private:
			qi::rule<pos_iterator_type, Type3FontOperators()> start;
			qi::rule<pos_iterator_type, SetCharWidthOperatorPtr()> char_width;
			qi::rule<pos_iterator_type, SetCacheDeviceOperatorPtr()> cache_device;
		};

		class ColorGrammar : public qi::grammar<pos_iterator_type,
			ColorOperators()>
		{
		public:
			ColorGrammar();

		private:
			qi::rule<pos_iterator_type, ColorOperators()> start;
			qi::rule<pos_iterator_type, ColorSpaceStrokeOperatorPtr()> color_space_stroke;
			qi::rule<pos_iterator_type, ColorSpaceNonstrokeOperatorPtr()> color_space_nstroke;
			qi::rule<pos_iterator_type, SetColorStrokeOperatorPtr()> sc_stroke;
			qi::rule<pos_iterator_type, SetColorStrokeExtendedOperatorPtr()> sc_extended_stroke;
			qi::rule<pos_iterator_type, SetColorNonstrokeOperatorPtr()> sc_nstroke;
			qi::rule<pos_iterator_type, SetColorNonstrokeExtendedOperatorPtr()> sc_extended_nstroke;
			qi::rule<pos_iterator_type, SetStrokingColorSpaceGrayOperatorPtr()> sgray;
			qi::rule<pos_iterator_type, SetNonstrokingColorSpaceGrayOperatorPtr()> nsgray;
			qi::rule<pos_iterator_type, SetStrokingColorSpaceRGBOperatorPtr()> srgb;
			qi::rule<pos_iterator_type, SetNonstrokingColorSpaceRGBOperatorPtr()> nsrgb;
			qi::rule<pos_iterator_type, SetStrokingColorSpaceCMYKOperatorPtr()> scmyk;
			qi::rule<pos_iterator_type, SetNonstrokingColorSpaceCMYKOperatorPtr()> nscmyk;
		};

		class ShadingPatternGrammar : public qi::grammar<pos_iterator_type,
			ShadingPatternOperators()>
		{
		public:
			ShadingPatternGrammar();

		private:
			qi::rule<pos_iterator_type, ShadingPatternOperators()> start;
			qi::rule<pos_iterator_type, ShadingPaintOperatorPtr()> shading;
		};

		class InlineImageGrammar : public qi::grammar<pos_iterator_type,
			InlineImageOperators()>
		{
		public:
			InlineImageGrammar();

		private:
			qi::rule<pos_iterator_type, InlineImageOperators()> start;
			qi::rule<pos_iterator_type, BeginInlineImageObjectOperatorPtr()> begin_image;
			qi::rule<pos_iterator_type, BeginInlineImageDataOperatorPtr()> begin_data;
			qi::rule<pos_iterator_type, EndInlineImageObjectOperatorPtr()> end_image;
		};

		class XObjectGrammar : public qi::grammar<pos_iterator_type,
			XObjectOperators()>
		{
		public:
			XObjectGrammar();

		private:
			qi::rule<pos_iterator_type, XObjectOperators()> start;
			qi::rule<pos_iterator_type, InvokeXObjectOperatorPtr()> invoke;
		};

		class MarkedContentGrammar : public qi::grammar<pos_iterator_type,
			MarkedContentOperators()>
		{
		public:
			MarkedContentGrammar();

		private:
			qi::rule<pos_iterator_type, MarkedContentOperators()> start;
			qi::rule<pos_iterator_type, DefineMarkedContentPointOperatorPtr()> define;
			qi::rule<pos_iterator_type, DefineMarkedContentPointWithPropertyListOperatorPtr()> define_prop;
			qi::rule<pos_iterator_type, BeginMarkedContentSequenceOperatorPtr()> begin;
			qi::rule<pos_iterator_type, BeginMarkedContentSequenceWithPropertyListOperatorPtr()> begin_prop;
			qi::rule<pos_iterator_type, EndMarkedContentSequenceOperatorPtr()> end;
		};

		class CompatibilityGrammar : public qi::grammar<pos_iterator_type,
			CompatibilityOperators()>
		{
		public:
			CompatibilityGrammar();

		private:
			qi::rule<pos_iterator_type, CompatibilityOperators()> start;
			qi::rule<pos_iterator_type, BeginCompatibilitySectionOperatorPtr()> begin_compatibility;
			qi::rule<pos_iterator_type, EndCompatibilitySectionOperatorPtr()> end_compatibility;
		};

		class ContentStreamOperatorGrammar : public qi::grammar<pos_iterator_type,
			ContentStreamOperator()>
		{
		public:
			ContentStreamOperatorGrammar();

		private:
			qi::rule<pos_iterator_type, ContentStreamOperator()> start;
			GeneralGraphicsStateGrammar general_graphics_state;
			SpecialGraphicsStateGrammar special_graphics_state;
			PathConstructionGrammar path_construction;
			PathPaintingGrammar path_painting;
			ClippingPathGrammar clipping_path;
			TextObjectGrammar text_object;
			TextStateGrammar text_state;
			TextPositioningGrammar text_positioning;
			TextShowingGrammar text_showing;
			Type3FontGrammar type_3_font;
			ColorGrammar color;
			ShadingPatternGrammar shading_pattern;
			InlineImageGrammar inline_image;
			XObjectGrammar xobject;
			MarkedContentGrammar marked_content;
			CompatibilityGrammar compatibility;
		};
	}
}

#endif /* _CONTENT_STREAM_OPERATOR_GRAMMAR_H */
