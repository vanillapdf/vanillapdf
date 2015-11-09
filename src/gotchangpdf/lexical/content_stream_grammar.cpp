#include "precompiled.h"
#include "content_stream_grammar.h"
//#include "abstract_syntax_tree.h"

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

		ContentStreamOperatorGrammar::ContentStreamOperatorGrammar() :
			base_type(start, "Content stream operator grammar")
		{
			start %=
				qi::lit("BT")[qi::_val = phoenix::construct<BeginTextOperatorPtr>()]
				| qi::lit("ET")[qi::_val = phoenix::construct<EndTextOperatorPtr>()];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContentStreamGrammar::ContentStreamGrammar() :
			base_type(start, "Content stream grammar")
		{
			start %=
				*(operation(qi::_r1) >> eol);

			operation %=
				*(_operand(qi::_r1) >> whitespace)
				> _operator;

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(operation);
		}
	}
}
