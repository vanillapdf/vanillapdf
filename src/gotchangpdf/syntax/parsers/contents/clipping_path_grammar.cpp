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

			void convert_ClipPathNonzero(ClipPathNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_ClipPathEvenOdd(ClipPathEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			ClippingPathGrammar::ClippingPathGrammar() :
				base_type(start, "Clipping path grammar")
			{
				start %=
					non_zero[phoenix::bind(&convert_ClipPathNonzero, qi::_1, qi::_val)]
					| even_odd[phoenix::bind(&convert_ClipPathEvenOdd, qi::_1, qi::_val)];

				non_zero %=
					qi::eps
					>> qi::lit("W");

				even_odd %=
					qi::eps
					>> qi::lit("W*");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(non_zero);
				BOOST_SPIRIT_DEBUG_NODE(even_odd);
			}
		}
	}
}
