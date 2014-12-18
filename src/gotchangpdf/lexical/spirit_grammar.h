#ifndef SPIRIT_GRAMMAR_H
#define SPIRIT_GRAMMAR_H

#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

		namespace qi = boost::spirit::qi;

		struct SpiritGrammar : qi::grammar<lexical::pos_iterator_type,
			IndirectObjectPtr(files::File*)>
		{
			SpiritGrammar();

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<lexical::pos_iterator_type, A(Inherited...)>;
			Rule<IndirectObjectPtr, files::File*> indirect_object;

			Rule<DirectObject, files::File*> direct_object;
			Rule<MixedArrayObjectPtr, files::File*> array_object;
			Rule<BooleanObjectPtr> boolean_object;
			Rule<DictionaryObjectPtr, files::File*> dictionary_object;
			Rule<FunctionObjectPtr> function_object;
			Rule<IndirectObjectReferencePtr> indirect_reference_object;
			Rule<IntegerObjectPtr> integer_object;
			Rule<NameObjectPtr> name_object;
			Rule<NameObject> name_key;
			Rule<NullObjectPtr> null_object;
			Rule<RealObjectPtr> real_object;
			qi::rule<lexical::pos_iterator_type, StreamObjectPtr(files::File*), qi::locals<DictionaryObjectPtr, int>> stream_object;
			Rule<StringObjectPtr> string_object;
			Rule<LiteralStringPtr> literal_string_object;
			Rule<HexadecimalStringPtr> hexadecimal_string_object;
		};
	}
}

#endif
