#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
