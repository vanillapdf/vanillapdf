#ifndef _REVERSE_GRAMMAR_H
#define _REVERSE_GRAMMAR_H

#include "file_position.h"
#include "offset_iterator.h"

#pragma warning (push, 3)
#include <boost/spirit/include/qi.hpp>
#pragma warning (pop)

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef offset_iterator<base_iterator_type, pdf_position> pos_iterator_type;

		namespace qi = boost::spirit::qi;

		class ReverseGrammar : public qi::grammar<pos_iterator_type,
			types::integer(),
			qi::locals<types::integer>>
		{
		public:
			ReverseGrammar();

		private:
			qi::rule<pos_iterator_type, types::integer(), qi::locals<types::integer>> start;
		};
	}
}

#endif /* _REVERSE_GRAMMAR_H */
