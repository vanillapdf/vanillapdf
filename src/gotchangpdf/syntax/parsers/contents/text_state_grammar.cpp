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

			void convert_CharacterSpacing(CharacterSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_WordSpacing(WordSpacingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_HorizontalScaling(HorizontalScalingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_Leading(LeadingOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextFont(TextFontOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextRenderingMode(TextRenderingModeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TextRise(TextRiseOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			TextStateGrammar::TextStateGrammar() :
				base_type(start, "Text state grammar")
			{
				start %=
					char_spacing[phoenix::bind(&convert_CharacterSpacing, qi::_1, qi::_val)]
					| word_spacing[phoenix::bind(&convert_WordSpacing, qi::_1, qi::_val)]
					| hscaling[phoenix::bind(&convert_HorizontalScaling, qi::_1, qi::_val)]
					| leading[phoenix::bind(&convert_Leading, qi::_1, qi::_val)]
					| font[phoenix::bind(&convert_TextFont, qi::_1, qi::_val)]
					| rendering_mode[phoenix::bind(&convert_TextRenderingMode, qi::_1, qi::_val)]
					| rise[phoenix::bind(&convert_TextRise, qi::_1, qi::_val)];

				char_spacing %=
					qi::eps
					>> qi::lit("Tc");

				word_spacing %=
					qi::eps
					>> qi::lit("Tw");

				hscaling %=
					qi::eps
					>> qi::lit("Tz");

				leading %=
					qi::eps
					>> qi::lit("TL");

				font %=
					qi::eps
					>> qi::lit("Tf");

				rendering_mode %=
					qi::eps
					>> qi::lit("Tr");

				rise %=
					qi::eps
					>> qi::lit("Ts");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(char_spacing);
				BOOST_SPIRIT_DEBUG_NODE(word_spacing);
				BOOST_SPIRIT_DEBUG_NODE(hscaling);
				BOOST_SPIRIT_DEBUG_NODE(leading);
				BOOST_SPIRIT_DEBUG_NODE(font);
				BOOST_SPIRIT_DEBUG_NODE(rendering_mode);
				BOOST_SPIRIT_DEBUG_NODE(rise);
			}
		}
	}
}
