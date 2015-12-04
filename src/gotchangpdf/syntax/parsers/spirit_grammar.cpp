#include "precompiled.h"
#include "spirit_grammar.h"

#include "iter_offset_parser.h"
#include "abstract_syntax_tree.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_advance.hpp>

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

void direct_object_file_handler(ObjectPtr obj, std::shared_ptr<File>* file)
{
	assert(nullptr != file && *file);

	obj->SetFile(*file);

	// This is a special case, which tries to dereference the object type
	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(obj)) {
		return;
	}

	if (ObjectUtils::IsType<StreamObjectPtr>(obj)) {
		auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(obj);
		auto header = converted->GetHeader();
		direct_object_file_handler(header, file);
	}
	else if (ObjectUtils::IsType<MixedArrayObjectPtr>(obj)) {
		auto converted = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(obj);

		for (auto item : *converted) {
			direct_object_file_handler(item, file);
		}
	}
	else if (ObjectUtils::IsType<DictionaryObjectPtr>(obj)) {
		auto converted = ObjectUtils::ConvertTo<DictionaryObjectPtr>(obj);
		for (auto item : *converted) {
			direct_object_file_handler(item.first, file);
			direct_object_file_handler(item.second, file);
		}
	}
}

void direct_object_offset_handler(ObjectPtr obj, types::stream_offset offset)
{
	obj->SetOffset(offset);
}

void indirect_object_handler(ObjectPtr obj, types::integer obj_number, types::ushort gen_number)
{
	obj->SetObjectNumber(obj_number);
	obj->SetGenerationNumber(gen_number);
}

void dictionary_length(const DictionaryObjectPtr& obj, types::stream_size& value)
{
	auto size = obj->FindAs<IntegerObjectPtr>(constant::Name::Length);
	value = static_cast<types::stream_size>(*size);
}

namespace gotchangpdf
{
	namespace syntax
	{
		namespace repo = boost::spirit::repository;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;

		void literal_to_string(LiteralStringObjectPtr& obj, StringObjectPtr& result) { return convert(obj, result); }
		void hexadecimal_to_string(HexadecimalStringObjectPtr& obj, StringObjectPtr& result) { return convert(obj, result); }

		void convert_array(MixedArrayObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_dictionary(DictionaryObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_integer(IntegerObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_real(RealObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_null(NullObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_string(StringObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_name(NameObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_reference(IndirectObjectReferencePtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_function(FunctionObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }
		void convert_boolean(BooleanObjectPtr& obj, ContainableObjectPtr& result) { return convert(obj, result); }

		void convert_stream_to_obj(StreamObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_array_to_obj(MixedArrayObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_dictionary_to_obj(DictionaryObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_integer_to_obj(IntegerObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_real_to_obj(RealObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_null_to_obj(NullObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_string_to_obj(StringObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_name_to_obj(NameObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_reference_to_obj(IndirectObjectReferencePtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_function_to_obj(FunctionObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }
		void convert_boolean_to_obj(BooleanObjectPtr& obj, ObjectPtr& result) { return convert(obj, result); }

		DirectObjectGrammar::DirectObjectGrammar() :
			base_type(start, "Direct object grammar")
		{
			start %=
				(
					qi::omit[qi::int_[qi::_a = qi::_1]]
					>> whitespace
					>> qi::omit[qi::ushort_[qi::_b = qi::_1]]
					>> whitespace
					>> qi::lit("obj")
					>> whitespaces
					>> direct_object(qi::_r1)
					[
						phoenix::bind(&direct_object_offset_handler, qi::_1, qi::_r2),
						phoenix::bind(&indirect_object_handler, qi::_val, qi::_a, qi::_b)
					]
					> whitespaces
					> qi::lit("endobj")
				) | direct_object(qi::_r1)
				[
					phoenix::bind(&direct_object_offset_handler, qi::_1, qi::_r2)
				];

			direct_object %=
				dict_or_stream(qi::_r1)
				| array_object(qi::_r1)[phoenix::bind(&convert_array_to_obj, qi::_1, qi::_val)]
				| boolean_object(qi::_r1)[phoenix::bind(&convert_boolean_to_obj, qi::_1, qi::_val)]
				| function_object(qi::_r1)[phoenix::bind(&convert_function_to_obj, qi::_1, qi::_val)]
				| indirect_object_reference(qi::_r1)[phoenix::bind(&convert_reference_to_obj, qi::_1, qi::_val)]
				| real_object(qi::_r1)[phoenix::bind(&convert_real_to_obj, qi::_1, qi::_val)]
				| integer_object(qi::_r1)[phoenix::bind(&convert_integer_to_obj, qi::_1, qi::_val)]
				| name_object(qi::_r1)[phoenix::bind(&convert_name_to_obj, qi::_1, qi::_val)]
				| null_object(qi::_r1)[phoenix::bind(&convert_null_to_obj, qi::_1, qi::_val)]
				| string_object(qi::_r1)[phoenix::bind(&convert_string_to_obj, qi::_1, qi::_val)];

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
		}

		BooleanGrammar::BooleanGrammar() :
			base_type(start, "Boolean grammar")
		{
			start %=
				qi::eps
				>> qi::omit
				[
					qi::bool_
					[
						qi::_val = phoenix::construct<BooleanObjectPtr>(qi::_1),
						phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
					]
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ArrayGrammar::ArrayGrammar(ContainableGrammar& containable_object) :
			base_type(start, "Array grammar"),
			containable_object(containable_object)
		{
			start %=
				qi::lit('[')
				> whitespaces
				> *(
					containable_object(qi::_r1)
					>> whitespaces
					)
				> qi::lit(']')[phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		IndirectObjectReferenceGrammar::IndirectObjectReferenceGrammar() :
			base_type(start, "Indirect object reference grammar")
		{
			start %=
				qi::omit[qi::int_[qi::_a = qi::_1]]
				>> whitespace
				>> qi::omit[qi::ushort_[qi::_b = qi::_1]]
				>> whitespace
				>> qi::lit('R')
				[
					qi::_val = phoenix::construct<IndirectObjectReferencePtr>(qi::_a, qi::_b),
					phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		FunctionGrammar::FunctionGrammar() :
			base_type(start, "Function grammar")
		{
			//start %=
			//	qi::eps
			//	>> strict_float_parser;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		RealGrammar::RealGrammar() :
			base_type(start, "Real grammar")
		{
			start %=
				qi::eps
				>> qi::omit
				[
					strict_float_parser
					[
						qi::_val = phoenix::construct<RealObjectPtr>(qi::_1),
						phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
					]
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		IntegerGrammar::IntegerGrammar() :
			base_type(start, "Integer grammar")
		{
			start %=
				qi::eps
				>> qi::omit
				[
					qi::int_
					[
						qi::_val = phoenix::construct<IntegerObjectPtr>(qi::_1),
						phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
					]
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		NullGrammar::NullGrammar() :
			base_type(start, "Null grammar")
		{
			start %=
				qi::eps
				>> qi::lit("null")
				[
					qi::_val = NullObject::GetInstance(),
					phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		LiteralStringGrammar::LiteralStringGrammar() :
			base_type(start, "Literal string grammar")
		{
			start %=
				qi::lit("(")
				>> *(
					(qi::lit('\\') >> qi::char_)
					| qi::char_('(')[qi::_a += 1]
					| (qi::eps(qi::_a > 0) >> qi::char_(')')[qi::_a -= 1])
					| (qi::char_ - qi::char_('(') - qi::char_(')'))
					)
				> qi::lit(")")[phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		HexadecimalStringGrammar::HexadecimalStringGrammar() :
			base_type(start, "Hexadecimal string grammar")
		{
			start %=
				qi::lit('<')
				>> *(qi::char_("0-9a-fA-F"))
				> qi::lit('>')[phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		StringGrammar::StringGrammar() :
			base_type(start, "String grammar")
		{
			start %=
				literal_string(qi::_r1)[phoenix::bind(&literal_to_string, qi::_1, qi::_val)]
				| hexadecimal_string(qi::_r1)[phoenix::bind(&hexadecimal_to_string, qi::_1, qi::_val)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContainableGrammar::ContainableGrammar() :
			base_type(start, "Containable grammar")
		{
			start %=
				array_object(qi::_r1)[phoenix::bind(&convert_array, qi::_1, qi::_val)]
				| boolean_object(qi::_r1)[phoenix::bind(&convert_boolean, qi::_1, qi::_val)]
				| dictionary_object(qi::_r1)[phoenix::bind(&convert_dictionary, qi::_1, qi::_val)]
				| function_object(qi::_r1)[phoenix::bind(&convert_function, qi::_1, qi::_val)]
				| indirect_object_reference(qi::_r1)[phoenix::bind(&convert_reference, qi::_1, qi::_val)]
				| real_object(qi::_r1)[phoenix::bind(&convert_real, qi::_1, qi::_val)]
				| integer_object(qi::_r1)[phoenix::bind(&convert_integer, qi::_1, qi::_val)]
				| name_object(qi::_r1)[phoenix::bind(&convert_name, qi::_1, qi::_val)]
				| null_object(qi::_r1)[phoenix::bind(&convert_null, qi::_1, qi::_val)]
				| string_object(qi::_r1)[phoenix::bind(&convert_string, qi::_1, qi::_val)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		DictionaryGrammar::DictionaryGrammar(ContainableGrammar& containable_grammar) :
			base_type(start, "Dictionary grammar"),
			containable_object(containable_grammar)
		{
			start %=
				qi::lit("<<")
				> whitespaces
				> *(
					name_object(qi::_r1)
					>> whitespaces
					>> containable_object(qi::_r1)
					>> whitespaces
					)
				> qi::lit(">>")[phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		StreamDataGrammar::StreamDataGrammar() :
			base_type(start, "Stream data grammar")
		{
			start %=
				qi::lit("stream")[phoenix::bind(&dictionary_length, qi::_r2, qi::_a)]
				> -qi::lit('\r') > qi::lit('\n')
				> qi::omit[repo::qi::iter_offset[qi::_b = qi::_1]]
				> repo::qi::advance(qi::_a)
				> -eol
				> qi::lit("endstream")
				[
					qi::_val = phoenix::construct<StreamObjectPtr>(qi::_r2, qi::_b),
					phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
				];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		IndirectStreamGrammar::IndirectStreamGrammar() :
			base_type(start, "Indirect Stream grammar")
		{
			start %=
				qi::omit[qi::int_[qi::_a = qi::_1]]
				> whitespace
				> qi::omit[qi::ushort_[qi::_b = qi::_1]]
				> whitespace
				> qi::lit("obj")
				> whitespaces
				> qi::omit[dictionary_object(qi::_r1)[qi::_c = qi::_1]]
				> stream_data(qi::_r1, qi::_c)
				[
					phoenix::bind(&direct_object_offset_handler, qi::_val, qi::_r2),
					phoenix::bind(&indirect_object_handler, qi::_val, qi::_a, qi::_b)
				]
				> whitespaces
				> qi::lit("endobj");

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		DictionaryOrStreamGrammar::DictionaryOrStreamGrammar() :
			base_type(start, "Dictionary or stream grammar")
		{
			start %=
				qi::omit[dictionary_object(qi::_r1)[qi::_c = qi::_1]]
				>> (
					whitespaces
					>> stream_data(qi::_r1, qi::_c)[phoenix::bind(&convert_stream_to_obj, qi::_1, qi::_val)]
					| qi::eps[qi::_val = qi::_c]
				);

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		NameGrammar::NameGrammar() :
			base_type(start, "Name grammar")
		{
			start %=
				qi::lit('/')
				> *(
					(qi::char_ - whitespace - qi::char_("()<>[]{}/%"))
					//| (qi::char_('#') > qi::digit > qi::digit)
					)[phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
		}
	}
}
