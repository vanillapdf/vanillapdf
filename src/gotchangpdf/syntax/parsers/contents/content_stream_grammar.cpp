#include "precompiled.h"
#include "content_stream_grammar.h"
#include "object_visitors.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

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
					| boolean_object(qi::_r1)
					| function_object(qi::_r1)
					| real_object(qi::_r1)
					| integer_object(qi::_r1)
					| name_object(qi::_r1)
					| null_object(qi::_r1)
					| literal_string_object(qi::_r1)
					| hexadecimal_string_object(qi::_r1);

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
						)[qi::_val = phoenix::construct<OperationGenericPtr>(qi::_1, qi::_2)]
					];

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(operation);
			}
		}
	}
}
