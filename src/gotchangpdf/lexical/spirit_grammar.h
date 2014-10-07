#ifndef SPIRIT_GRAMMAR_H
#define SPIRIT_GRAMMAR_H

#include "spirit_lexer.h"
#include "spirit_value_grammar.h"
#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;
		//namespace ast = gotchangpdf::lexical::ast;

		struct SpiritGrammar : qi::grammar<lexical::SpiritLexer::Iterator,
			ast::IndirectObject(lexical::pos_iterator_type, int),
			qi::locals<lexical::pos_iterator_type, int> >
		{
			SpiritGrammar(const lexical::SpiritLexer& lexer) :
			base_type(start, "Grammar"),
			value_grammar(lexer)
			{
				auto const value = value_grammar(qi::_a, qi::_b);

				//auto local_begin = qi::lazy(boost::phoenix::construct<qi::position>(qi::_a, qi::_b));
				//start %= qi::eps[qi::_a = qi::_r1, qi::_b = qi::_r2] >> program;

				indirect_object %=
					integer_object
					>> integer_object
					> lexer.obj
					>> direct_object;
				/*
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
					> lexer.indirect_reference_marker;

				integer_object =
					qi::eps
					>> lexer.integer;
					*/
				/*
				array_object %=
					lexer.left_bracket
					>> direct_object;
					//> lexer.right_bracket;
					*/

				/*
				//dictionary_object %=
				//	lexer.dictionary_begin;
				//	//>> *(name_object >> direct_object)
				//	//>> lexer.dictionary_end;
				//	*/

				//// TODO
				//name_object %=
				//	qi::eps
				//	>> lexer.name_object_begin;

				//real_object %=
				//	qi::eps
				//	>> lexer.float_;
				///*
				//stream_object %=
				//	qi::eps
				//	>> dictionary_object
				//	> lexer.stream_begin;
				//	*/
				//// TODO
				//literal_string_object %=
				//	qi::eps
				//	> lexer.left_parenthesis;

				//hexadecimal_string_object %=
				//	qi::eps
				//	> lexer.less_than_sign;
			}

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<lexical::SpiritLexer::Iterator, A(Inherited...), qi::locals<lexical::pos_iterator_type, int>>;
			Rule<ast::IndirectObject, lexical::pos_iterator_type, int> start;

			Rule<ast::IndirectObject> indirect_object;
			Rule<ast::DirectObject> direct_object;
			Rule<ast::ArrayObject> array_object;
			Rule<ast::BooleanObject> boolean_object;
			Rule<ast::DictionaryObject> dictionary_object;
			Rule<ast::FunctionObject> function_object;
			Rule<ast::IndirectReferenceObject> indirect_reference_object;
			Rule<ast::IntegerObject> integer_object;
			Rule<ast::NameObject> name_object;
			Rule<ast::NullObject> null_object;
			Rule<ast::RealObject> real_object;
			Rule<ast::StreamObject> stream_object;
			Rule<ast::StringObject> string_object;
			Rule<ast::LiteralStringObject> literal_string_object;
			Rule<ast::HexadecimalStringObject> hexadecimal_string_object;

			const SpiritValueGrammar value_grammar;
		};
	}
}

#endif
