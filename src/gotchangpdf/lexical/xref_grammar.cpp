#include "precompiled.h"
#include "xref_grammar.h"

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

void add_section(files::Xref xref, const XrefSubsectionPtr& section)
{
	files::XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->Add(section);
}

void add_entry(XrefSubsectionPtr section, const files::XrefEntry& entry)
{
	section->Add(entry);
}

void set_file_to_entry(files::XrefEntry& entry, files::File* file)
{
	files::XrefEntryBaseVisitor visitor;
	auto base = entry.apply_visitor(visitor);
	base->SetFile(file);
}

void set_file_to_subsection(XrefSubsectionPtr& section, files::File* file)
{
	section->SetFile(file);
}

void set_file_to_xref(files::Xref& xref, files::File* file)
{
	files::XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->SetFile(file);
}

void set_trailer(files::Xref& xref, DirectObject obj)
{
	files::XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);

	ObjectVisitor<DictionaryObjectPtr> dict_visitor;
	auto dict = obj.apply_visitor(dict_visitor);

	base->SetTrailerDictionary(dict);
}

void set_last_offset(files::Xref& xref, types::integer offset)
{
	files::XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->SetLastXrefOffset(offset);
}

namespace gotchangpdf
{
	namespace lexical
	{
		namespace repo = boost::spirit::repository;
		namespace ascii = boost::spirit::ascii;
		namespace phoenix = boost::phoenix;

		BOOST_SPIRIT_AUTO(qi, eol2, (-qi::lit('\r') >> qi::lit('\n')) | (qi::lit('\r') >> -qi::lit('\n')));
		BOOST_SPIRIT_AUTO(qi, whitespace2, qi::omit[qi::char_(" \r\n\f\t") | qi::char_('\0')]);
		BOOST_SPIRIT_AUTO(qi, whitespaces2, *whitespace2);

		XrefTableSubsectionsGrammar::XrefTableSubsectionsGrammar() :
			base_type(start, "Xref table subsections grammar")
		{
			start %=
				qi::lit("xref")[qi::_val = phoenix::construct<XrefTablePtr>(files::XrefTable()), phoenix::bind(&set_file_to_xref, qi::_r0, qi::_r1)]
				> whitespaces2
				> qi::omit[*(subsection(qi::_r1)[phoenix::bind(&add_section, qi::_r0, qi::_1), phoenix::bind(&set_file_to_subsection, qi::_1, qi::_r1)])]
				> qi::lit("trailer")
				> eol2
				> qi::omit[direct_object(qi::_r1, 0)[phoenix::bind(&set_trailer, qi::_r0, qi::_1)]]
				> eol2
				> qi::lit("startxref")
				> eol2
				> qi::omit[qi::int_[phoenix::bind(&set_last_offset, qi::_r0, qi::_1)]]
				> eol2
				> qi::lit("%%EOF");

			subsection %=
				qi::eps
				>> qi::omit
				[
					(qi::int_ > qi::lit(' ') > qi::int_ > whitespaces2)
					[
						qi::_a = qi::_1, qi::_b = qi::_2, qi::_c = 0, qi::_val = phoenix::construct<XrefSubsectionPtr>(qi::_1, qi::_2)
					]
					> qi::repeat(qi::_b)
					[
						entry(qi::_a + qi::_c)[++qi::_c, phoenix::bind(&add_entry, qi::_val, qi::_1), phoenix::bind(&set_file_to_entry, qi::_1, qi::_r1)]
					]
				];

			entry %=
				qi::omit[offset_parser[qi::_a = qi::_1]]
				> qi::lit(' ')
				> qi::omit[gen_parser[qi::_b = qi::_1]]
				> qi::lit(' ')
				> (
					qi::lit('n')[qi::_val = phoenix::construct<XrefUsedEntryPtr>(qi::_r1, qi::_b, qi::_a)]
					|
					qi::lit('f')[qi::_val = phoenix::construct<XrefFreeEntryPtr>(qi::_r1, qi::_b, qi::_a)]
				)
				> (qi::lit("\r\n") | qi::lit(" \n") | qi::lit(" \r"));

			BOOST_SPIRIT_DEBUG_NODE(start);
			BOOST_SPIRIT_DEBUG_NODE(subsection);
			BOOST_SPIRIT_DEBUG_NODE(entry);
		}
	}
}
