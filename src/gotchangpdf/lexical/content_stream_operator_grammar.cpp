#include "precompiled.h"
#include "content_stream_operator_grammar.h"
//#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace phoenix = boost::phoenix;

		ContentStreamOperatorGrammar::ContentStreamOperatorGrammar() :
			base_type(start, "Content stream operator grammar")
		{
			start %=
				text_object
				| compatibility
				| marked_content
				| inline_image
				| shading_pattern
				| text_positioning
				| text_showing
				| text_state
				| type_3_font
				| xobject
				| general_graphics_state
				| special_graphics_state
				| color
				| clipping_path
				| path_construction
				| path_painting;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}
	}
}
