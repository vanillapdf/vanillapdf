#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
