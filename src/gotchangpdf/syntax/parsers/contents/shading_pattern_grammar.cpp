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

			void convert_ShadingPaint(ShadingPaintOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			ShadingPatternGrammar::ShadingPatternGrammar() :
				base_type(start, "Shading pattern grammar")
			{
				start %=
					shading[phoenix::bind(&convert_ShadingPaint, qi::_1, qi::_val)];

				shading %=
					qi::eps
					>> qi::lit("sh");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(shading);
			}
		}
	}
}
