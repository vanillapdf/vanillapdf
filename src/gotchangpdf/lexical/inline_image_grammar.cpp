#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		InlineImageGrammar::InlineImageGrammar() :
			base_type(start, "Inline image grammar")
		{
			start %=
				begin_image
				| begin_data
				| end_image;

			begin_image %=
				qi::eps
				>> qi::lit("BI");

			begin_data %=
				qi::eps
				>> qi::lit("ID");

			end_image %=
				qi::eps
				>> qi::lit("EI");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(begin_image);
			BOOST_SPIRIT_DEBUG_NODE(begin_data);
			BOOST_SPIRIT_DEBUG_NODE(end_image);
		}
	}
}
