#include "spirit_parser.h"

#include "exception.h"
#include "file.h"
#include "indirect_object.h"
#include "log.h"

#include "spirit_grammar.h"

#include <iomanip>
//#include <boost/variant/get.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace std;
		using namespace exceptions;

		namespace qi = boost::spirit::qi;

		class SpiritParser::Impl
		{
		public:
			Impl(files::File * file) : _file(file) {}

			files::File * _file;
			IndirectObjectGrammar _indirect_grammar;
			DirectObjectGrammar _direct_grammar;
		};

		SpiritParser::SpiritParser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _impl(new Impl(file)) {}

		SpiritParser::SpiritParser(const SpiritParser & other)
			: lexical::Stream(other) { _impl->_file = other._impl->_file; }

		files::File * SpiritParser::file(void) const { return _impl->_file; }

		DirectObject SpiritParser::ReadDirectObject(types::stream_offset offset)
		{
			DirectObject obj;

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			seekg(offset, std::ios::beg);
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			try
			{
				auto result = qi::parse(input_begin_pos, input_end_pos, _impl->_direct_grammar(_impl->_file), obj);
				if (result) {
					return obj;
				}
				else {
					LOG_ERROR << "Parsing failed" << endl;

					const auto& pos = input_begin_pos.get_position();
					LOG_ERROR <<
						"Error at offset " << pos.offset << endl <<
						"'" << input_begin_pos.get_currentline() << "'" << endl <<
						setw(pos.column + 7) << " ^- here" << endl;

					throw exceptions::Exception("Parsing failed");
				}
			}
			catch (const qi::expectation_failure<pos_iterator_type>& exception) {
				LOG_ERROR << "Parsing failed" << endl;

				auto pos_begin = exception.first;
				const auto& pos = pos_begin.get_position();
				LOG_ERROR <<
					"Error at offset " << pos.offset << endl <<
					"Expecting " << exception.what() << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here" << endl;

				throw;
			}
		}

		IndirectObjectPtr SpiritParser::ReadIndirectObject(types::stream_offset offset)
		{
			IndirectObjectPtr obj = IndirectObject(_impl->_file);
			obj->SetOffset(offset);

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			seekg(offset, std::ios::beg);
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			try
			{
				auto result = qi::parse(input_begin_pos, input_end_pos, _impl->_indirect_grammar(_impl->_file), obj);
				if (result) {
					return obj;
				}
				else {
					//auto offset = tellg();
					cout << "Parsing failed" << endl;
					const auto& pos = input_begin_pos.get_position();
					cout <<
						"Error at file " << pos.file << " offset " << pos.offset << endl <<
						"'" << input_begin_pos.get_currentline() << "'" << endl <<
						setw(pos.column + 7) << " ^- here" << endl;

					throw exceptions::Exception("Parsing failed");
				}
			}
			catch (const qi::expectation_failure<pos_iterator_type>& exception) {
				cout << "Parsing failed" << endl;

				auto pos_begin = exception.first;
				const auto& pos = pos_begin.get_position();
				cout <<
					"Error at file " << pos.file << " offset " << pos.offset << " Expecting " << exception.what_ << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here" << endl;

				throw;
			}
		}
	}
}
