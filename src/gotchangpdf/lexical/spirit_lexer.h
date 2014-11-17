#ifndef SPIRIT_LEXER_H
#define SPIRIT_LEXER_H

#include "buffer.h"

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
		namespace spirit = boost::spirit;
		namespace lex = boost::spirit::lex;


		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

		//typedef lex::lexertl::token<pos_iterator_type> token_type;

		typedef lex::lexertl::token <
			pos_iterator_type, boost::mpl::vector<Buffer, int, char, double>
		> token_type;

#ifdef USE_STATIC_LEXER
		typedef boost::spirit::lex::lexertl::static_actor_lexer<Tok, lex::lexertl::static_::lexer_sl> lexer_type;
#else
		//typedef lex::lexertl::actor_lexer<token_type> lexer_type;
		typedef lex::lexertl::lexer<token_type> lexer_type;
#endif

		class SpiritLexer : public lex::lexer<lexer_type>
		{
		public:
			typedef lex::token_def<lex::omit> ConsumedToken;
			typedef lex::token_def<Buffer> StringToken;
			typedef lex::token_def<int> IntegerToken;
			typedef lex::token_def<char> CharToken;
			typedef lex::token_def<double> FloatToken;

			//typedef lexer_type::iterator_type Iterator;

		public:
			SpiritLexer()
			{
				/* keywords */
				true_ = "true";
				false_ = "false";

				obj = "obj";
				endobj = "endobj";

				dictionary_begin = "<<";
				dictionary_end = ">>";

				stream_begin = "stream";
				stream_end = "endstream";

				left_parenthesis = '(';
				right_parenthesis = ')';

				less_than_sign = '<';
				greater_than_sign = '>';

				indirect_reference_marker = 'R';
				name_object_begin = '/';

				plus = '+';
				minus = '-';
				dot = '.';

				line_feed = '\n';
				space = ' ';
				carriage_return = '\r';
				null = '\0';
				horizontal_tab = '\t';
				form_feed = '\f';

				left_bracket = '[';
				right_bracket = ']';

				float_ = "[0-9]+\".\"[0-9]+";
				integer = "[0-9]+";

				anything = ".*";

				//this->self += anything;

				this->self += true_ | false_;
				this->self += integer | float_;
				this->self += obj | endobj;

				this->self += indirect_reference_marker;
				//this->self += dot;

				this->self += line_feed | space | carriage_return | null | horizontal_tab | form_feed;

				/* Raw values */
				/*
				identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
				float_ = "[0-9]+\".\"[0-9]+";
				integer = "[0-9]+";
				string_literal = "\\\"[^\\\"]*\\\"";
				left_parenth = '(';
				right_parenth = ')';
				left_brace = '{';
				right_brace = '}';
				left_bracket = '[';
				right_bracket = ']';
				whitespaces = "[ \\t\\n]+";
				multiline_comment = "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/";
				singleline_comment = "\\/\\/[^\n]*";
				*/
				//Ignore whitespaces
				/*
				this->self += whitespaces[lex::_pass = lex::pass_flags::pass_ignore];
				this->self += left_parenth | right_parenth | left_brace | right_brace | left_bracket | right_bracket;
				this->self += float_ | integer | string_literal;
				this->self += assign | swap;
				this->self += comma | stop | dot;
				this->self += addition | subtraction | multiplication | division | modulo;
				this->self += compound_add | compound_sub | compound_mul | compound_div | compound_mod;
				this->self += increment | decrement;
				this->self += for_ | do_ | while_ | true_ | false_ | if_ | else_ | from_ | to_ | in_ | foreach_ | return_ | const_ | include | struct_ | null | this_;
				this->self += new_ | delete_;
				this->self += and_ | or_;
				this->self += addressof;
				this->self += equals | not_equals | greater_equals | less_equals | greater | less | not_;
				this->self += double_dot | question_mark | tilde;
				this->self += template_ | type | extends;
				this->self += case_ | switch_ | default_;
				this->self += identifier | char_literal;

				//Ignore comments
				this->self += multiline_comment[lex::_pass = lex::pass_flags::pass_ignore];
				this->self += singleline_comment[lex::_pass = lex::pass_flags::pass_ignore];
				*/
			}

			StringToken anything;

			ConsumedToken true_, false_;
			ConsumedToken obj, endobj;
			StringToken stream_begin, stream_end;
			ConsumedToken dictionary_begin, dictionary_end;
			CharToken line_feed, space, carriage_return, null, horizontal_tab, form_feed;
			ConsumedToken left_bracket, right_bracket, left_parenthesis, right_parenthesis;
			ConsumedToken less_than_sign, greater_than_sign;
			ConsumedToken indirect_reference_marker;
			ConsumedToken dot, name_object_begin;
			CharToken plus, minus;
			IntegerToken integer;
			FloatToken float_;
		};
	}
}
#endif
