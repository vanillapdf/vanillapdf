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

			void convert_BeginSubpath(BeginSubpathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_Line(LineOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FullCurve(FullCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FinalCurve(FinalCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_InitialCurve(InitialCurveOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_CloseSubpath(CloseSubpathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_Rectangle(RectangleOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			PathConstructionGrammar::PathConstructionGrammar() :
				base_type(start, "Path construction grammar")
			{
				start %=
					begin[phoenix::bind(&convert_BeginSubpath, qi::_1, qi::_val)]
					| line[phoenix::bind(&convert_Line, qi::_1, qi::_val)]
					| full_curve[phoenix::bind(&convert_FullCurve, qi::_1, qi::_val)]
					| final_curve[phoenix::bind(&convert_FinalCurve, qi::_1, qi::_val)]
					| initial_curve[phoenix::bind(&convert_InitialCurve, qi::_1, qi::_val)]
					| close[phoenix::bind(&convert_CloseSubpath, qi::_1, qi::_val)]
					| rectangle[phoenix::bind(&convert_Rectangle, qi::_1, qi::_val)];

				begin %=
					qi::eps
					>> qi::lit("m");

				line %=
					qi::eps
					>> qi::lit("l");

				full_curve %=
					qi::eps
					>> qi::lit("c");

				final_curve %=
					qi::eps
					>> qi::lit("v");

				initial_curve %=
					qi::eps
					>> qi::lit("y");

				close %=
					qi::eps
					>> qi::lit("h");

				rectangle %=
					qi::eps
					>> qi::lit("re");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin);
				BOOST_SPIRIT_DEBUG_NODE(line);
				BOOST_SPIRIT_DEBUG_NODE(full_curve);
				BOOST_SPIRIT_DEBUG_NODE(final_curve);
				BOOST_SPIRIT_DEBUG_NODE(initial_curve);
				BOOST_SPIRIT_DEBUG_NODE(close);
				BOOST_SPIRIT_DEBUG_NODE(rectangle);
			}
		}
	}
}
