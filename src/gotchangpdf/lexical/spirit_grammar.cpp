#include "spirit_grammar.h"

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;

		using qi::lit;

		SpiritGrammar::SpiritGrammar(const lexical::SpiritLexer& lexer) :
			base_type(boolean_object, "Grammar")
		{
			//auto local_begin = qi::lazy(boost::phoenix::construct<qi::position>(qi::_a, qi::_b));
			//start %= qi::eps[qi::_a = qi::_r1, qi::_b = qi::_r2] >> indirect_object;
			//start %= qi::eps [qi::_a = qi::_r1] >> boolean_object;

			auto true_ = boost::spirit::qi::as<ast::True>()[(
				qi::eps
				>> lexer.true_
				)];

			auto false_ = boost::spirit::qi::as<ast::False>()[(
				qi::eps
				>> lexer.false_
				)];

			boolean_object =
				true_
				| false_;

			/*
			auto integer_object = boost::spirit::qi::as<ast::IntegerObject>()[(
				qi::eps
				>> lexer.integer
				)];

			indirect_object %=
				integer_object
				>> qi::omit[lexer.space]
				>> integer_object
				>> qi::omit[lexer.space]
				>> lexer.obj
				>> -(qi::omit[lexer.carriage_return])
				>> qi::omit[lexer.line_feed]
				>> direct_object
				>> -(qi::omit[lexer.carriage_return])
				>> qi::omit[lexer.line_feed]
				>> lexer.endobj;

			string_object =
				literal_string_object
				| hexadecimal_string_object;

			direct_object =
				array_object
				| boolean_object
				| dictionary_object
				| function_object
				| indirect_reference_object
				| integer_object
				| name_object
				| null_object
				| real_object
				| stream_object
				| string_object;

			indirect_reference_object %=
				integer_object
				>> integer_object
				>> lexer.indirect_reference_marker;
				*/
			/*
			integer_object %=
				qi::eps
				>> lexer.integer;

			real_object %=
				qi::eps
				>> lexer.float_;

			// TODO
			name_object %=
				qi::eps
				>> lexer.name_object_begin;

			// TODO
			hexadecimal_string_object %=
				lexer.less_than_sign
				>> lexer.anything
				>> lexer.greater_than_sign;

			array_object %=
				lexer.left_bracket
				>> direct_object
				>> lexer.right_bracket;

			dictionary_object %=
				lexer.dictionary_begin
				>> *(name_object >> direct_object)
				>> lexer.dictionary_end;

			stream_object %=
				dictionary_object
				>> lexer.stream_begin
				>> repo::qi::iter_pos;

			// TODO
			literal_string_object %=
				lexer.left_parenthesis
				>> lexer.anything
				>> lexer.right_parenthesis;
				*/
		}
	}
}
