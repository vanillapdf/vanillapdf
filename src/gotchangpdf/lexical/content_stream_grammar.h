#ifndef _CONTENT_STREAM_GRAMMAR_H
#define _CONTENT_STREAM_GRAMMAR_H

#include "qi_common.h"
#include "content_stream_operator_grammar.h"
#include "spirit_grammar.h"

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		typedef boost::variant <
			// first is null object
			NullObjectPtr,
			MixedArrayObjectPtr,
			NameObjectPtr,
			DictionaryObjectPtr,
			FunctionObjectPtr,
			BooleanObjectPtr,
			IntegerObjectPtr,
			RealObjectPtr,
			LiteralStringObjectPtr,
			HexadecimalStringObjectPtr
		> ContentStreamOperand;

		class ContentStreamOperandGrammar : public qi::grammar<pos_iterator_type,
			ContentStreamOperand(files::File*)>
		{
		public:
			ContentStreamOperandGrammar();

		private:
			qi::rule<pos_iterator_type, ContentStreamOperand(files::File*)> start;

			ContainableGrammar containable_object;
			NullGrammar null_object;
			ArrayGrammar array_object = { containable_object };
			NameGrammar name_object;
			DictionaryGrammar dictionary_object = { containable_object };
			FunctionGrammar function_object;
			BooleanGrammar boolean_object;
			IntegerGrammar integer_object;
			RealGrammar real_object;
			LiteralStringGrammar literal_string_object;
			HexadecimalStringGrammar hexadecimal_string_object;
		};

		typedef std::pair<std::vector<ContentStreamOperand>, ContentStreamOperator> ContentStreamOperation;
		typedef std::vector<ContentStreamOperation> ContentStreamOperationCollection;

		class ContentStreamGrammar : public qi::grammar<pos_iterator_type,
			ContentStreamOperationCollection(files::File*)>
		{
		public:
			ContentStreamGrammar();

		private:
			qi::rule<pos_iterator_type, ContentStreamOperationCollection(files::File*)> start;
			qi::rule<pos_iterator_type, ContentStreamOperation(files::File*)> operation;

			ContentStreamOperandGrammar _operand;
			ContentStreamOperatorGrammar _operator;
			EndOfLine eol;
			Whitespace whitespaces;
		};
	}
}

#endif /* _CONTENT_STREAM_GRAMMAR_H */
