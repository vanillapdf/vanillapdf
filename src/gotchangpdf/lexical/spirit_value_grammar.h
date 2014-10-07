#ifndef SPIRIT_VALUE_GRAMMAR_H
#define SPIRIT_VALUE_GRAMMAR_H

#include "spirit_lexer.h"
#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;
		//namespace ast = gotchangpdf::lexical::ast;

		struct SpiritValueGrammar : qi::grammar<lexical::SpiritLexer::Iterator,
			ast::DirectObject(lexical::pos_iterator_type, int),
			qi::locals<lexical::pos_iterator_type, int> >
		{
			SpiritValueGrammar(const lexical::SpiritLexer& lexer) :
			base_type(start, "_value Grammar")
			{
				//auto local_begin = qi::lazy(boost::phoenix::construct<qi::position>(qi::_a, qi::_b));
				//start %= qi::eps[qi::_a = qi::_r1, qi::_b = qi::_r2] >> program;
			}

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<lexical::SpiritLexer::Iterator, A(Inherited...), qi::locals<lexical::pos_iterator_type, int>>;
			Rule<ast::DirectObject, lexical::pos_iterator_type, int> start;

			Rule<ast::DirectObject> direct_object;

		};
	}
}

#endif
