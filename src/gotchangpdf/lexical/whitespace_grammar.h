#ifndef _WHITESPACE_GRAMMAR_H
#define _WHITESPACE_GRAMMAR_H

#include "file_position.h"
#include "offset_iterator.h"
#include "qi_common.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		class EndOfLine : public qi::grammar<pos_iterator_type, void()>
		{
		public:
			EndOfLine();

		private:
			qi::rule<pos_iterator_type, void()> start;
		};

		class SingleWhitespace : public qi::grammar<pos_iterator_type, void()>
		{
		public:
			SingleWhitespace();

		private:
			qi::rule<pos_iterator_type, void()> start;
		};

		class Whitespace : public qi::grammar<pos_iterator_type, void()>
		{
		public:
			Whitespace();

		private:
			qi::rule<pos_iterator_type, void()> start;
			SingleWhitespace single_whitespace;
		};
	}
}

#endif /* _WHITESPACE_GRAMMAR_H */
