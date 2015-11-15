#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
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
		}
	}
}
