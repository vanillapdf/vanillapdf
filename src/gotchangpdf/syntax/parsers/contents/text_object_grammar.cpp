#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			namespace phoenix = boost::phoenix;

			void convert_BeginText(BeginTextOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_EndText(EndTextOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			TextObjectGrammar::TextObjectGrammar() :
				base_type(start, "Text object grammar")
			{
				start %=
					begin_text[phoenix::bind(&convert_BeginText, qi::_1, qi::_val)]
					| end_text[phoenix::bind(&convert_EndText, qi::_1, qi::_val)];

				begin_text %=
					qi::eps
					>> qi::lit("BT");

				end_text %=
					qi::eps
					>> qi::lit("ET");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_text);
				BOOST_SPIRIT_DEBUG_NODE(end_text);
			}
		}
	}
}
