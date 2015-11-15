#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
