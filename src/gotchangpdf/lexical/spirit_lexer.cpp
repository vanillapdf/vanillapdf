#include "spirit_lexer.h"

namespace gotchangpdf
{
	namespace lexical
	{
		namespace spirit = boost::spirit;
		namespace lex = boost::spirit::lex;

		SpiritLexer::SpiritLexer()
		{
			/* keywords */
			true_ = "true";
			false_ = "false";
			obj = "obj";
			endobj = "endobj";
			stream_begin = "stream";
			stream_end = "endstream";

			/* delimiters */
			left_parenthesis = '(';
			right_parenthesis = ')';
			less_than_sign = '<';
			greater_than_sign = '>';
			left_bracket = '[';
			right_bracket = ']';
			left_curly_bracket = '{';
			right_curly_bracket = '}';
			solidus = '/';
			percent_sign = '%';

			/* whitespace */
			line_feed = '\n';
			space = ' ';
			carriage_return = '\r';
			null = '\0';
			horizontal_tab = '\t';
			form_feed = '\f';

			dictionary_begin = "<<";
			dictionary_end = ">>";

			indirect_reference_marker = 'R';

			word = "[0-9a-zA-Z+,]+";

			//plus = '+';
			//minus = '-';
			//dot = '.';

			float_ = "[0-9]+\".\"[0-9]+";
			integer = "(\\+|-)?[0-9]+";

			//anything = ".*";
			//literal_text = "[0-9a-zA-Z:./ ]+";
			parenthesed_string = "\\([0-9a-zA-Z:./ ]+\\)";
			//parenthesed_string = "a" | "b";

			//this->self += anything;

			//this->self += name_object_begin;
			//this->self += name_object_value;

			this->self += true_ | false_;
			this->self += obj | endobj;
			this->self += dictionary_begin | dictionary_end;
			this->self += stream_begin | stream_end;
			this->self += line_feed | space | carriage_return | null | horizontal_tab | form_feed;
			this->self += less_than_sign | greater_than_sign | left_bracket | right_bracket | left_parenthesis | right_parenthesis | left_curly_bracket | right_curly_bracket | solidus | percent_sign;

			this->self += indirect_reference_marker;

			this->self += integer | float_;
			this->self += word;
			this->self += parenthesed_string;

			//this->self += indirect_reference_marker;
			//this->self += dot;

			//this->self("WS") = line_feed | space | carriage_return | null | horizontal_tab | form_feed;

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
	}
}
