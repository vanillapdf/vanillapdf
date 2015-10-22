#ifndef _XREF_GRAMMAR_H
#define _XREF_GRAMMAR_H

#include "xref.h"
#include "spirit_grammar.h"
#include "qi_common.h"
#include "whitespace_grammar.h"

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		namespace qi = boost::spirit::qi;

		class XrefTableSubsectionsGrammar : public qi::grammar<pos_iterator_type,
			files::Xref(files::File*, types::stream_offset)>
		{
		public:
			XrefTableSubsectionsGrammar();

		private:
			qi::rule<pos_iterator_type, files::Xref(files::File*, types::stream_offset)> start;
			qi::rule<pos_iterator_type, XrefSubsectionPtr(files::File*), qi::locals<types::integer, types::integer, types::integer>> subsection;
			qi::rule<pos_iterator_type, files::XrefEntry(types::integer), qi::locals<types::integer, types::ushort>> entry;

			qi::int_parser<types::integer, 10U, 10, 10> offset_parser;
			qi::int_parser<types::ushort, 10U, 5, 5> gen_parser;

			ContainableGrammar containable_object;
			DictionaryGrammar dictionary_object = { containable_object };
			IndirectStreamGrammar stream_object;

			EndOfLine eol;
			Whitespace whitespaces;
		};
	}
}

#endif /* _XREF_GRAMMAR_H */
