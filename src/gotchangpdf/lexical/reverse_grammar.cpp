#include "precompiled.h"
#include "reverse_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		ReverseGrammar::ReverseGrammar() :
			base_type(start, "Reverse grammar")
		{
			start %=
				whitespaces
				> qi::lit("FOE%%")[qi::_val = 0, qi::_a = 1]
				> reverse_eol
				> qi::omit[+qi::char_("0-9")[qi::_val += qi::_a * (qi::_1 - 48), qi::_a *= 10]]
				> reverse_eol
				> qi::lit("ferxtrats");

			reverse_eol %= (-qi::lit('\n') >> qi::lit('\r')) | (qi::lit('\n') >> -qi::lit('\r'));

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(reverse_eol);
		}
	}
}
