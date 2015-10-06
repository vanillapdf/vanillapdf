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

		class ContainableGrammar;

		class HexadecimalStringGrammar : public qi::grammar<pos_iterator_type,
			HexadecimalStringObjectPtr()>
		{
		public:
			HexadecimalStringGrammar();

		private:
			qi::rule<pos_iterator_type, HexadecimalStringObjectPtr()> start;
		};

		class LiteralStringGrammar : public qi::grammar<pos_iterator_type,
			LiteralStringObjectPtr(),
			qi::locals<types::integer>>
		{
		public:
			LiteralStringGrammar();

		private:
			qi::rule<pos_iterator_type, LiteralStringObjectPtr(), qi::locals<types::integer>> start;
		};

		class NullGrammar : public qi::grammar<pos_iterator_type,
			NullObjectPtr()>
		{
		public:
			NullGrammar();

		private:
			qi::rule<pos_iterator_type, NullObjectPtr()> start;
		};

		class IntegerGrammar : public qi::grammar<pos_iterator_type,
			IntegerObjectPtr()>
		{
		public:
			IntegerGrammar();

		private:
			qi::rule<pos_iterator_type, IntegerObjectPtr()> start;
		};

		class RealGrammar : public qi::grammar<pos_iterator_type,
			RealObjectPtr()>
		{
		public:
			RealGrammar();

		private:
			qi::rule<pos_iterator_type, RealObjectPtr()> start;
			qi::real_parser<float, qi::strict_real_policies<float>> strict_float_parser;
		};

		class FunctionGrammar : public qi::grammar<pos_iterator_type,
			FunctionObjectPtr()>
		{
		public:
			FunctionGrammar();

		private:
			qi::rule<pos_iterator_type, FunctionObjectPtr()> start;
		};

		class IndirectObjectReferenceGrammar : public qi::grammar<pos_iterator_type,
			IndirectObjectReferencePtr(),
			qi::locals<types::integer, types::ushort>>
		{
		public:
			IndirectObjectReferenceGrammar();

		private:
			qi::rule<pos_iterator_type, IndirectObjectReferencePtr(), qi::locals<types::integer, types::ushort>> start;
			SingleWhitespace whitespace;
		};

		class ArrayGrammar : public qi::grammar<pos_iterator_type,
			MixedArrayObjectPtr(files::File*)>
		{
		public:
			ArrayGrammar(ContainableGrammar &containable_object);

		private:
			qi::rule<pos_iterator_type, MixedArrayObjectPtr(files::File*)> start;
			ContainableGrammar &containable_object;
			Whitespace whitespaces;
		};

		class BooleanGrammar : public qi::grammar<pos_iterator_type,
			BooleanObjectPtr()>
		{
		public:
			BooleanGrammar();

		private:
			qi::rule<pos_iterator_type, BooleanObjectPtr()> start;
		};

		class NameGrammar : public qi::grammar<pos_iterator_type,
			NameObjectPtr()>
		{
		public:
			NameGrammar();

		private:
			qi::rule<pos_iterator_type, NameObjectPtr()> start;
			SingleWhitespace whitespace;
		};

		class DictionaryGrammar : public qi::grammar<pos_iterator_type,
			DictionaryObjectPtr(files::File*)>
		{
		public:
			DictionaryGrammar(ContainableGrammar& containable_grammar);

		private:
			qi::rule<pos_iterator_type, DictionaryObjectPtr(files::File*)> start;
			ContainableGrammar &containable_object;
			NameGrammar name_object;
			Whitespace whitespaces;
			EndOfLine eol;
		};

		class ContainableGrammar : public qi::grammar<pos_iterator_type,
			ContainableObject(files::File*)>
		{
		public:
			ContainableGrammar();

		private:
			qi::rule<pos_iterator_type, ContainableObject(files::File*)> start;
			ArrayGrammar array_object = { *this };
			BooleanGrammar boolean_object;
			DictionaryGrammar dictionary_object = { *this };
			FunctionGrammar function_object;
			IndirectObjectReferenceGrammar indirect_object_reference;
			RealGrammar real_object;
			IntegerGrammar integer_object;
			NameGrammar name_object;
			NullGrammar null_object;
			LiteralStringGrammar literal_string_object;
			HexadecimalStringGrammar hexadecimal_string_object;
		};

		using DictionaryOrStream = boost::variant<DictionaryObjectPtr, StreamObjectPtr>;
		class DictionaryOrStreamGrammar : public qi::grammar<pos_iterator_type,
			DictionaryOrStream(files::File*),
			qi::locals<types::stream_size, types::stream_size, DictionaryObjectPtr >>
		{
		public:
			DictionaryOrStreamGrammar();

		private:
			qi::rule<pos_iterator_type, DictionaryOrStream(files::File*), qi::locals<types::stream_size, types::stream_size, DictionaryObjectPtr>> start;
			ContainableGrammar containable_object;
			DictionaryGrammar dictionary_object = { containable_object };
			NameGrammar name_object;
			Whitespace whitespaces;
			EndOfLine eol;
		};

		class DirectObjectGrammar : public qi::grammar<pos_iterator_type,
			DirectObject(files::File*, types::stream_offset),
			qi::locals<types::integer, types::ushort>>
		{
		public:
			DirectObjectGrammar();

		private:
			qi::rule<pos_iterator_type, DirectObject(files::File*, types::stream_offset), qi::locals<types::integer, types::ushort>> start;
			qi::rule<pos_iterator_type, DirectObject(files::File*)> direct_object;

			ContainableGrammar containable_object;
			DictionaryOrStreamGrammar dict_or_stream;
			ArrayGrammar array_object = { containable_object };
			BooleanGrammar boolean_object;
			FunctionGrammar function_object;
			IndirectObjectReferenceGrammar indirect_object_reference;
			RealGrammar real_object;
			IntegerGrammar integer_object;
			NameGrammar name_object;
			NullGrammar null_object;
			LiteralStringGrammar literal_string_object;
			HexadecimalStringGrammar hexadecimal_string_object;

			SingleWhitespace whitespace;
			Whitespace whitespaces;
		};
	}
}

#endif /* _SPIRIT_GRAMMAR_H */
