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

		class DirectObjectGrammar : public qi::grammar<pos_iterator_type,
			DirectObject(files::File*)>
		{
		public:
			DirectObjectGrammar();

		private:
			template <typename A, typename... Inherited>
			using Rule = qi::rule<pos_iterator_type, A(Inherited...)>;

			Rule<DirectObject, files::File*> direct_object;
			Rule<MixedArrayObjectPtr, files::File*> array_object;
			Rule<BooleanObjectPtr> boolean_object;
			Rule<DictionaryObjectPtr, files::File*> dictionary_object;
			Rule<FunctionObjectPtr> function_object;
			Rule<IndirectObjectReferencePtr> indirect_object_reference;
			Rule<IntegerObjectPtr> integer_object;
			Rule<NameObjectPtr> name_object;
			Rule<NullObjectPtr> null_object;
			Rule<RealObjectPtr> real_object;
			qi::rule<pos_iterator_type, StreamObjectPtr(files::File*), qi::locals<DictionaryObjectPtr, types::stream_size>> stream_object;
			Rule<LiteralStringPtr> literal_string_object;
			Rule<HexadecimalStringPtr> hexadecimal_string_object;

			qi::real_parser<float, qi::strict_real_policies<float>> strict_float_parser;
		};

		class IndirectObjectGrammar : public qi::grammar<pos_iterator_type,
			IndirectObjectPtr(files::File*)>
		{
		public:
			IndirectObjectGrammar();

		private:
			template <typename A, typename... Inherited>
			using Rule = qi::rule<pos_iterator_type, A(Inherited...)>;

			Rule<IndirectObjectPtr, files::File*> indirect_object;
			Rule<IntegerObjectPtr> object_number;
			Rule<IntegerObjectPtr> generation_number;
			DirectObjectGrammar direct_object;
		};
	}
}

#endif
