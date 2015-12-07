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

			void convert_ColorSpaceStroke(ColorSpaceStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_ColorSpaceNonstroke(ColorSpaceNonstrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetColorStroke(SetColorStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetColorStrokeExtended(SetColorStrokeExtendedOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetColorNonstroke(SetColorNonstrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetColorNonstrokeExtended(SetColorNonstrokeExtendedOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetStrokingColorSpaceGray(SetStrokingColorSpaceGrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetNonstrokingColorSpaceGray(SetNonstrokingColorSpaceGrayOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetStrokingColorSpaceRGB(SetStrokingColorSpaceRGBOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetNonstrokingColorSpaceRGB(SetNonstrokingColorSpaceRGBOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetStrokingColorSpaceCMYK(SetStrokingColorSpaceCMYKOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetNonstrokingColorSpaceCMYK(SetNonstrokingColorSpaceCMYKOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			ColorGrammar::ColorGrammar() :
				base_type(start, "Color grammar")
			{
				start %=
					color_space_stroke[phoenix::bind(&convert_ColorSpaceStroke, qi::_1, qi::_val)]
					| color_space_nstroke[phoenix::bind(&convert_ColorSpaceNonstroke, qi::_1, qi::_val)]
					| sc_stroke[phoenix::bind(&convert_SetColorStroke, qi::_1, qi::_val)]
					| sc_extended_stroke[phoenix::bind(&convert_SetColorStrokeExtended, qi::_1, qi::_val)]
					| sc_nstroke[phoenix::bind(&convert_SetColorNonstroke, qi::_1, qi::_val)]
					| sc_extended_nstroke[phoenix::bind(&convert_SetColorNonstrokeExtended, qi::_1, qi::_val)]
					| sgray[phoenix::bind(&convert_SetStrokingColorSpaceGray, qi::_1, qi::_val)]
					| nsgray[phoenix::bind(&convert_SetNonstrokingColorSpaceGray, qi::_1, qi::_val)]
					| srgb[phoenix::bind(&convert_SetStrokingColorSpaceRGB, qi::_1, qi::_val)]
					| nsrgb[phoenix::bind(&convert_SetNonstrokingColorSpaceRGB, qi::_1, qi::_val)]
					| scmyk[phoenix::bind(&convert_SetStrokingColorSpaceCMYK, qi::_1, qi::_val)]
					| nscmyk[phoenix::bind(&convert_SetNonstrokingColorSpaceCMYK, qi::_1, qi::_val)];

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
}
