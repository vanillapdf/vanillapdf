#include "precompiled.h"
#include "content_stream_grammar.h"
//#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			namespace phoenix = boost::phoenix;

			OperandGrammar::OperandGrammar() :
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
					qi::eps
					>> qi::omit
					[
						(
							*(_operand(qi::_r1) > whitespaces)
							>> _operator
						)[qi::_val = phoenix::construct<OperationPtr>(qi::_1, qi::_2)]
					];

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(operation);
			}
		}
	}
}
