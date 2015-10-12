#include "precompiled.h"
#include "spirit_grammar.h"

#include "file.h"
#include "iter_offset_parser.h"
#include "abstract_syntax_tree.h"

#pragma warning (push, 3)
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <boost/spirit/repository/include/qi_advance.hpp>
#pragma warning (pop)

using namespace gotchangpdf;
using namespace gotchangpdf::lexical;

void direct_object_file_handler(DirectObject obj, files::File* file)
{
	assert(nullptr != file);

	ObjectBaseVisitor visitor;
	auto base = obj.apply_visitor(visitor);
	base->SetFile(file);
}

void direct_object_offset_handler(DirectObject obj, types::stream_offset offset)
{
	ObjectBaseVisitor visitor;
	auto base = obj.apply_visitor(visitor);
	base->SetOffset(offset);
}

void indirect_object_handler(DirectObject obj, types::integer obj_number, types::ushort gen_number)
{
	ObjectBaseVisitor visitor;
	auto base = obj.apply_visitor(visitor);
	base->SetObjectNumber(obj_number);
	base->SetGenerationNumber(gen_number);
}

void dictionary_item_handler(DictionaryObjectPtr obj, ContainableObject item)
{
	SetContainerVisitor visitor(obj);
	item.apply_visitor(visitor);
}

void array_item_handler(MixedArrayObjectPtr obj, ContainableObject item)
{
	SetContainerVisitor visitor(obj);
	item.apply_visitor(visitor);
}

void dictionary_length(const DictionaryObjectPtr& obj, types::stream_size& value)
{
	auto size = obj->FindAs<gotchangpdf::IntegerObjectPtr>(constant::Name::Length);
	value = static_cast<types::stream_size>(*size);
}

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;

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
				dict_or_stream(qi::_r1)[phoenix::bind(&direct_object_file_handler, qi::_1, qi::_r1)]
				|
				(
				array_object(qi::_r1)
				| boolean_object
				| function_object
				| indirect_object_reference
				| real_object
				| integer_object
				| name_object
				| null_object
				| literal_string_object
				| hexadecimal_string_object
				)[phoenix::bind(&direct_object_file_handler, qi::_1, qi::_r1)];

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
		}

		BooleanGrammar::BooleanGrammar() :
			base_type(start, "Boolean grammar")
		{
			start %=
				qi::eps
				>> qi::bool_;

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
					containable_object(qi::_r1)[phoenix::bind(&array_item_handler, qi::_val, qi::_1)]
					>> whitespaces
					)
				> qi::lit(']');

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
				>> qi::lit('R')[qi::_val = phoenix::construct<IndirectObjectReferencePtr>(qi::_a, qi::_b)];

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
				>> strict_float_parser;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		IntegerGrammar::IntegerGrammar() :
			base_type(start, "Integer grammar")
		{
			start %=
				qi::eps
				>> qi::int_;

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		NullGrammar::NullGrammar() :
			base_type(start, "Null grammar")
		{
			start %=
				qi::eps
				>> qi::lit("null")[qi::_val = NullObject::GetInstance()];

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
				> qi::lit(")");

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		HexadecimalStringGrammar::HexadecimalStringGrammar() :
			base_type(start, "Hexadecimal string grammar")
		{
			start %=
				qi::lit('<')
				>> *(qi::char_("0-9a-fA-F"))
				> qi::lit('>');

			BOOST_SPIRIT_DEBUG_NODE(start);
		}

		ContainableGrammar::ContainableGrammar() :
			base_type(start, "Containable grammar")
		{
			start %=
				(
					array_object(qi::_r1)
					| boolean_object
					| dictionary_object(qi::_r1)
					| function_object
					| indirect_object_reference
					| real_object
					| integer_object
					| name_object
					| null_object
					| literal_string_object
					| hexadecimal_string_object
					)[phoenix::bind(&direct_object_file_handler, qi::_1, qi::_r1)];

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
					name_object[phoenix::bind(&direct_object_file_handler, qi::_1, qi::_r1)]
					>> whitespaces
					>> containable_object(qi::_r1)[phoenix::bind(&dictionary_item_handler, qi::_val, qi::_1)]
					>> whitespaces
					)
				> qi::lit(">>");

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
				> qi::lit("endstream")[qi::_val = phoenix::construct<StreamObjectPtr>(qi::_r2, qi::_b)];

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
					phoenix::bind(&direct_object_file_handler, qi::_c, qi::_r1),
					phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1),
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
					>> stream_data(qi::_r1, qi::_c)
					[
						phoenix::bind(&direct_object_file_handler, qi::_c, qi::_r1),
						phoenix::bind(&direct_object_file_handler, qi::_val, qi::_r1)
					]
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
					);

			BOOST_SPIRIT_DEBUG_NODE(start);
		}
	}
}
