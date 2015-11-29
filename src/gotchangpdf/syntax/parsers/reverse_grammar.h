#ifndef _REVERSE_GRAMMAR_H
#define _REVERSE_GRAMMAR_H

#include "qi_common.h"
#include "whitespace_grammar.h"

#include <boost/spirit/include/qi_grammar.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace qi = boost::spirit::qi;

		class ReverseGrammar : public qi::grammar<pos_iterator_type,
			types::integer(),
			qi::locals<types::integer>>
		{
		public:
			ReverseGrammar();

		private:
			qi::rule<pos_iterator_type, types::integer(), qi::locals<types::integer>> start;
			qi::rule<pos_iterator_type, void()> reverse_eol;

			Whitespace whitespaces;

		};
	}
}

#endif /* _REVERSE_GRAMMAR_H */
