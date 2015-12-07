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

			void convert_Stroke(StrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_CloseAndStroke(CloseAndStrokeOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FillPathNonzero(FillPathNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FillPathCompatibility(FillPathCompatibilityOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FillPathEvenOdd(FillPathEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FillStrokeNonzero(FillStrokeNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_FillStrokeEvenOdd(FillStrokeEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_CloseFillStrokeNonzero(CloseFillStrokeNonzeroOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_CloseFillStrokeEvenOdd(CloseFillStrokeEvenOddOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_EndPath(EndPathOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			PathPaintingGrammar::PathPaintingGrammar() :
				base_type(start, "Path painting grammar")
			{
				start %=
					stroke[phoenix::bind(&convert_Stroke, qi::_1, qi::_val)]
					| close_stroke[phoenix::bind(&convert_CloseAndStroke, qi::_1, qi::_val)]
					| fill_nz[phoenix::bind(&convert_FillPathNonzero, qi::_1, qi::_val)]
					| fill_comp[phoenix::bind(&convert_FillPathCompatibility, qi::_1, qi::_val)]
					| fill_eo[phoenix::bind(&convert_FillPathEvenOdd, qi::_1, qi::_val)]
					| fill_stroke_nz[phoenix::bind(&convert_FillStrokeNonzero, qi::_1, qi::_val)]
					| fill_stroke_eo[phoenix::bind(&convert_FillStrokeEvenOdd, qi::_1, qi::_val)]
					| close_fill_stroke_nz[phoenix::bind(&convert_CloseFillStrokeNonzero, qi::_1, qi::_val)]
					| close_fill_stroke_eo[phoenix::bind(&convert_CloseFillStrokeEvenOdd, qi::_1, qi::_val)]
					| end_path[phoenix::bind(&convert_EndPath, qi::_1, qi::_val)];

				stroke %=
					qi::eps
					>> qi::lit("S");

				close_stroke %=
					qi::eps
					>> qi::lit("s");

				fill_nz %=
					qi::eps
					>> qi::lit("f");

				fill_comp %=
					qi::eps
					>> qi::lit("F");

				fill_eo %=
					qi::eps
					>> qi::lit("f*");

				fill_stroke_nz %=
					qi::eps
					>> qi::lit("B");

				fill_stroke_eo %=
					qi::eps
					>> qi::lit("B*");

				close_fill_stroke_nz %=
					qi::eps
					>> qi::lit("b");

				close_fill_stroke_eo %=
					qi::eps
					>> qi::lit("b*");

				end_path %=
					qi::eps
					>> qi::lit("n");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(stroke);
				BOOST_SPIRIT_DEBUG_NODE(close_stroke);
				BOOST_SPIRIT_DEBUG_NODE(fill_nz);
				BOOST_SPIRIT_DEBUG_NODE(fill_comp);
				BOOST_SPIRIT_DEBUG_NODE(fill_eo);
				BOOST_SPIRIT_DEBUG_NODE(fill_stroke_nz);
				BOOST_SPIRIT_DEBUG_NODE(fill_stroke_eo);
				BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_nz);
				BOOST_SPIRIT_DEBUG_NODE(close_fill_stroke_eo);
				BOOST_SPIRIT_DEBUG_NODE(end_path);
			}
		}
	}
}
