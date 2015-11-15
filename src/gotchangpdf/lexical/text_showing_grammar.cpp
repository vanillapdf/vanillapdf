#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
