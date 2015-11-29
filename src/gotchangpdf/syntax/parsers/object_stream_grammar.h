#ifndef _OBJECT_STREAM_GRAMMAR_H
#define _OBJECT_STREAM_GRAMMAR_H

#include "qi_common.h"
#include "whitespace_grammar.h"
#include "object_stream_header.h"

#include <boost/spirit/include/qi_grammar.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace qi = boost::spirit::qi;

		class ObjectStreamGrammar : public qi::grammar < pos_iterator_type,
			ObjectStreamHeaders(types::integer size) >
		{
		public:
			ObjectStreamGrammar();

		private:
			template <typename A, typename... Inherited>
			using Rule = qi::rule < pos_iterator_type, A(Inherited...) >;

			qi::rule<pos_iterator_type, ObjectStreamHeaders(types::integer size)> start;
			Rule<ObjectStreamHeader> header;

			Whitespace whitespaces;
		};
	}
}

#endif /* _OBJECT_STREAM_GRAMMAR_H */
