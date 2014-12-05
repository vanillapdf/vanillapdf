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

			template <>
			struct transform_attribute<gotchangpdf::lexical::base_iterator_type, gotchangpdf::types::stream_offset, qi::domain>
			{
				//typedef int type;
				static int pre(gotchangpdf::types::stream_offset& d) { return 0; }//not useful in this case but required to avoid compiler errors
				static void post(gotchangpdf::types::stream_offset& val, gotchangpdf::lexical::pos_iterator_type const& attr) //`val` is the "returned" string, `attr` is what int_ parses
				{
					val = 0;
				}
				static void fail(gotchangpdf::types::stream_offset&) {}
			};
		}
	}
}

#define BOOST_SPIRIT_AUTO(domain_, name, expr)                                  \
	typedef boost::proto::result_of::                                           \
	deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type;                   \
	BOOST_SPIRIT_ASSERT_MATCH(\
	boost::spirit::domain_::domain, name##_expr_type);                      \
	BOOST_AUTO(name, boost::proto::deep_copy(expr));                            \

void test(gotchangpdf::IndirectObjectReferencePtr obj, gotchangpdf::files::File* file)
{
	obj->SetFile(file);
}

typedef
boost::spirit::context<
boost::fusion::cons<gotchangpdf::IndirectObjectPtr&, boost::fusion::nil>,
boost::fusion::vector1<int>
> f_context;
void f(gotchangpdf::IntegerObjectPtr attribute, const f_context& con, bool& mFlag){
	std::cout << "matched integer: '" << attribute << "'" << std::endl
		<< "match flag: " << mFlag << std::endl;

	//assign output attribute from parsed value    
	auto test = qi::_a;
	auto bb = boost::phoenix::at_c<0>(con.locals);
	auto ee = boost::fusion::at_c<0>(con.locals);
}

void print(gotchangpdf::IndirectObjectReferencePtr const& obj)
{
	std::cout << "Obj: " << obj->GetObjectNumber()->Value() << "Gen: " << obj->GetGenerationNumber()->Value() << std::endl;
}

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;

		SpiritGrammar::SpiritGrammar(const lexical::SpiritLexer& lexer) :
			base_type(indirect_object, "Indirect object grammar")
		{
			//auto local_begin = qi::lazy(boost::phoenix::construct<qi::position>(qi::_a, qi::_b));
			//start %= indirect_object;

			BOOST_SPIRIT_AUTO(qi, whitespace, lexer.space
				| lexer.carriage_return
				| lexer.line_feed
				| lexer.form_feed
				| lexer.horizontal_tab
				| lexer.null);

			BOOST_SPIRIT_AUTO(qi, whitespaces, *whitespace);
			BOOST_SPIRIT_AUTO(qi, eol, -lexer.carriage_return >> lexer.line_feed);

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
				//>> qi::attr_cast(repo::qi::iter_pos)
				>> eol
				>> direct_object(qi::_r1);
				//>> eol
				//>> lexer.endobj;

			/*
			string_object =
				literal_string_object
				| hexadecimal_string_object;
				*/

			direct_object %=
				array_object(qi::_r1)
				//| boolean_object(qi::_r1)
				| stream_object(qi::_r1)
				| dictionary_object(qi::_r1)
				//| function_object(qi::_r1)
				| indirect_reference_object[boost::phoenix::bind(&test, qi::_1, qi::_r1)]
				| integer_object
				| name_object
				//| null_object(qi::_r1)
				//| real_object(qi::_r1)
				| literal_string_object;

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
				>> lexer.word;
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
				> *(direct_object(qi::_r1) > whitespaces)
				> whitespaces
				> lexer.right_bracket;

			dictionary_object %=
				lexer.dictionary_begin
				> whitespaces
				> *(name_object > whitespaces > direct_object(qi::_r1) > whitespaces)
				> whitespaces
				> lexer.dictionary_end;

			stream_object %=
				dictionary_object(qi::_r1)
				>> whitespaces
				>> lexer.stream_begin
				> eol;
				//> repo::qi::iter_pos;
				//> lexer.anything // skip length
				//> lexer.stream_end;

			literal_string_object %=
				qi::eps
				>> lexer.parenthesed_string;
			/*
				lexer.left_parenthesis
				> (lexer.word | lexer.literal_text)
				> lexer.right_parenthesis;
				*/
			BOOST_SPIRIT_DEBUG_NODE(boolean_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_object);
			BOOST_SPIRIT_DEBUG_NODE(direct_object);
			BOOST_SPIRIT_DEBUG_NODE(indirect_reference_object);
			BOOST_SPIRIT_DEBUG_NODE(integer_object);
			BOOST_SPIRIT_DEBUG_NODE(name_object);
			BOOST_SPIRIT_DEBUG_NODE(array_object);
			BOOST_SPIRIT_DEBUG_NODE(dictionary_object);
			BOOST_SPIRIT_DEBUG_NODE(literal_string_object);
		}
	}
}

