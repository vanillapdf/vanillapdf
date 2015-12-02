#include "precompiled.h"
#include "xref_grammar.h"

#include "iter_offset_parser.h"
#include "abstract_syntax_tree.h"
#include "object_visitors.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

void add_section(Xref xref, const XrefSubsectionPtr& section)
{
	XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->Add(section);
}

void add_entry(XrefSubsectionPtr section, const XrefEntry& entry)
{
	section->Add(entry);
}

void set_file_to_entry(XrefEntry& entry, std::shared_ptr<File>* file)
{
	assert(nullptr != file && *file);

	XrefEntryBaseVisitor visitor;
	auto base = entry.apply_visitor(visitor);
	base->SetFile(*file);
}

void set_file_to_subsection(XrefSubsectionPtr& section, std::shared_ptr<File>* file)
{
	assert(nullptr != file && *file);
	section->SetFile(*file);
}

void set_file_to_xref(Xref& xref, std::shared_ptr<File>* file)
{
	assert(nullptr != file && *file);

	XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->SetFile(*file);
}

void set_trailer(Xref& xref, DictionaryObjectPtr& dict)
{
	XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);

	base->SetTrailerDictionary(dict);
}

void set_last_offset(Xref& xref, types::integer offset)
{
	XrefBaseVisitor visitor;
	auto base = xref.apply_visitor(visitor);
	base->SetLastXrefOffset(offset);
}

void read_xref_stream_data(Xref xref, StreamObjectPtr stream, std::shared_ptr<File>* file)
{
	assert(nullptr != file && *file);

	XrefBaseVisitor visitor;
	auto xref_base = xref.apply_visitor(visitor);

	// Get stream object data
	auto header = stream->GetHeader();

	auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);
	auto size = header->FindAs<IntegerObjectPtr>(constant::Name::Size);

	auto field_size = fields->Size();
	assert(field_size == 3);

	ArrayObjectPtr<IntegerObjectPtr> index = { IntegerObjectPtr(0), size };
	if (header->Contains(constant::Name::Index)) {
		index = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::Index);
	}

	auto index_size = index->Size();
	assert(index_size % 2 == 0);

	auto body = stream->GetBodyDecoded();

	auto field1_size = fields->At(0);
	auto field2_size = fields->At(1);
	auto field3_size = fields->At(2);

	// Iterate over entries
	auto it = body.begin();
	for (auto i = 0; i < index_size; i += 2) {

		auto subsection_index = index->At(i);
		auto subsection_size = index->At(i + 1);

		XrefSubsectionPtr subsection(subsection_index->Value(), subsection_size->Value());
		for (auto idx = 0; idx < *subsection_size; idx++) {

			IntegerObject field1;
			for (int j = 0; j < *field1_size; ++j) {
				field1 = (field1 << 8) + (*it & 0xff);
				it++;
			}

			assert(field1 == 0 || field1 == 1 || field1 == 2);

			IntegerObject field2;
			for (int j = 0; j < *field2_size; ++j) {
				field2 = (field2 << 8) + (*it & 0xff);
				it++;
			}

			IntegerObject field3;
			for (int j = 0; j < *field3_size; ++j) {
				field3 = (field3 << 8) + (*it & 0xff);
				it++;
			}

			switch (field1) {
			case 0:
			{
				XrefFreeEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
				entry->SetFile(*file);
				subsection->Add(entry);
				break;
			}
			case 1:
			{
				XrefUsedEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
				entry->SetFile(*file);
				subsection->Add(entry);
				break;
			}
			case 2:
			{
				XrefCompressedEntryPtr entry(*subsection_index + idx, static_cast<types::ushort>(0), field2, field3);
				entry->SetFile(*file);
				subsection->Add(entry);
				break;
			}
			default:
				throw Exception("Unknown field in cross reference stream");
			}
		}

		subsection->SetFile(*file);
		xref_base->Add(subsection);
	}

	xref_base->SetFile(*file);
	xref_base->SetTrailerDictionary(header);
}

namespace gotchangpdf
{
	namespace syntax
	{
		namespace phoenix = boost::phoenix;

		XrefTableSubsectionsGrammar::XrefTableSubsectionsGrammar() :
			base_type(start, "Xref table subsections grammar")
		{
			start %=
				qi::omit[stream_object(qi::_r1, qi::_r2)
				[
					qi::_val = phoenix::construct<XrefStreamPtr>(),
					phoenix::bind(&set_file_to_xref, qi::_val, qi::_r1),
					phoenix::bind(&read_xref_stream_data, qi::_val, qi::_1, qi::_r1)
				]]
				|
				(
					qi::lit("xref")[qi::_val = phoenix::construct<XrefTablePtr>(), phoenix::bind(&set_file_to_xref, qi::_val, qi::_r1)]
					> whitespaces
					> qi::omit[*(subsection(qi::_r1)[phoenix::bind(&add_section, qi::_r0, qi::_1), phoenix::bind(&set_file_to_subsection, qi::_1, qi::_r1)])]
					> qi::lit("trailer")
					> eol
					> qi::omit[dictionary_object(qi::_r1)[phoenix::bind(&set_trailer, qi::_r0, qi::_1)]]
					> eol
					> qi::lit("startxref")
					> eol
					> qi::omit[qi::int_[phoenix::bind(&set_last_offset, qi::_r0, qi::_1)]]
					> eol
					> qi::lit("%%EOF")
				);

			subsection %=
				qi::eps
				>> qi::omit
				[
					(qi::int_ > qi::lit(' ') > qi::int_ > whitespaces)
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
