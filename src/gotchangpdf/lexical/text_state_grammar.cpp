#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		TextStateGrammar::TextStateGrammar() :
			base_type(start, "Text state grammar")
		{
			start %=
				char_spacing
				| word_spacing
				| hscaling
				| leading
				| font
				| rendering_mode
				| rise;

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
