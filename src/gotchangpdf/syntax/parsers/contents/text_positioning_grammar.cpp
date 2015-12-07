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

			void convert_TextTranslate(TextTranslateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextTranslateLeading(TextTranslateLeadingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextMatrix(TextMatrixOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextNextLine(TextNextLineOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			TextPositioningGrammar::TextPositioningGrammar() :
				base_type(start, "Text positioning grammar")
			{
				start %=
					translate[phoenix::bind(&convert_TextTranslate, qi::_1, qi::_val)]
					| translate_leading[phoenix::bind(&convert_TextTranslateLeading, qi::_1, qi::_val)]
					| matrix[phoenix::bind(&convert_TextMatrix, qi::_1, qi::_val)]
					| next_line[phoenix::bind(&convert_TextNextLine, qi::_1, qi::_val)];

				translate %=
					qi::eps
					>> qi::lit("Td");

				translate_leading %=
					qi::eps
					>> qi::lit("TD");

				matrix %=
					qi::eps
					>> qi::lit("Tm");

				next_line %=
					qi::eps
					>> qi::lit("T*");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(translate);
				BOOST_SPIRIT_DEBUG_NODE(translate_leading);
				BOOST_SPIRIT_DEBUG_NODE(matrix);
				BOOST_SPIRIT_DEBUG_NODE(next_line);
			}
		}
	}
}
