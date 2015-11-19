#ifndef _CONTENT_STREAM_OPERATOR_GRAMMAR_H
#define _CONTENT_STREAM_OPERATOR_GRAMMAR_H

#include "unknown_interface.h"
#include "qi_common.h"
#include "content_stream_operations.h"

#include <boost/spirit/include/qi_grammar.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

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
