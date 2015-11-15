#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
