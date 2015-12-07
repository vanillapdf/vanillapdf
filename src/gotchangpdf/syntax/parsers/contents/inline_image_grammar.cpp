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

			void convert_BeginInlineImageObject(BeginInlineImageObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_BeginInlineImageData(BeginInlineImageDataOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_EndInlineImageObject(EndInlineImageObjectOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			InlineImageGrammar::InlineImageGrammar() :
				base_type(start, "Inline image grammar")
			{
				start %=
					begin_image[phoenix::bind(&convert_BeginInlineImageObject, qi::_1, qi::_val)]
					| begin_data[phoenix::bind(&convert_BeginInlineImageData, qi::_1, qi::_val)]
					| end_image[phoenix::bind(&convert_EndInlineImageObject, qi::_1, qi::_val)];

				begin_image %=
					qi::eps
					>> qi::lit("BI");

				begin_data %=
					qi::eps
					>> qi::lit("ID");

				end_image %=
					qi::eps
					>> qi::lit("EI");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_image);
				BOOST_SPIRIT_DEBUG_NODE(begin_data);
				BOOST_SPIRIT_DEBUG_NODE(end_image);
			}
		}
	}
}
