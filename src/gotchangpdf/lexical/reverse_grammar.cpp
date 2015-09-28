#include "precompiled.h"
#include "reverse_grammar.h"

#define BOOST_SPIRIT_AUTO(domain_, name, expr)                                  \
	typedef boost::proto::result_of::                                           \
	deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type;                       \
	BOOST_SPIRIT_ASSERT_MATCH(                                                  \
	boost::spirit::domain_::domain, name##_expr_type);                          \
	BOOST_AUTO(name, boost::proto::deep_copy(expr));                            \

namespace gotchangpdf
{
	namespace lexical
	{
		BOOST_SPIRIT_AUTO(qi, eol3, (-qi::lit('\n') >> qi::lit('\r')) | (qi::lit('\n') >> -qi::lit('\r')));
		BOOST_SPIRIT_AUTO(qi, whitespace3, qi::omit[qi::char_(" \r\n\f\t") | qi::char_('\0')]);
		BOOST_SPIRIT_AUTO(qi, whitespaces3, *whitespace3);

		ReverseGrammar::ReverseGrammar() :
			base_type(start, "Reverse grammar")
		{
			start %=
				whitespaces3
				> qi::lit("FOE%%")[qi::_val = 0, qi::_a = 1]
				> eol3
				> qi::omit[+qi::char_("0-9")[qi::_val += qi::_a * (qi::_1 - 48), qi::_a *= 10]]
				> eol3
				> qi::lit("ferxtrats");

			BOOST_SPIRIT_DEBUG_NODE(start);
		}
	}
}
