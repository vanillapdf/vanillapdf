#ifndef _SPIRIT_GRAMMAR_H
#define _SPIRIT_GRAMMAR_H

#include "qi_common.h"
#include "whitespace_grammar.h"

#include <boost/spirit/include/qi_grammar.hpp>

#pragma warning (push, 3)

// conversion from 'unsigned int' to 'float', possible loss of data in real_impl
#pragma warning (disable: 4244)

// signed/unsigned mismatch in real_impl
#pragma warning (disable: 4018)

#include <boost/spirit/home/qi/numeric/real.hpp>

#pragma warning (pop)

namespace gotchangpdf
{
	namespace syntax
	{
		namespace qi = boost::spirit::qi;

		class ContainableGrammar;

		class HexadecimalStringGrammar : public qi::grammar<pos_iterator_type,
			HexadecimalStringObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			HexadecimalStringGrammar();

		private:
			qi::rule<pos_iterator_type, HexadecimalStringObjectPtr(std::shared_ptr<File>*)> start;
		};

		class LiteralStringGrammar : public qi::grammar<pos_iterator_type,
			LiteralStringObjectPtr(std::shared_ptr<File>*),
			qi::locals<types::integer>>
		{
		public:
			LiteralStringGrammar();

		private:
			qi::rule<pos_iterator_type, LiteralStringObjectPtr(std::shared_ptr<File>*), qi::locals<types::integer>> start;
		};

		class StringGrammar : public qi::grammar<pos_iterator_type,
			StringObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			StringGrammar();

		private:
			qi::rule<pos_iterator_type, StringObjectPtr(std::shared_ptr<File>*)> start;
			HexadecimalStringGrammar hexadecimal_string;
			LiteralStringGrammar literal_string;
		};

		class NullGrammar : public qi::grammar<pos_iterator_type,
			NullObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			NullGrammar();

		private:
			qi::rule<pos_iterator_type, NullObjectPtr(std::shared_ptr<File>*)> start;
		};

		class IntegerGrammar : public qi::grammar<pos_iterator_type,
			IntegerObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			IntegerGrammar();

		private:
			qi::rule<pos_iterator_type, IntegerObjectPtr(std::shared_ptr<File>*)> start;
		};

		class RealGrammar : public qi::grammar<pos_iterator_type,
			RealObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			RealGrammar();

		private:
			qi::rule<pos_iterator_type, RealObjectPtr(std::shared_ptr<File>*)> start;
			qi::real_parser<float, qi::strict_real_policies<float>> strict_float_parser;
		};

		class IndirectObjectReferenceGrammar : public qi::grammar<pos_iterator_type,
			IndirectObjectReferencePtr(std::shared_ptr<File>*),
			qi::locals<types::integer, types::ushort>>
		{
		public:
			IndirectObjectReferenceGrammar();

		private:
			qi::rule<pos_iterator_type, IndirectObjectReferencePtr(std::shared_ptr<File>*), qi::locals<types::integer, types::ushort>> start;
			SingleWhitespace whitespace;
		};

		class ArrayGrammar : public qi::grammar<pos_iterator_type,
			MixedArrayObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			ArrayGrammar(ContainableGrammar &containable_object);

		private:
			qi::rule<pos_iterator_type, MixedArrayObjectPtr(std::shared_ptr<File>*)> start;
			ContainableGrammar &containable_object;
			Whitespace whitespaces;
		};

		class BooleanGrammar : public qi::grammar<pos_iterator_type,
			BooleanObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			BooleanGrammar();

		private:
			qi::rule<pos_iterator_type, BooleanObjectPtr(std::shared_ptr<File>*)> start;
		};

		class NameGrammar : public qi::grammar<pos_iterator_type,
			NameObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			NameGrammar();

		private:
			qi::rule<pos_iterator_type, NameObjectPtr(std::shared_ptr<File>*)> start;
			SingleWhitespace whitespace;
		};

		class DictionaryGrammar : public qi::grammar<pos_iterator_type,
			DictionaryObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			DictionaryGrammar(ContainableGrammar& containable_grammar);

		private:
			qi::rule<pos_iterator_type, DictionaryObjectPtr(std::shared_ptr<File>*)> start;
			ContainableGrammar &containable_object;
			NameGrammar name_object;
			Whitespace whitespaces;
			EndOfLine eol;
		};

		class StreamDataGrammar : public qi::grammar<pos_iterator_type,
			StreamObjectPtr(std::shared_ptr<File>*, DictionaryObjectPtr),
			qi::locals<types::stream_size, types::stream_size>>
		{
		public:
			StreamDataGrammar();

		private:
			qi::rule<pos_iterator_type, StreamObjectPtr(std::shared_ptr<File>*, DictionaryObjectPtr), qi::locals<types::stream_size, types::stream_size>> start;
			Whitespace whitespaces;
			EndOfLine eol;
		};

		class ContainableGrammar : public qi::grammar<pos_iterator_type,
			ContainableObjectPtr(std::shared_ptr<File>*)>
		{
		public:
			ContainableGrammar();

		private:
			qi::rule<pos_iterator_type, ContainableObjectPtr(std::shared_ptr<File>*)> start;
			ArrayGrammar array_object = { *this };
			BooleanGrammar boolean_object;
			DictionaryGrammar dictionary_object = { *this };
			IndirectObjectReferenceGrammar indirect_object_reference;
			RealGrammar real_object;
			IntegerGrammar integer_object;
			NameGrammar name_object;
			NullGrammar null_object;
			StringGrammar string_object;
		};

		class IndirectStreamGrammar : public qi::grammar<pos_iterator_type,
			StreamObjectPtr(std::shared_ptr<File>*, types::stream_offset),
			qi::locals<types::integer, types::ushort, DictionaryObjectPtr >>
		{
		public:
			IndirectStreamGrammar();

		private:
			qi::rule<pos_iterator_type, StreamObjectPtr(std::shared_ptr<File>*, types::stream_offset), qi::locals<types::integer, types::ushort, DictionaryObjectPtr>> start;
			StreamDataGrammar stream_data;
			ContainableGrammar containable_object;
			DictionaryGrammar dictionary_object = { containable_object };

			Whitespace whitespaces;
			SingleWhitespace whitespace;
		};

		class DictionaryOrStreamGrammar : public qi::grammar<pos_iterator_type,
			ObjectPtr(std::shared_ptr<File>*),
			qi::locals<types::stream_size, types::stream_size, DictionaryObjectPtr >>
		{
		public:
			DictionaryOrStreamGrammar();

		private:
			qi::rule<pos_iterator_type, ObjectPtr(std::shared_ptr<File>*), qi::locals<types::stream_size, types::stream_size, DictionaryObjectPtr>> start;
			ContainableGrammar containable_object;
			DictionaryGrammar dictionary_object = { containable_object };
			StreamDataGrammar stream_data;
			NameGrammar name_object;
			Whitespace whitespaces;
			EndOfLine eol;
		};

		class DirectObjectGrammar : public qi::grammar<pos_iterator_type,
			ObjectPtr(std::shared_ptr<File>*, types::stream_offset),
			qi::locals<types::integer, types::ushort>>
		{
		public:
			DirectObjectGrammar();

		private:
			qi::rule<pos_iterator_type, ObjectPtr(std::shared_ptr<File>*, types::stream_offset), qi::locals<types::integer, types::ushort>> start;
			qi::rule<pos_iterator_type, ObjectPtr(std::shared_ptr<File>*)> direct_object;

			ContainableGrammar containable_object;
			DictionaryOrStreamGrammar dict_or_stream;
			ArrayGrammar array_object = { containable_object };
			BooleanGrammar boolean_object;
			IndirectObjectReferenceGrammar indirect_object_reference;
			RealGrammar real_object;
			IntegerGrammar integer_object;
			NameGrammar name_object;
			NullGrammar null_object;
			StringGrammar string_object;

			SingleWhitespace whitespace;
			Whitespace whitespaces;
		};
	}
}

#endif /* _SPIRIT_GRAMMAR_H */
