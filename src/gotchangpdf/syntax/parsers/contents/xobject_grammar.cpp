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

			void convert_InvokeXObject(InvokeXObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			XObjectGrammar::XObjectGrammar() :
				base_type(start, "XObject grammar")
			{
				start %=
					invoke[phoenix::bind(&convert_InvokeXObject, qi::_1, qi::_val)];

				invoke %=
					qi::eps
					>> qi::lit("Do");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(invoke);
			}
		}
	}
}
