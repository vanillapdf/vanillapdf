#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ColorGrammar::ColorGrammar() :
			base_type(start, "Color grammar")
		{
			start %=
				color_space_stroke
				| color_space_nstroke
				| sc_stroke
				| sc_extended_stroke
				| sc_nstroke
				| sc_extended_nstroke
				| sgray
				| nsgray
				| srgb
				| nsrgb
				| scmyk
				| nscmyk;

			color_space_stroke %=
				qi::eps
				>> qi::lit("CS");

			color_space_nstroke %=
				qi::eps
				>> qi::lit("cs");

			sc_stroke %=
				qi::eps
				>> qi::lit("SC");

			sc_extended_stroke %=
				qi::eps
				>> qi::lit("SCN");

			sc_nstroke %=
				qi::eps
				>> qi::lit("sc");

			sc_extended_nstroke %=
				qi::eps
				>> qi::lit("scn");

			sgray %=
				qi::eps
				>> qi::lit("G");

			nsgray %=
				qi::eps
				>> qi::lit("g");

			srgb %=
				qi::eps
				>> qi::lit("RG");

			nsrgb %=
				qi::eps
				>> qi::lit("rg");

			scmyk %=
				qi::eps
				>> qi::lit("K");

			nscmyk %=
				qi::eps
				>> qi::lit("k");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(color_space_stroke);
			BOOST_SPIRIT_DEBUG_NODE(color_space_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_stroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_extended_stroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_extended_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sgray);
			BOOST_SPIRIT_DEBUG_NODE(nsgray);
			BOOST_SPIRIT_DEBUG_NODE(srgb);
			BOOST_SPIRIT_DEBUG_NODE(nsrgb);
			BOOST_SPIRIT_DEBUG_NODE(scmyk);
			BOOST_SPIRIT_DEBUG_NODE(nscmyk);
		}

		ClippingPathGrammar::ClippingPathGrammar() :
			base_type(start, "Clipping path grammar")
		{
			start %=
				non_zero
				| even_odd;

			non_zero %=
				qi::eps
				>> qi::lit("W");

			even_odd %=
				qi::eps
				>> qi::lit("W*");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(non_zero);
			BOOST_SPIRIT_DEBUG_NODE(even_odd);
		}

		CompatibilityGrammar::CompatibilityGrammar() :
			base_type(start, "Compatibility grammar")
		{
			start %=
				// TODO: enter compatibility section - suppress errors from unknown operators
				begin_compatibility
				| end_compatibility;

			begin_compatibility %=
				qi::eps
				>> qi::lit("BX");

			end_compatibility %=
				qi::eps
				>> qi::lit("EX");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(begin_compatibility);
			BOOST_SPIRIT_DEBUG_NODE(end_compatibility);
		}

		GeneralGraphicsStateGrammar::GeneralGraphicsStateGrammar() :
			base_type(start, "General graphics state grammar")
		{
			start %=
				line_width
				| line_cap
				| line_join
				| miter_limit
				| dash_pattern
				| color_rendering_intent
				| flatness
				| graphics_state;

			line_width %=
				qi::eps
				>> qi::lit("w");

			line_cap %=
				qi::eps
				>> qi::lit("J");

			line_join %=
				qi::eps
				>> qi::lit("j");

			miter_limit %=
				qi::eps
				>> qi::lit("M");

			dash_pattern %=
				qi::eps
				>> qi::lit("d");

			color_rendering_intent %=
				qi::eps
				>> qi::lit("ri");

			flatness %=
				qi::eps
				>> qi::lit("i");

			graphics_state %=
				qi::eps
				>> qi::lit("gs");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(line_width);
			BOOST_SPIRIT_DEBUG_NODE(line_cap);
			BOOST_SPIRIT_DEBUG_NODE(line_join);
			BOOST_SPIRIT_DEBUG_NODE(miter_limit);
			BOOST_SPIRIT_DEBUG_NODE(dash_pattern);
			BOOST_SPIRIT_DEBUG_NODE(color_rendering_intent);
			BOOST_SPIRIT_DEBUG_NODE(flatness);
			BOOST_SPIRIT_DEBUG_NODE(graphics_state);
		}

		InlineImageGrammar::InlineImageGrammar() :
			base_type(start, "Inline image grammar")
		{
			start %=
				begin_image
				| begin_data
				| end_image;

			begin_image %=
				qi::eps
				>> qi::lit("BI");

			begin_data %=
				qi::eps
				>> qi::lit("ID");

			end_image %=
				qi::eps
				>> qi::lit("EI");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(begin_image);
			BOOST_SPIRIT_DEBUG_NODE(begin_data);
			BOOST_SPIRIT_DEBUG_NODE(end_image);
		}

		MarkedContentGrammar::MarkedContentGrammar() :
			base_type(start, "Marked content grammar")
		{
			start %=
				define
				| define_prop
				| begin
				| begin_prop
				| end;

			define %=
				qi::eps
				>> qi::lit("MP");

			define_prop %=
				qi::eps
				>> qi::lit("DP");

			begin %=
				qi::eps
				>> qi::lit("BMC");

			begin_prop %=
				qi::eps
				>> qi::lit("BDC");

			end %=
				qi::eps
				>> qi::lit("EMC");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(define);
			BOOST_SPIRIT_DEBUG_NODE(define_prop);
			BOOST_SPIRIT_DEBUG_NODE(begin);
			BOOST_SPIRIT_DEBUG_NODE(begin_prop);
			BOOST_SPIRIT_DEBUG_NODE(end);
		}

		PathConstructionGrammar::PathConstructionGrammar() :
			base_type(start, "Path construction grammar")
		{
			start %=
				begin
				| line
				| full_curve
				| final_curve
				| initial_curve
				| close
				| rectangle;

			begin %=
				qi::eps
				>> qi::lit("m");

			line %=
				qi::eps
				>> qi::lit("l");

			full_curve %=
				qi::eps
				>> qi::lit("c");

			final_curve %=
				qi::eps
				>> qi::lit("v");

			initial_curve %=
				qi::eps
				>> qi::lit("y");

			close %=
				qi::eps
				>> qi::lit("h");

			rectangle %=
				qi::eps
				>> qi::lit("re");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(begin);
			BOOST_SPIRIT_DEBUG_NODE(line);
			BOOST_SPIRIT_DEBUG_NODE(full_curve);
			BOOST_SPIRIT_DEBUG_NODE(final_curve);
			BOOST_SPIRIT_DEBUG_NODE(initial_curve);
			BOOST_SPIRIT_DEBUG_NODE(close);
			BOOST_SPIRIT_DEBUG_NODE(rectangle);
		}

		PathPaintingGrammar::PathPaintingGrammar() :
			base_type(start, "Path painting grammar")
		{
			start %=
				stroke
				| close_stroke
				| fill_nz
				| fill_comp
				| fill_eo
				| fill_stroke_nz
				| fill_stroke_eo
				| close_fill_stroke_nz
				| close_fill_stroke_eo
				| end_path;

			stroke %=
				qi::eps
				>> qi::lit("S");

			close_stroke %=
				qi::eps
				>> qi::lit("s");

			fill_nz %=
				qi::eps
				>> qi::lit("f");

			fill_comp %=
				qi::eps
				>> qi::lit("F");

			fill_eo %=
				qi::eps
				>> qi::lit("f*");

			fill_stroke_nz %=
				qi::eps
				>> qi::lit("B");

			fill_stroke_eo %=
				qi::eps
				>> qi::lit("B*");

			close_fill_stroke_nz %=
				qi::eps
				>> qi::lit("b");

			close_fill_stroke_eo %=
				qi::eps
				>> qi::lit("b*");

			end_path %=
				qi::eps
				>> qi::lit("n");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(stroke);
			BOOST_SPIRIT_DEBUG_NODE(close_stroke);
			BOOST_SPIRIT_DEBUG_NODE(fill_nz);
			BOOST_SPIRIT_DEBUG_NODE(fill_comp);
			BOOST_SPIRIT_DEBUG_NODE(fill_eo);
			BOOST_SPIRIT_DEBUG_NODE(fill_stroke_nz);
			BOOST_SPIRIT_DEBUG_NODE(fill_stroke_eo);
			BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_nz);
			BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_eo);
			BOOST_SPIRIT_DEBUG_NODE(end_path);
		}

		ShadingPatternGrammar::ShadingPatternGrammar() :
			base_type(start, "Shading pattern grammar")
		{
			start %=
				shading;

			shading %=
				qi::eps
				>> qi::lit("sh");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(shading);
		}

		SpecialGraphicsStateGrammar::SpecialGraphicsStateGrammar() :
			base_type(start, "Special graphics state grammar")
		{
			start %=
				save
				| restore
				| matrix;

			save %=
				qi::eps
				>> qi::lit("q");

			restore %=
				qi::eps
				>> qi::lit("Q");

			matrix %=
				qi::eps
				>> qi::lit("cm");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(save);
			BOOST_SPIRIT_DEBUG_NODE(restore);
			BOOST_SPIRIT_DEBUG_NODE(matrix);
		}

		TextObjectGrammar::TextObjectGrammar() :
			base_type(start, "Text object grammar")
		{
			start %=
				begin_text
				| end_text;

			begin_text %=
				qi::eps
				>> qi::lit("BT");

			end_text %=
				qi::eps
				>> qi::lit("ET");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(begin_text);
			BOOST_SPIRIT_DEBUG_NODE(end_text);
		}

		TextPositioningGrammar::TextPositioningGrammar() :
			base_type(start, "Text positioning grammar")
		{
			start %=
				translate
				| translate_leading
				| matrix
				| next_line;

			translate %=
				qi::eps
				>> qi::lit("Td");

			translate_leading %=
				qi::eps
				>> qi::lit("TD");

			matrix %=
				qi::eps
				>> qi::lit("Tm");

			next_line %=
				qi::eps
				>> qi::lit("T*");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(translate);
			BOOST_SPIRIT_DEBUG_NODE(translate_leading);
			BOOST_SPIRIT_DEBUG_NODE(matrix);
			BOOST_SPIRIT_DEBUG_NODE(next_line);
		}

		TextShowingGrammar::TextShowingGrammar() :
			base_type(start, "Text showing grammar")
		{
			start %=
				show
				| show_array
				| next_line_show
				| next_line_show_spacing;

			show %=
				qi::eps
				>> qi::lit("Tj");

			show_array %=
				qi::eps
				>> qi::lit("TJ");

			next_line_show %=
				qi::eps
				>> qi::lit("'");

			next_line_show_spacing %=
				qi::eps
				>> qi::lit("\"");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(show);
			BOOST_SPIRIT_DEBUG_NODE(show_array);
			BOOST_SPIRIT_DEBUG_NODE(next_line_show);
			BOOST_SPIRIT_DEBUG_NODE(next_line_show_spacing);
		}

		TextStateGrammar::TextStateGrammar() :
			base_type(start, "Text state grammar")
		{
			start %=
				char_spacing
				| word_spacing
				| hscaling
				| leading
				| font
				| rendering_mode
				| rise;

			char_spacing %=
				qi::eps
				>> qi::lit("Tc");

			word_spacing %=
				qi::eps
				>> qi::lit("Tw");

			hscaling %=
				qi::eps
				>> qi::lit("Tz");

			leading %=
				qi::eps
				>> qi::lit("TL");

			font %=
				qi::eps
				>> qi::lit("Tf");

			rendering_mode %=
				qi::eps
				>> qi::lit("Tr");

			rise %=
				qi::eps
				>> qi::lit("Ts");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(char_spacing);
			BOOST_SPIRIT_DEBUG_NODE(word_spacing);
			BOOST_SPIRIT_DEBUG_NODE(hscaling);
			BOOST_SPIRIT_DEBUG_NODE(leading);
			BOOST_SPIRIT_DEBUG_NODE(font);
			BOOST_SPIRIT_DEBUG_NODE(rendering_mode);
			BOOST_SPIRIT_DEBUG_NODE(rise);
		}

		Type3FontGrammar::Type3FontGrammar() :
			base_type(start, "Type 3 font grammar")
		{
			start %=
				char_width
				| cache_device;

			char_width %=
				qi::eps
				>> qi::lit("d0");

			cache_device %=
				qi::eps
				>> qi::lit("d1");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(char_width);
			BOOST_SPIRIT_DEBUG_NODE(cache_device);
		}

		XObjectGrammar::XObjectGrammar() :
			base_type(start, "XObject grammar")
		{
			start %=
				invoke;

			invoke %=
				qi::eps
				>> qi::lit("Do");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(invoke);
		}
	}
}
