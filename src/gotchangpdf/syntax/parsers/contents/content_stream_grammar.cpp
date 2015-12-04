#include "precompiled.h"
#include "content_stream_grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			namespace phoenix = boost::phoenix;

			void convert_array_to_object(MixedArrayObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_dictionary_to_object(DictionaryObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_integer_to_object(IntegerObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_real_to_object(RealObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_null_to_object(NullObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_string_to_object(StringObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_name_to_object(NameObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_reference_to_object(IndirectObjectReferencePtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_function_to_object(FunctionObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }
			void convert_boolean_to_object(BooleanObjectPtr obj, ObjectPtr& result) { return convert(obj, result); }

			OperandGrammar::OperandGrammar() :
				base_type(start, "Content stream operand grammar")
			{
				start %=
					array_object(qi::_r1)[phoenix::bind(&convert_array_to_object, qi::_1, qi::_val)]
					| boolean_object(qi::_r1)[phoenix::bind(&convert_boolean_to_object, qi::_1, qi::_val)]
					| dictionary_object(qi::_r1)[phoenix::bind(&convert_dictionary_to_object, qi::_1, qi::_val)]
					| function_object(qi::_r1)[phoenix::bind(&convert_function_to_object, qi::_1, qi::_val)]
					| real_object(qi::_r1)[phoenix::bind(&convert_real_to_object, qi::_1, qi::_val)]
					| integer_object(qi::_r1)[phoenix::bind(&convert_integer_to_object, qi::_1, qi::_val)]
					| name_object(qi::_r1)[phoenix::bind(&convert_name_to_object, qi::_1, qi::_val)]
					| null_object(qi::_r1)[phoenix::bind(&convert_null_to_object, qi::_1, qi::_val)]
					| string_object(qi::_r1)[phoenix::bind(&convert_string_to_object, qi::_1, qi::_val)];

				BOOST_SPIRIT_DEBUG_NODE(start);
			}

			ContentStreamGrammar::ContentStreamGrammar() :
				base_type(start, "Content stream grammar")
			{
				start %=
					*(operation(qi::_r1) > whitespaces);

				operation %=
					qi::eps
					>> qi::omit
					[
						(
							*(_operand(qi::_r1) > whitespaces)
							>> _operator
						)[qi::_val = phoenix::construct<OperationGenericPtr>(qi::_1, qi::_2)]
					];

				BOOST_SPIRIT_DEBUG_NODE(start);
				BOOST_SPIRIT_DEBUG_NODE(operation);
			}
		}
	}
}
