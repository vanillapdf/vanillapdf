#ifndef _CONTENT_STREAM_GRAMMAR_H
#define _CONTENT_STREAM_GRAMMAR_H

#include "qi_common.h"
#include "content_stream_operations.h"
#include "content_stream_operator_grammar.h"
#include "spirit_grammar.h"

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			namespace qi = boost::spirit::qi;

			class OperandGrammar : public qi::grammar<pos_iterator_type,
				Operand(std::shared_ptr<File>*)>
			{
			public:
				OperandGrammar();

			private:
				qi::rule<pos_iterator_type, Operand(std::shared_ptr<File>*)> start;

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

			class ContentStreamGrammar : public qi::grammar<pos_iterator_type,
				OperationCollection(std::shared_ptr<File>*)>
			{
			public:
				ContentStreamGrammar();

			private:
				qi::rule<pos_iterator_type, OperationCollection(std::shared_ptr<File>*)> start;
				qi::rule<pos_iterator_type, OperationPtr(std::shared_ptr<File>*)> operation;

				OperandGrammar _operand;
				OperatorGrammar _operator;
				EndOfLine eol;
				Whitespace whitespaces;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_GRAMMAR_H */
