#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
