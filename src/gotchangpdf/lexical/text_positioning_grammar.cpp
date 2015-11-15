#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
