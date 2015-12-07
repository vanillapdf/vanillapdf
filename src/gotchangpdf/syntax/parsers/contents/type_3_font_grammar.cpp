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

			void convert_SetCharWidth(SetCharWidthOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }
			void convert_SetCacheDevice(SetCacheDeviceOperatorPtr& obj, OperatorBasePtr& result) { return convert(obj, result); }

			Type3FontGrammar::Type3FontGrammar() :
				base_type(start, "Type 3 font grammar")
			{
				start %=
					char_width[phoenix::bind(&convert_SetCharWidth, qi::_1, qi::_val)]
					| cache_device[phoenix::bind(&convert_SetCacheDevice, qi::_1, qi::_val)];

				char_width %=
					qi::eps
					>> qi::lit("d0");

				cache_device %=
					qi::eps
					>> qi::lit("d1");

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(char_width);
				BOOST_SPIRIT_DEBUG_NODE(cache_device);
			}
		}
	}
}
