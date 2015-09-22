#ifndef _XREF_GRAMMAR_H
#define _XREF_GRAMMAR_H

#include "file_position.h"
#include "offset_iterator.h"
#include "xref.h"
#include "spirit_grammar.h"

#pragma warning (push, 3)
#include <boost/spirit/include/qi.hpp>
#pragma warning (pop)

namespace gotchangpdf
{
	namespace lexical
	{
		typedef boost::spirit::istream_iterator base_iterator_type;
		typedef offset_iterator<base_iterator_type, pdf_position> pos_iterator_type;

		namespace qi = boost::spirit::qi;

		class XrefTableSubsectionsGrammar : public qi::grammar<pos_iterator_type,
			files::Xref(files::File*)>
		{
		public:
			XrefTableSubsectionsGrammar();

		private:
			template <typename A, typename... Inherited>
			using Rule = qi::rule<pos_iterator_type, A(Inherited...)>;

			qi::rule<pos_iterator_type, files::Xref(files::File*)> start;
			qi::rule<pos_iterator_type, XrefSubsectionPtr(files::File*), qi::locals<types::integer, types::integer, types::integer>> subsection;
			qi::rule<pos_iterator_type, files::XrefEntry(types::integer), qi::locals<types::integer, types::ushort>> entry;

			qi::int_parser<types::integer, 10U, 10, 10> offset_parser;
			qi::int_parser<types::ushort, 10U, 5, 5> gen_parser;

			DirectObjectGrammar direct_object;
		};
	}
}

#endif /* _XREF_GRAMMAR_H */
