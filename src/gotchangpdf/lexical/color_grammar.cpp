#include "precompiled.h"
#include "content_stream_operator_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ColorGrammar::ColorGrammar() :
			base_type(start, "Color grammar")
		{
			start %=
				color_space_stroke
				| color_space_nstroke
				| sc_stroke
				| sc_extended_stroke
				| sc_nstroke
				| sc_extended_nstroke
				| sgray
				| nsgray
				| srgb
				| nsrgb
				| scmyk
				| nscmyk;

			color_space_stroke %=
				qi::eps
				>> qi::lit("CS");

			color_space_nstroke %=
				qi::eps
				>> qi::lit("cs");

			sc_stroke %=
				qi::eps
				>> qi::lit("SC");

			sc_extended_stroke %=
				qi::eps
				>> qi::lit("SCN");

			sc_nstroke %=
				qi::eps
				>> qi::lit("sc");

			sc_extended_nstroke %=
				qi::eps
				>> qi::lit("scn");

			sgray %=
				qi::eps
				>> qi::lit("G");

			nsgray %=
				qi::eps
				>> qi::lit("g");

			srgb %=
				qi::eps
				>> qi::lit("RG");

			nsrgb %=
				qi::eps
				>> qi::lit("rg");

			scmyk %=
				qi::eps
				>> qi::lit("K");

			nscmyk %=
				qi::eps
				>> qi::lit("k");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(color_space_stroke);
			BOOST_SPIRIT_DEBUG_NODE(color_space_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_stroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_extended_stroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sc_extended_nstroke);
			BOOST_SPIRIT_DEBUG_NODE(sgray);
			BOOST_SPIRIT_DEBUG_NODE(nsgray);
			BOOST_SPIRIT_DEBUG_NODE(srgb);
			BOOST_SPIRIT_DEBUG_NODE(nsrgb);
			BOOST_SPIRIT_DEBUG_NODE(scmyk);
			BOOST_SPIRIT_DEBUG_NODE(nscmyk);
		}
	}
}
