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

			void convert_DefineMarkedContentPoint(DefineMarkedContentPointOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_DefineMarkedContentPointWithPropertyList(DefineMarkedContentPointWithPropertyListOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_BeginMarkedContentSequence(BeginMarkedContentSequenceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_BeginMarkedContentSequenceWithPropertyList(BeginMarkedContentSequenceWithPropertyListOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_EndMarkedContentSequence(EndMarkedContentSequenceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			MarkedContentGrammar::MarkedContentGrammar() :
				base_type(start, "Marked content grammar")
			{
				start %=
					define[phoenix::bind(&convert_DefineMarkedContentPoint, qi::_1, qi::_val)]
					| define_prop[phoenix::bind(&convert_DefineMarkedContentPointWithPropertyList, qi::_1, qi::_val)]
					| begin[phoenix::bind(&convert_BeginMarkedContentSequence, qi::_1, qi::_val)]
					| begin_prop[phoenix::bind(&convert_BeginMarkedContentSequenceWithPropertyList, qi::_1, qi::_val)]
					| end[phoenix::bind(&convert_EndMarkedContentSequence, qi::_1, qi::_val)];

				define %=
					qi::eps
					>> qi::lit("MP");

				define_prop %=
					qi::eps
					>> qi::lit("DP");

				begin %=
					qi::eps
					>> qi::lit("BMC");

				begin_prop %=
					qi::eps
					>> qi::lit("BDC");

				end %=
					qi::eps
					>> qi::lit("EMC");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(define);
				BOOST_SPIRIT_DEBUG_NODE(define_prop);
				BOOST_SPIRIT_DEBUG_NODE(begin);
				BOOST_SPIRIT_DEBUG_NODE(begin_prop);
				BOOST_SPIRIT_DEBUG_NODE(end);
			}
		}
	}
}
