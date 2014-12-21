#ifndef SPIRIT_GRAMMAR_H
#define SPIRIT_GRAMMAR_H

#include "abstract_syntax_tree.h"
#include "file_position.h"
#include "offset_iterator.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef offset_iterator<base_iterator_type, pdf_position> pos_iterator_type;

		namespace qi = boost::spirit::qi;

		struct SpiritGrammar : qi::grammar<pos_iterator_type,
			IndirectObjectPtr(files::File*)>
		{
			SpiritGrammar();

		private:
			template <typename A, typename... Inherited> using Rule = qi::rule<pos_iterator_type, A(Inherited...)>;

			Rule<IndirectObjectPtr, files::File*> indirect_object;
			Rule<DirectObject, files::File*> direct_object;
			Rule<MixedArrayObjectPtr, files::File*> array_object;
			Rule<BooleanObjectPtr> boolean_object;
			Rule<DictionaryObjectPtr, files::File*> dictionary_object;
			Rule<FunctionObjectPtr> function_object;
			//Rule<IndirectObjectReferencePtr, files::File*> indirect_reference_object;
			qi::rule<pos_iterator_type, IndirectObjectReferencePtr(files::File*), qi::locals<IntegerObjectPtr, IntegerObjectPtr>> indirect_reference_object;
			Rule<IntegerObjectPtr> integer_object;
			Rule<NameObjectPtr> name_object;
			Rule<NameObject> name_key;
			Rule<NullObjectPtr> null_object;
			Rule<RealObjectPtr> real_object;
			qi::rule<pos_iterator_type, StreamObjectPtr(files::File*), qi::locals<DictionaryObjectPtr, int>> stream_object;
			Rule<StringObjectPtr> string_object;
			Rule<LiteralStringPtr> literal_string_object;
			Rule<HexadecimalStringPtr> hexadecimal_string_object;
		};
	}
}

#endif
