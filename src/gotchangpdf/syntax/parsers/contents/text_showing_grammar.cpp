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

			void convert_TextShow(TextShowOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextShowArray(TextShowArrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextNextLineShow(TextNextLineShowOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextNextLineShowSpacing(TextNextLineShowSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			TextShowingGrammar::TextShowingGrammar() :
				base_type(start, "Text showing grammar")
			{
				start %=
					show[phoenix::bind(&convert_TextShow, qi::_1, qi::_val)]
					| show_array[phoenix::bind(&convert_TextShowArray, qi::_1, qi::_val)]
					| next_line_show[phoenix::bind(&convert_TextNextLineShow, qi::_1, qi::_val)]
					| next_line_show_spacing[phoenix::bind(&convert_TextNextLineShowSpacing, qi::_1, qi::_val)];

				show %=
					qi::eps
					>> qi::lit("Tj");

				show_array %=
					qi::eps
					>> qi::lit("TJ");

				next_line_show %=
					qi::eps
					>> qi::lit("'");

				next_line_show_spacing %=
					qi::eps
					>> qi::lit("\"");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(show);
				BOOST_SPIRIT_DEBUG_NODE(show_array);
				BOOST_SPIRIT_DEBUG_NODE(next_line_show);
				BOOST_SPIRIT_DEBUG_NODE(next_line_show_spacing);
			}
		}
	}
}
