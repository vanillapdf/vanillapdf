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

			void convert_BeginCompatibilitySection(BeginCompatibilitySectionOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_EndCompatibilitySection(EndCompatibilitySectionOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			CompatibilityGrammar::CompatibilityGrammar() :
				base_type(start, "Compatibility grammar")
			{
				start %=
					// TODO: enter compatibility section - suppress errors from unknown operators
					begin_compatibility[phoenix::bind(&convert_BeginCompatibilitySection, qi::_1, qi::_val)]
					| end_compatibility[phoenix::bind(&convert_EndCompatibilitySection, qi::_1, qi::_val)];

				begin_compatibility %=
					qi::eps
					>> qi::lit("BX");

				end_compatibility %=
					qi::eps
					>> qi::lit("EX");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(begin_compatibility);
				BOOST_SPIRIT_DEBUG_NODE(end_compatibility);
			}
		}
	}
}
