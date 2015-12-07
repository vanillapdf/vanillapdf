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

			void convert_LineWidth(LineWidthOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_LineCap(LineCapOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_LineJoin(LineJoinOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_MiterLimit(MiterLimitOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_DashPattern(DashPatternOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_ColorRenderingIntent(ColorRenderingIntentOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_Flatness(FlatnessOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_GraphicsState(GraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			GeneralGraphicsStateGrammar::GeneralGraphicsStateGrammar() :
				base_type(start, "General graphics state grammar")
			{
				start %=
					line_width[phoenix::bind(&convert_LineWidth, qi::_1, qi::_val)]
					| line_cap[phoenix::bind(&convert_LineCap, qi::_1, qi::_val)]
					| line_join[phoenix::bind(&convert_LineJoin, qi::_1, qi::_val)]
					| miter_limit[phoenix::bind(&convert_MiterLimit, qi::_1, qi::_val)]
					| dash_pattern[phoenix::bind(&convert_DashPattern, qi::_1, qi::_val)]
					| color_rendering_intent[phoenix::bind(&convert_ColorRenderingIntent, qi::_1, qi::_val)]
					| flatness[phoenix::bind(&convert_Flatness, qi::_1, qi::_val)]
					| graphics_state[phoenix::bind(&convert_GraphicsState, qi::_1, qi::_val)];

				line_width %=
					qi::eps
					>> qi::lit("w");

				line_cap %=
					qi::eps
					>> qi::lit("J");

				line_join %=
					qi::eps
					>> qi::lit("j");

				miter_limit %=
					qi::eps
					>> qi::lit("M");

				dash_pattern %=
					qi::eps
					>> qi::lit("d");

				color_rendering_intent %=
					qi::eps
					>> qi::lit("ri");

				flatness %=
					qi::eps
					>> qi::lit("i");

				graphics_state %=
					qi::eps
					>> qi::lit("gs");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(line_width);
				BOOST_SPIRIT_DEBUG_NODE(line_cap);
				BOOST_SPIRIT_DEBUG_NODE(line_join);
				BOOST_SPIRIT_DEBUG_NODE(miter_limit);
				BOOST_SPIRIT_DEBUG_NODE(dash_pattern);
				BOOST_SPIRIT_DEBUG_NODE(color_rendering_intent);
				BOOST_SPIRIT_DEBUG_NODE(flatness);
				BOOST_SPIRIT_DEBUG_NODE(graphics_state);
			}
		}
	}
}
