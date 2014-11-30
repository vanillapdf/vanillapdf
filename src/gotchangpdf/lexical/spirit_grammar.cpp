#include "spirit_grammar.h"

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>

//#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace boost {
	namespace spirit {
		namespace traits
		{
			template <>
			struct transform_attribute<gotchangpdf::IntegerObjectPtr, gotchangpdf::types::integer, qi::domain>
			{
				//typedef int type;
				static int pre(gotchangpdf::types::integer& d) { return 0; }//not useful in this case but required to avoid compiler errors
				static void post(gotchangpdf::types::integer& val, gotchangpdf::IntegerObjectPtr const& attr) //`val` is the "returned" string, `attr` is what int_ parses
				{
					val = attr->Value();
				}
				static void fail(gotchangpdf::types::integer&) {}
			};

			template <>
			struct transform_attribute<gotchangpdf::IntegerObjectPtr, gotchangpdf::types::ushort, qi::domain>
			{
				//typedef int type;
				static int pre(gotchangpdf::types::ushort& d) { return 0; }//not useful in this case but required to avoid compiler errors
				static void post(gotchangpdf::types::ushort& val, gotchangpdf::IntegerObjectPtr const& attr) //`val` is the "returned" string, `attr` is what int_ parses
				{
					val = attr->Value();
				}
				static void fail(gotchangpdf::types::ushort&) {}
			};
		}
	}
}

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;

		SpiritGrammar::SpiritGrammar(const lexical::SpiritLexer& lexer) :
			base_type(indirect_object, "Grammar")
		{
			//auto local_begin = qi::lazy(boost::phoenix::construct<qi::position>(qi::_a, qi::_b));
			//start %= qi::eps[qi::_a = qi::_r1, qi::_b = qi::_r2] >> indirect_object;
			//start %= qi::eps [qi::_a = qi::_r1] >> boolean_object;

			auto whitespace =
				lexer.space
				| lexer.carriage_return
				| lexer.line_feed
				| lexer.form_feed
				| lexer.horizontal_tab
				| lexer.null;

			auto whitespaces =
				*whitespace;

			auto eol =
				-lexer.carriage_return
				> lexer.line_feed;
			/*
			auto true_ = boost::spirit::qi::as<ast::True>()[(
				qi::eps
				>> lexer.true_
				)];

			auto false_ = boost::spirit::qi::as<ast::False>()[(
				qi::eps
				>> lexer.false_
				)];

			boolean_object =
				true_
				| false_;
				*/

			indirect_object %=
				integer_object
				>> lexer.space
				>> integer_object
				>> lexer.space
				>> lexer.obj
				>> eol
				>> direct_object
				>> eol
				>> lexer.endobj;

			/*
			string_object =
				literal_string_object
				| hexadecimal_string_object;
				*/

			direct_object =
				array_object
				//| boolean_object
				| dictionary_object
				//| function_object
				| indirect_reference_object
				| integer_object
				| name_object;
				//| null_object
				//| real_object
				//| stream_object
				//| string_object;

			indirect_reference_object %=
				integer_object
				>> lexer.space
				>> integer_object
				>> lexer.space
				>> lexer.indirect_reference_marker;

			integer_object %=
				qi::eps
				>> lexer.integer;
			/*
			real_object %=
				qi::eps
				>> lexer.float_;
				*/
			name_object %=
				qi::eps
				>> lexer.solidus
				>> lexer.regular_characters;
			/*
			name_object_dereferenced %=
				qi::eps
				>> lexer.solidus
				> *lexer.regular_character;
				*/
			// TODO
			/*
			hexadecimal_string_object %=
				lexer.less_than_sign
				>> lexer.anything
				>> lexer.greater_than_sign;
				*/

			array_object %=
				lexer.left_bracket
				> whitespaces
				> *(direct_object > whitespaces)
				> whitespaces
				> lexer.right_bracket;

			dictionary_object %=
				lexer.dictionary_begin
				> whitespaces
				> *(name_object > whitespaces > direct_object > whitespaces)
				> whitespaces
				> lexer.dictionary_end;

			/*
			stream_object %=
				dictionary_object
				>> lexer.stream_begin
				>> repo::qi::iter_pos;
				*/
			// TODO
			/*
			literal_string_object %=
				lexer.left_parenthesis
				>> lexer.anything
				>> lexer.right_parenthesis;
				*/

			BOOST_SPIRIT_DEBUG_NODE(boolean_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_object);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_reference_object);
			BOOST_SPIRIT_DEBUG_NODE(integer_object);
			BOOST_SPIRIT_DEBUG_NODE(name_object);
			BOOST_SPIRIT_DEBUG_NODE(array_object);
			BOOST_SPIRIT_DEBUG_NODE(dictionary_object);
		}
	}
}

