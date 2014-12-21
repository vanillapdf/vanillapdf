#include "spirit_grammar.h"

#include "file.h"
#include "iter_offset_parser.h"

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
#include <boost/spirit/repository/include/qi_advance.hpp>

#define BOOST_SPIRIT_AUTO(domain_, name, expr)                                  \
	typedef boost::proto::result_of::                                           \
	deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type;                       \
	BOOST_SPIRIT_ASSERT_MATCH(                                                  \
	boost::spirit::domain_::domain, name##_expr_type);                          \
	BOOST_AUTO(name, boost::proto::deep_copy(expr));                            \

gotchangpdf::IndirectObjectReferencePtr indirect_reference_handler(gotchangpdf::files::File* file, gotchangpdf::IntegerObjectPtr obj, gotchangpdf::IntegerObjectPtr gen)
{
	return gotchangpdf::IndirectObjectReference(file, obj, gen);
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

void stream_item_handler(gotchangpdf::DictionaryObjectPtr obj, int& value)
{
	auto size_raw = obj->Find(gotchangpdf::constant::Name::Length);
	gotchangpdf::KillIndirectionVisitor<gotchangpdf::IntegerObjectPtr> visitor;
	gotchangpdf::IntegerObjectPtr size = size_raw.apply_visitor(visitor);

	value = static_cast<int>(*size);
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

		SpiritGrammar::SpiritGrammar() :
			base_type(indirect_object, "Indirect object grammar")
		{
			BOOST_SPIRIT_AUTO(qi, whitespace, qi::omit[qi::char_(" \r\n\f\t") | qi::char_('\0')]);
			BOOST_SPIRIT_AUTO(qi, whitespaces, *whitespace);
			BOOST_SPIRIT_AUTO(qi, eol, -qi::lit('\r') >> qi::lit('\n'));
			BOOST_SPIRIT_AUTO(qi, name, qi::lit('/') > *qi::char_("0-9a-zA-Z+,"));

			boolean_object %=
				qi::eps
				>> qi::bool_;

			indirect_object %=
				integer_object
				>> whitespace
				>> integer_object
				>> whitespace
				>> qi::lit("obj")
				>> repo::qi::iter_offset
				>> whitespaces
				>> direct_object(qi::_r1)
				>> whitespaces
				>> qi::lit("endobj");

			direct_object %=
				array_object(qi::_r1)
				| boolean_object
				| stream_object(qi::_r1)
				| dictionary_object(qi::_r1)
				| function_object
				| indirect_reference_object(qi::_r1)//[boost::phoenix::bind(&indirect_reference_handler, qi::_1, qi::_r1)]
				| integer_object
				| name_object
				| null_object
				| real_object
				| literal_string_object
				| hexadecimal_string_object;

			null_object %=
				qi::eps
				>> qi::lit("null")[qi::_val = NullObject::GetInstance()];

			indirect_reference_object %=
				integer_object[qi::_a = qi::_1]
				>> whitespace
				>> integer_object[qi::_b = qi::_1]
				>> whitespace
				>> qi::lit('R')[qi::_val = boost::phoenix::bind(&indirect_reference_handler, qi::_r1, qi::_a, qi::_b)];

			integer_object %=
				qi::eps
				>> qi::int_;

			real_object %=
				qi::eps
				>> qi::float_;

			name_object %=
				name;

			name_key %=
				name;

			hexadecimal_string_object %=
				qi::lit('<')
				>> *(qi::digit)
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
					name_key
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
			BOOST_SPIRIT_DEBUG_NODE(indirect_object);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_reference_object);
			BOOST_SPIRIT_DEBUG_NODE(integer_object);
			BOOST_SPIRIT_DEBUG_NODE(name_object);
			BOOST_SPIRIT_DEBUG_NODE(name_key);
			BOOST_SPIRIT_DEBUG_NODE(array_object);
			BOOST_SPIRIT_DEBUG_NODE(dictionary_object);
			BOOST_SPIRIT_DEBUG_NODE(literal_string_object);
			//BOOST_SPIRIT_DEBUG_NODE(stream_object);
		}
	}
}

