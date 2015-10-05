#ifndef _SPIRIT_GRAMMAR_H
#define _SPIRIT_GRAMMAR_H

#include "qi_common.h"
#include "whitespace_grammar.h"

#pragma warning (push, 3)
#include <boost/spirit/include/qi.hpp>
#pragma warning (pop)

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		class DirectObjectGrammar : public qi::grammar<pos_iterator_type,
			DirectObject(files::File*, types::stream_offset),
			qi::locals<types::integer, types::ushort>>
		{
		public:
			DirectObjectGrammar();

		private:
			template <typename A, typename... Inherited>
			using Rule = qi::rule<pos_iterator_type, A(Inherited...)>;

			qi::rule<pos_iterator_type, DirectObject(files::File*, types::stream_offset), qi::locals<types::integer, types::ushort>> start;
			Rule<DirectObject, files::File*> direct_object;
			Rule<ContainableObject, files::File*> containable_object;
			Rule<MixedArrayObjectPtr, files::File*> array_object;
			Rule<BooleanObjectPtr> boolean_object;
			Rule<DictionaryObjectPtr, files::File*> dictionary_object;
			Rule<DictionaryObject, files::File*> dictionary_object_raw;
			Rule<FunctionObjectPtr> function_object;
			qi::rule<pos_iterator_type, IndirectObjectReferencePtr(), qi::locals<types::integer, types::ushort>> indirect_object_reference;
			Rule<IntegerObjectPtr> integer_object;
			Rule<NameObjectPtr> name_object;
			Rule<NullObjectPtr> null_object;
			Rule<RealObjectPtr> real_object;
			qi::rule<pos_iterator_type, StreamObjectPtr(files::File*), qi::locals<DictionaryObjectPtr, types::stream_size>> stream_object;
			qi::rule<pos_iterator_type, LiteralStringObjectPtr(), qi::locals<types::integer>> literal_string_object;
			//Rule<LiteralStringObjectPtr> literal_string_object;
			Rule<HexadecimalStringObjectPtr> hexadecimal_string_object;

			qi::real_parser<float, qi::strict_real_policies<float>> strict_float_parser;

			EndOfLine eol;
			SingleWhitespace whitespace;
			Whitespace whitespaces;
		};
	}
}

#endif /* _SPIRIT_GRAMMAR_H */
