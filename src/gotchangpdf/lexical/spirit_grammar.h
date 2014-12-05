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
			IndirectObjectPtr(files::File*)>
		{
			SpiritGrammar(const lexical::SpiritLexer& lexer);

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<SpiritLexer::iterator_type, A(Inherited...)>;
			Rule<IndirectObjectPtr, files::File*> indirect_object;

			Rule<DirectObject, files::File*> direct_object;

			Rule<MixedArrayObjectPtr, files::File*> array_object;
			Rule<BooleanObjectPtr> boolean_object;
			Rule<DictionaryObjectPtr, files::File*> dictionary_object;
			Rule<FunctionObjectPtr> function_object;
			Rule<IndirectObjectReferencePtr> indirect_reference_object;
			Rule<IntegerObjectPtr> integer_object;
			Rule<NameObjectPtr> name_object;
			Rule<NullObjectPtr> null_object;
			Rule<RealObjectPtr> real_object;
			Rule<StreamObjectPtr, files::File*> stream_object;
			Rule<StringObjectPtr> string_object;
			Rule<LiteralStringPtr> literal_string_object;
			Rule<HexadecimalStringPtr> hexadecimal_string_object;
		};
	}
}

#endif
