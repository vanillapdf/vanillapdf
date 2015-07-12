#include "precompiled.h"
#include "spirit_parser.h"

#include "xref.h"
#include "character.h"
#include "exception.h"
#include "file.h"
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
		using namespace character;

		namespace qi = boost::spirit::qi;

		class SpiritParser::Impl
		{
		public:
			Impl(files::File * file) : _file(file) {}

			XrefEntryPtr ReadTableEntry(lexical::SpiritParser& s, types::integer objNumber);

			files::File * _file;
			ObjectStreamGrammar _obj_stream_grammar;
			DirectObjectGrammar _direct_grammar;
		};

		SpiritParser::SpiritParser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _impl(new Impl(file)) {}

		SpiritParser::SpiritParser(const SpiritParser & other)
			: lexical::Stream(other) { _impl->_file = other._impl->_file; }

		files::File * SpiritParser::file(void) const { return _impl->_file; }

		XrefEntryPtr SpiritParser::Impl::ReadTableEntry(lexical::SpiritParser& s, types::integer objNumber)
		{
			char sp1, sp2, key, eol1, eol2;
			Token offset, number;
			s >> offset >> sp1 >> number >> sp2 >> key >> eol1 >> eol2;

			if (!(IsSpace(eol1) && Equals(eol2, WhiteSpace::CARRIAGE_RETURN)) &&
				!(IsSpace(eol1) && IsNewline(eol2)) &&
				!(Equals(eol1, WhiteSpace::CARRIAGE_RETURN) && IsNewline(eol2))) {
				throw Exception("End of line marker was not found in xref table entry");
			}

			static const char IN_USE = 'n';
			static const char NOT_IN_USE = 'f';

			if (key == IN_USE) {
				XrefUsedEntryPtr result(new files::XrefUsedEntry());
				result->SetFile(_file);
				result->SetOffset(IntegerObject(offset));
				result->SetObjectNumber(objNumber);
				result->SetGenerationNumber(IntegerObject(number).SafeConvert<types::ushort>());
				result->SetInitialized(false);
				return result;
			} else if (key == NOT_IN_USE) {
				XrefFreeEntryPtr result(new files::XrefFreeEntry());
				result->SetFile(_file);
				result->SetNextFreeObjectNumber(IntegerObject(offset));
				result->SetObjectNumber(objNumber);
				result->SetGenerationNumber(IntegerObject(number).SafeConvert<types::ushort>());
				result->SetInitialized(false);
				return result;
			} else {
				stringstream buffer;
				buffer << "Key in XRef table is either of " << IN_USE << " or " << NOT_IN_USE;

				throw Exception(buffer.str());
			}
		}

		XrefPtr SpiritParser::ReadXref(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadXref();
		}

		XrefPtr SpiritParser::ReadXref(void)
		{
			if (PeekTokenType() == Token::Type::INTEGER_OBJECT) {
				XrefStreamPtr result = new files::XrefStream();

				// Get stream object data
				auto xref = ReadDirectObjectWithType<StreamObjectPtr>();
				auto header = xref->GetHeader();

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

				auto body = xref->GetBodyDecoded();

				auto field1_size = fields->At(0);
				auto field2_size = fields->At(1);
				auto field3_size = fields->At(2);

				// Iterate over entries
				auto it = body.begin();
				for (auto i = 0; i < index_size; i += 2) {

					auto subsection_index = index->At(i);
					auto subsection_size = index->At(i + 1);
					for (auto idx = 0; idx < *subsection_size; idx++) {

						IntegerObject field1;
						for (int i = 0; i < *field1_size; ++i) {
							field1 = (field1 << 8) + (*it & 0xff);
							it++;
						}

						assert(field1 == 0 || field1 == 1 || field1 == 2);

						IntegerObject field2;
						for (int i = 0; i < *field2_size; ++i) {
							field2 = (field2 << 8) + (*it & 0xff);
							it++;
						}

						IntegerObject field3;
						for (int i = 0; i < *field3_size; ++i) {
							field3 = (field3 << 8) + (*it & 0xff);
							it++;
						}

						switch (field1) {
						case 0:
						{
							XrefFreeEntryPtr entry(new files::XrefFreeEntry());
							entry->SetFile(_impl->_file);
							entry->SetObjectNumber(*subsection_index + idx);
							entry->SetNextFreeObjectNumber(field2);
							entry->SetGenerationNumber(field3.SafeConvert<types::ushort>());
							result->push_back(entry);
							break;
						}
						case 1:
						{
							XrefUsedEntryPtr entry(new files::XrefUsedEntry());
							entry->SetFile(_impl->_file);
							entry->SetOffset(field2);
							entry->SetGenerationNumber(field3.SafeConvert<types::ushort>());
							entry->SetObjectNumber(*subsection_index + idx);
							result->push_back(entry);
							break;
						}
						case 2:
						{
							XrefCompressedEntryPtr entry(new files::XrefCompressedEntry());
							entry->SetFile(_impl->_file);
							entry->SetObjectNumber(*subsection_index + idx);
							entry->SetObjectStreamNumber(field2);
							entry->SetIndex(field3);
							result->push_back(entry);
							break;
						}
						default:
							throw exceptions::Exception("Unknown field in cross reference stream");
						}
					}
				}

				result->SetFile(_impl->_file);
				result->SetDictionary(header);
				return result;
			} else {
				XrefTablePtr table(new files::XrefTable());

				ReadTokenWithType(Token::Type::XREF_MARKER);
				ReadTokenWithType(Token::Type::EOL);

				while (PeekTokenType() != Token::Type::TRAILER) {
					IntegerObject revision, numberOfObjects;
					*this >> revision >> numberOfObjects;

					ReadTokenWithType(Token::Type::EOL);

					for (types::integer i = 0; i < numberOfObjects; ++i) {
						// check for overflow
						assert(revision + i >= revision);

						auto entry = _impl->ReadTableEntry(*this, static_cast<types::integer>(revision + i));
						table->push_back(entry);
					}
				}

				table->SetFile(_impl->_file);
				return table;
			}
		}

		std::vector<DirectObject> SpiritParser::ReadObjectStreamEntries(types::integer first, types::integer size)
		{
			std::vector<DirectObject> result;
			auto headers = ReadObjectStreamHeaders(size);
			for (auto header : headers) {
				seekg(first + header.offset);
				auto obj = ReadDirectObject();

				ObjectBaseVisitor visitor;
				auto base = obj.apply_visitor(visitor);
				base->SetObjectNumber(header.object_number);

				result.push_back(obj);
			}

			return result;
		}

		std::vector<ObjectStreamHeader> SpiritParser::ReadObjectStreamHeaders(types::integer size)
		{
			std::vector<ObjectStreamHeader> obj;

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			try {
				auto result = qi::parse(input_begin_pos, input_end_pos, _impl->_obj_stream_grammar(size), obj);
				if (result) {
					return obj;
				} else {
					LOG_ERROR << "Parsing failed" << endl;

					const auto& pos = input_begin_pos.get_position();
					LOG_ERROR <<
						"Error at offset " << pos.offset << endl <<
						"'" << input_begin_pos.get_currentline() << "'" << endl <<
						setw(pos.column + 7) << " ^- here" << endl;

					throw exceptions::Exception("Parsing failed");
				}
			} catch (const qi::expectation_failure<pos_iterator_type>& exception) {
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

		DirectObject SpiritParser::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, std::ios::beg);
			return ReadDirectObject();
		}

		DirectObject SpiritParser::ReadDirectObject(void)
		{
			DirectObject obj;

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			try
			{
				auto result = qi::parse(input_begin_pos, input_end_pos, _impl->_direct_grammar(_impl->_file, offset), obj);
				if (result) {
					// For some reason, end of parsing and current offset does not match
					// So I adjust current stream position
					auto end_pos = input_begin_pos.get_position();
					seekg(end_pos.offset, std::ios::beg);
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
	}
}
