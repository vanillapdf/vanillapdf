#include "precompiled.h"
#include "spirit_parser.h"

#include "xref.h"
#include "exception.h"
#include "file.h"
#include "log.h"

#include "spirit_grammar.h"
#include "xref_grammar.h"
#include "reverse_grammar.h"
#include "object_stream_grammar.h"
#include "content_stream_grammar.h"

#include <iomanip>

#include <boost/spirit/include/qi.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		namespace qi = boost::spirit::qi;

		class SpiritParser::Impl
		{
		public:
			Impl(std::weak_ptr<File> file) : _file(file) {}

			template <typename Result, typename Grammar, typename Iterator>
			Result Read(Grammar& grammar, Iterator& input_begin_pos, Iterator& input_end_pos);

			std::weak_ptr<File> _file;
			XrefTableSubsectionsGrammar _xref_grammar;
			contents::ContentStreamGrammar _content_stream_grammar;
			ObjectStreamGrammar _obj_stream_grammar;
			DirectObjectGrammar _direct_grammar;
			ReverseGrammar _reverse_grammar;
		};

		SpiritParser::SpiritParser(std::weak_ptr<File> file, CharacterSource & stream)
			: Stream(stream), _impl(pdf_new Impl(file)) {}

		SpiritParser::SpiritParser(const SpiritParser & other)
			: Stream(other) { _impl->_file = other._impl->_file; }

		std::weak_ptr<File> SpiritParser::file(void) const { return _impl->_file; }

		template <typename Result, typename Grammar, typename Iterator>
		Result SpiritParser::Impl::Read(Grammar& grammar, Iterator& input_begin_pos, Iterator& input_end_pos)
		{
			try {
				Result obj;
				auto result = qi::parse(input_begin_pos, input_end_pos, grammar, obj);
				if (result) {
					return obj;
				}

				LOG_ERROR << "Parsing failed";

				const auto& pos = input_begin_pos.get_position();
				LOG_ERROR <<
					"Error at offset " << pos.offset << endl <<
					"'" << input_begin_pos.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here";

				throw ParseException(pos.offset);
			}
			catch (const qi::expectation_failure<pos_iterator_type>& exception) {
				LOG_ERROR << "Parsing failed";

				auto pos_begin = exception.first;
				const auto& pos = pos_begin.get_position();
				LOG_ERROR <<
					"Error at offset " << pos.offset << endl <<
					"Expecting " << exception.what() << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here";

				throw ParseException(pos.offset);
			}
		}

		XrefBasePtr SpiritParser::ReadXref(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadXref();
		}

		XrefBasePtr SpiritParser::ReadXref(void)
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			auto locked_file = _impl->_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			types::stream_offset offset = tellg();
			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, locked_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_xref_grammar(&locked_file, offset);
			return _impl->Read<XrefBasePtr>(gram, input_begin_pos, input_end_pos);
		}

		contents::GenericOperationCollection SpiritParser::ReadContentStreamOperations(void)
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			auto locked_file = _impl->_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			types::stream_offset offset = tellg();
			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, locked_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_content_stream_grammar(&locked_file);
			return _impl->Read<contents::GenericOperationCollection>(gram, input_begin_pos, input_end_pos);
		}

		std::vector<ObjectPtr> SpiritParser::ReadObjectStreamEntries(types::integer first, types::integer size)
		{
			std::vector<ObjectPtr> result;
			auto headers = ReadObjectStreamHeaders(size);
			for (auto header : headers) {
				seekg(first + header.offset);
				auto obj = ReadDirectObject();
				obj->SetObjectNumber(header.object_number);

				result.push_back(obj);
			}

			return result;
		}

		types::integer SpiritParser::ReadLastXrefOffset()
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			auto locked_file = _impl->_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			types::stream_offset offset = tellg();
			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, locked_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_reverse_grammar;
			return _impl->Read<types::integer>(gram, input_begin_pos, input_end_pos);
		}

		std::vector<ObjectStreamHeader> SpiritParser::ReadObjectStreamHeaders(types::integer size)
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			auto locked_file = _impl->_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			types::stream_offset offset = tellg();
			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, locked_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_obj_stream_grammar(size);
			return _impl->Read<std::vector<ObjectStreamHeader>>(gram, input_begin_pos, input_end_pos);
		}

		ObjectPtr SpiritParser::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, std::ios::beg);
			return ReadDirectObject();
		}

		ObjectPtr SpiritParser::ReadDirectObject(void)
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			auto locked_file = _impl->_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			types::stream_offset offset = tellg();
			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, locked_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_direct_grammar(&locked_file, offset);
			return _impl->Read<ObjectPtr>(gram, input_begin_pos, input_end_pos);
		}
	}
}
