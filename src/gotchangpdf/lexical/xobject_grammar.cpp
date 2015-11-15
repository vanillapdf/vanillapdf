#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
