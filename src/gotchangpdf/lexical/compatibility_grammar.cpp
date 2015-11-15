#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

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
	}
}
