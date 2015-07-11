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

#define BOOST_SPIRIT_AUTO(domain_, name, expr)                                  \
	typedef boost::proto::result_of::                                           \
	deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type;                       \
	BOOST_SPIRIT_ASSERT_MATCH(                                                  \
	boost::spirit::domain_::domain, name##_expr_type);                          \
	BOOST_AUTO(name, boost::proto::deep_copy(expr));                            \

using namespace gotchangpdf;

void direct_object_file_handler(DirectObject obj, files::File* file)
{
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

void dictionary_item_handler(const DictionaryObjectPtr obj, ContainableObject item)
{
	SetContainerVisitor visitor(obj);
	item.apply_visitor(visitor);
}

void array_item_handler(const MixedArrayObjectPtr obj, ContainableObject item)
{
	SetContainerVisitor visitor(obj);
	item.apply_visitor(visitor);
}

void stream_item_handler(const DictionaryObjectPtr& obj, types::stream_size& value)
{
	auto size_raw = obj->Find(constant::Name::Length);
	KillIndirectionVisitor<gotchangpdf::IntegerObjectPtr> visitor;
	IntegerObjectPtr size = size_raw.apply_visitor(visitor);

	value = static_cast<types::stream_size>(*size);
}

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;

		BOOST_SPIRIT_AUTO(qi, delimiter, qi::omit[qi::char_("()<>[]{}/%")]);
		BOOST_SPIRIT_AUTO(qi, whitespace, qi::omit[qi::char_(" \r\n\f\t") | qi::char_('\0')]);
		BOOST_SPIRIT_AUTO(qi, whitespaces, *whitespace);
		BOOST_SPIRIT_AUTO(qi, eol, (-qi::lit('\r') >> qi::lit('\n')) | (qi::lit('\r') >> -qi::lit('\n')));
		BOOST_SPIRIT_AUTO(qi, optcrlf, -qi::lit('\r') >> qi::lit('\n'));

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

			boolean_object %=
				qi::eps
				>> qi::bool_;

			direct_object %=
				(
				array_object(qi::_r1)
				| boolean_object
				| stream_object(qi::_r1)
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

			containable_object %=
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

			null_object %=
				qi::eps
				>> qi::lit("null")[qi::_val = NullObject::GetInstance()];

			indirect_object_reference %=
				qi::int_
				>> whitespace
				>> qi::ushort_
				>> whitespace
				>> qi::lit('R');

			integer_object %=
				qi::eps
				>> qi::int_;

			real_object %=
				qi::eps
				//>> qi::float_
				>> strict_float_parser;

			name_object %=
				qi::lit('/')
				> *(
					(qi::char_ - whitespace - delimiter)
					//| (qi::char_('%') > qi::digit > qi::digit)
				);

			hexadecimal_string_object %=
				qi::lit('<')
				>> *(qi::char_("0-9a-fA-F"))
				> qi::lit('>');

			array_object %=
				qi::lit('[')
				> whitespaces
				> *(
				containable_object(qi::_r1)[phoenix::bind(&array_item_handler, qi::_val, qi::_1)]
				>> whitespaces
				)
				> qi::lit(']');

			dictionary_object %=
				qi::lit("<<")
				> whitespaces
				> *(
				name_object
				>> whitespaces
				>> containable_object(qi::_r1)[phoenix::bind(&dictionary_item_handler, qi::_val, qi::_1)]
				>> whitespaces
				)
				> qi::lit(">>");

			dictionary_object_raw %=
				qi::lit("<<")
				> whitespaces
				> *(
				name_object
				>> whitespaces
				>> containable_object(qi::_r1)[phoenix::bind(&dictionary_item_handler, qi::_val, qi::_1)]
				>> whitespaces
				)
				> qi::lit(">>");

			stream_object %=
				dictionary_object_raw(qi::_r1)[qi::_a = qi::_1]
				>> whitespaces
				>> qi::lit("stream")[phoenix::bind(&stream_item_handler, qi::_a, qi::_b)]
				> optcrlf
				> repo::qi::iter_offset
				> repo::qi::advance(qi::_b)
				> -eol
				> qi::lit("endstream");

			literal_string_object %=
				qi::lit("(")
				>> *(qi::char_ - qi::char_('()')) // TODO there can be balanced or escaped
				> qi::lit(")");

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(boolean_object);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_object_reference);
			BOOST_SPIRIT_DEBUG_NODE(integer_object);
			BOOST_SPIRIT_DEBUG_NODE(name_object);
			BOOST_SPIRIT_DEBUG_NODE(array_object);
			BOOST_SPIRIT_DEBUG_NODE(dictionary_object);
			BOOST_SPIRIT_DEBUG_NODE(literal_string_object);
			BOOST_SPIRIT_DEBUG_NODE(stream_object);
		}

		ObjectStreamGrammar::ObjectStreamGrammar() :
			base_type(start, "Direct object grammar")
		{
			start %=
				qi::repeat(qi::_r1)[header];

			header %=
				qi::int_
				> whitespaces
				> qi::long_long
				> whitespaces;

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(header);
		}
	}
}
