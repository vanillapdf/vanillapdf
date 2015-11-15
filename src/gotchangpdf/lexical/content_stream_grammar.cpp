#include "precompiled.h"
#include "content_stream_grammar.h"
//#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ContentStreamOperandGrammar::ContentStreamOperandGrammar() :
			base_type(start, "Content stream operand grammar")
		{
			start %=
				array_object(qi::_r1)
				| dictionary_object(qi::_r1)
				| boolean_object
				| function_object
				| real_object
				| integer_object
				| name_object
				| null_object
				| literal_string_object
				| hexadecimal_string_object;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContentStreamGrammar::ContentStreamGrammar() :
			base_type(start, "Content stream grammar")
		{
			start %=
				*(operation(qi::_r1) > whitespaces);

			operation %=
				*(_operand(qi::_r1) > whitespaces)
				>> _operator;

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(operation);
		}
	}
}
