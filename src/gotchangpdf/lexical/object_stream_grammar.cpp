#include "precompiled.h"
#include "object_stream_grammar.h"
#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		ObjectStreamGrammar::ObjectStreamGrammar() :
			base_type(start, "Object stream grammar")
		{
			start %=
				qi::repeat(qi::_r1)[header];

			header %=
				qi::int_
				> whitespaces
				> qi::long_long
				> whitespaces;

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(header);
		}
	}
}
