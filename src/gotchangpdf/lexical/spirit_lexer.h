#ifndef SPIRIT_LEXER_H
#define SPIRIT_LEXER_H

#include "buffer.h"
#include "constants.h"

#include <istream>
#include <string>
#include <utility>
#include <stack>

#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/lex_static_lexertl.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/qi.hpp>

#ifdef USE_STATIC_LEXER
#include "static_lexer.hpp"
#endif

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

		//typedef lex::lexertl::token<pos_iterator_type> token_type;

		typedef boost::spirit::lex::lexertl::token <
			//pos_iterator_type, boost::mpl::vector<Buffer, int, char, double>
			pos_iterator_type
		> token_type;

#ifdef USE_STATIC_LEXER
		typedef boost::spirit::lex::lexertl::static_actor_lexer<Tok, boost::spirit::lex::lexertl::static_::lexer_sl> lexer_type;
#else
		//typedef lex::lexertl::actor_lexer<token_type> lexer_type;
		typedef boost::spirit::lex::lexertl::lexer<token_type> lexer_type;
#endif

		class SpiritLexer : public boost::spirit::lex::lexer<lexer_type>
		{
		public:
			SpiritLexer();

		public:
			typedef boost::spirit::lex::token_def<boost::spirit::lex::omit> ConsumedToken;
			typedef boost::spirit::lex::token_def<Buffer> StringToken;
			typedef boost::spirit::lex::token_def<types::integer> IntegerToken;
			typedef boost::spirit::lex::token_def<types::ushort> UShortToken;
			typedef boost::spirit::lex::token_def<char> CharToken;
			typedef boost::spirit::lex::token_def<double> FloatToken;

			//typedef lexer_type::iterator_type Iterator;

		public:
			StringToken anything;
			//StringToken name_object_value;

		public:
			/* keywords */
			ConsumedToken true_, false_;
			ConsumedToken obj, endobj;
			ConsumedToken stream_begin, stream_end;

			/* delimiters */
			ConsumedToken left_bracket, right_bracket;
			ConsumedToken less_than_sign, greater_than_sign;
			ConsumedToken left_parenthesis, right_parenthesis;
			ConsumedToken left_curly_bracket, right_curly_bracket;
			ConsumedToken solidus;
			ConsumedToken percent_sign;

			/* whitespace */
			ConsumedToken line_feed, space, carriage_return, null, horizontal_tab, form_feed;
			
			ConsumedToken indirect_reference_marker;
			//ConsumedToken dot;

			ConsumedToken dictionary_begin, dictionary_end;
			//CharToken plus, minus;
			StringToken regular_characters;
			IntegerToken integer;
			FloatToken float_;
		};
	}
}
#endif
