#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		MarkedContentGrammar::MarkedContentGrammar() :
			base_type(start, "Marked content grammar")
		{
			start %=
				define
				| define_prop
				| begin
				| begin_prop
				| end;

			define %=
				qi::eps
				>> qi::lit("MP");

			define_prop %=
				qi::eps
				>> qi::lit("DP");

			begin %=
				qi::eps
				>> qi::lit("BMC");

			begin_prop %=
				qi::eps
				>> qi::lit("BDC");

			end %=
				qi::eps
				>> qi::lit("EMC");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(define);
			BOOST_SPIRIT_DEBUG_NODE(define_prop);
			BOOST_SPIRIT_DEBUG_NODE(begin);
			BOOST_SPIRIT_DEBUG_NODE(begin_prop);
			BOOST_SPIRIT_DEBUG_NODE(end);
		}
	}
}
