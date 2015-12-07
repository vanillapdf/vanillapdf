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

			void convert_SaveGraphicsState(SaveGraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_RestoreGraphicsState(RestoreGraphicsStateOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_TransformationMatrix(TransformationMatrixOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			SpecialGraphicsStateGrammar::SpecialGraphicsStateGrammar() :
				base_type(start, "Special graphics state grammar")
			{
				start %=
					save[phoenix::bind(&convert_SaveGraphicsState, qi::_1, qi::_val)]
					| restore[phoenix::bind(&convert_RestoreGraphicsState, qi::_1, qi::_val)]
					| matrix[phoenix::bind(&convert_TransformationMatrix, qi::_1, qi::_val)];

				save %=
					qi::eps
					>> qi::lit("q");

				restore %=
					qi::eps
					>> qi::lit("Q");

				matrix %=
					qi::eps
					>> qi::lit("cm");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(save);
				BOOST_SPIRIT_DEBUG_NODE(restore);
				BOOST_SPIRIT_DEBUG_NODE(matrix);
			}
		}
	}
}
