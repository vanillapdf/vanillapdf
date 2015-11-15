#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		SpecialGraphicsStateGrammar::SpecialGraphicsStateGrammar() :
			base_type(start, "Special graphics state grammar")
		{
			start %=
				save
				| restore
				| matrix;

			save %=
				qi::eps
				>> qi::lit("q");

			restore %=
				qi::eps
				>> qi::lit("Q");

			matrix %=
				qi::eps
				>> qi::lit("cm");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(save);
			BOOST_SPIRIT_DEBUG_NODE(restore);
			BOOST_SPIRIT_DEBUG_NODE(matrix);
		}
	}
}
