#include "spirit_grammar.h"

#include "file.h"
#include "iter_offset_parser.h"

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_advance.hpp>

#define BOOST_SPIRIT_AUTO(domain_, name, expr)                                  \
	typedef boost::proto::result_of::                                           \
	deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type;                       \
	BOOST_SPIRIT_ASSERT_MATCH(                                                  \
	boost::spirit::domain_::domain, name##_expr_type);                          \
	BOOST_AUTO(name, boost::proto::deep_copy(expr));                            \

void indirect_reference_handler(gotchangpdf::IndirectObjectReferencePtr obj, gotchangpdf::files::File* file)
{
	obj->SetFile(file);
}

void dictionary_item_handler(gotchangpdf::DictionaryObjectPtr obj, gotchangpdf::DirectObject item)
{
	gotchangpdf::ContainableVisitor visitor;
	auto containable = item.apply_visitor(visitor);
	containable->SetContainer(obj);
}

void array_item_handler(gotchangpdf::MixedArrayObjectPtr obj, gotchangpdf::DirectObject item)
{
	gotchangpdf::ContainableVisitor visitor;
	auto containable = item.apply_visitor(visitor);
	containable->SetContainer(obj);
}

void stream_item_handler(gotchangpdf::DictionaryObjectPtr obj, gotchangpdf::types::stream_size& value)
{
	auto size_raw = obj->Find(gotchangpdf::constant::Name::Length);
	gotchangpdf::KillIndirectionVisitor<gotchangpdf::IntegerObjectPtr> visitor;
	gotchangpdf::IntegerObjectPtr size = size_raw.apply_visitor(visitor);

	value = static_cast<gotchangpdf::types::stream_size>(*size);
}
/*
std::ostream& operator<<(std::ostream& os, const boost::spirit::qi::rule<gotchangpdf::lexical::pos_iterator_type, gotchangpdf::IndirectObjectPtr(gotchangpdf::files::File*)> param)
{
	return os;
}
*/

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
		BOOST_SPIRIT_AUTO(qi, eol, -qi::lit('\r') >> qi::lit('\n'));

		DirectObjectGrammar::DirectObjectGrammar() :
			base_type(direct_object, "Direct object grammar")
		{
			boolean_object %=
				qi::eps
				>> qi::bool_;

			direct_object %=
				array_object(qi::_r1)
				| boolean_object
				| stream_object(qi::_r1)
				| dictionary_object(qi::_r1)
				| function_object
				| indirect_object_reference[phoenix::bind(&indirect_reference_handler, qi::_1, qi::_r1)]
				| real_object
				| integer_object
				| name_object
				| null_object
				| literal_string_object
				| hexadecimal_string_object;

			null_object %=
				qi::eps
				>> qi::lit("null")[qi::_val = NullObject::GetInstance()];

			indirect_object_reference %=
				integer_object
				>> whitespace
				>> integer_object
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
				direct_object(qi::_r1)[phoenix::bind(&array_item_handler, qi::_val, qi::_1)]
				>> whitespaces
				)
				> qi::lit(']');

			dictionary_object %=
				qi::lit("<<")
				> whitespaces
				> *(
				name_object
				>> whitespaces
				>> direct_object(qi::_r1)[phoenix::bind(&dictionary_item_handler, qi::_val, qi::_1)]
				>> whitespaces
				)
				> qi::lit(">>");

			stream_object =
				dictionary_object(qi::_r1)[qi::_a = qi::_1]
				>> whitespaces
				>> qi::lit("stream")[phoenix::bind(&stream_item_handler, qi::_a, qi::_b)]
				> eol
				> repo::qi::iter_offset
				> repo::qi::advance(qi::_b)
				> -eol
				> qi::lit("endstream");

			literal_string_object %=
				qi::lit("(")
				>> *(qi::char_ - qi::char_('()')) // TODO there can be balanced or escaped
				> qi::lit(")");

			BOOST_SPIRIT_DEBUG_NODE(boolean_object);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_object_reference);
			BOOST_SPIRIT_DEBUG_NODE(integer_object);
			BOOST_SPIRIT_DEBUG_NODE(name_object);
			BOOST_SPIRIT_DEBUG_NODE(array_object);
			BOOST_SPIRIT_DEBUG_NODE(dictionary_object);
			BOOST_SPIRIT_DEBUG_NODE(literal_string_object);
			//BOOST_SPIRIT_DEBUG_NODE(stream_object);
		}

		IndirectObjectGrammar::IndirectObjectGrammar() :
			base_type(indirect_object, "Indirect object grammar")
		{
			indirect_object %=
				object_number
				>> whitespace
				>> generation_number
				>> whitespace
				>> qi::lit("obj")
				>> whitespaces
				>> direct_object(qi::_r1)
				> whitespaces
				> qi::lit("endobj");

			object_number %=
				qi::eps
				>> qi::int_;

			generation_number %=
				qi::eps
				>> qi::ushort_;

			BOOST_SPIRIT_DEBUG_NODE(indirect_object);
		}
	}
}
