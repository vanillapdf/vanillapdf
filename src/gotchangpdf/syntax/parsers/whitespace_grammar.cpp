#include "precompiled.h"
#include "whitespace_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		EndOfLine::EndOfLine() :
			base_type(start, "EOL grammar")
		{
			start %= (-qi::lit('\r') >> qi::lit('\n')) | (qi::lit('\r') >> -qi::lit('\n'));
			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		SingleWhitespace::SingleWhitespace() :
			base_type(start, "Single whitespace grammar")
		{
			start %= qi::omit[qi::char_(" \r\n\f\t") | qi::char_('\0')];
			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		Whitespace::Whitespace() :
			base_type(start, "Whitespace grammar")
		{
			start %= *single_whitespace;
			BOOST_SPIRIT_DEBUG_NODE(start);
		}
	}
}
