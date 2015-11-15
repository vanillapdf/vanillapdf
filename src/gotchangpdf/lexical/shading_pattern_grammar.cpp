#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ShadingPatternGrammar::ShadingPatternGrammar() :
			base_type(start, "Shading pattern grammar")
		{
			start %=
				shading;

			shading %=
				qi::eps
				>> qi::lit("sh");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(shading);
		}
	}
}
