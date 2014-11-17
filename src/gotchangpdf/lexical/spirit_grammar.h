#ifndef SPIRIT_GRAMMAR_H
#define SPIRIT_GRAMMAR_H

#include "spirit_lexer.h"
#include "abstract_syntax_tree.h"

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		struct SpiritGrammar : qi::grammar<SpiritLexer::iterator_type,
			ast::IndirectObject()>
		{
			SpiritGrammar(const lexical::SpiritLexer& lexer);

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<SpiritLexer::iterator_type, A(Inherited...)>;

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
		};
	}
}

#endif
