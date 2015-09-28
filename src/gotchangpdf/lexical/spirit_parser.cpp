#include "precompiled.h"
#include "spirit_parser.h"

#include "xref.h"
#include "character.h"
#include "exception.h"
#include "file.h"
#include "log.h"

#include "spirit_grammar.h"
#include "xref_grammar.h"
#include "reverse_grammar.h"

#include <iomanip>

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

			files::XrefEntry ReadTableEntry(lexical::SpiritParser& s, types::integer objNumber);

			template <typename Result, typename Grammar, typename Iterator>
			Result Read(Grammar& grammar, Iterator& input_begin_pos, Iterator& input_end_pos);

			files::File *_file = nullptr;
			XrefTableSubsectionsGrammar _xref_grammar;
			ObjectStreamGrammar _obj_stream_grammar;
			DirectObjectGrammar _direct_grammar;
			ReverseGrammar _reverse_grammar;
		};

		SpiritParser::SpiritParser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _impl(new Impl(file)) {}

		SpiritParser::SpiritParser(const SpiritParser & other)
			: lexical::Stream(other) { _impl->_file = other._impl->_file; }

		files::File * SpiritParser::file(void) const { return _impl->_file; }

		files::XrefEntry SpiritParser::Impl::ReadTableEntry(lexical::SpiritParser& s, types::integer objNumber)
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
				XrefUsedEntryPtr result(objNumber, IntegerObject(number).SafeConvert<types::ushort>(), IntegerObject(offset).SafeConvert<types::stream_offset>());
				result->SetFile(_file);
				result->SetInitialized(false);
				return result;
			} else if (key == NOT_IN_USE) {
				XrefFreeEntryPtr result(objNumber, IntegerObject(number).SafeConvert<types::ushort>(), IntegerObject(offset).SafeConvert<types::integer>());
				result->SetFile(_file);
				result->SetInitialized(false);
				return result;
			} else {
				stringstream buffer;
				buffer << "Key in XRef table is neither of " << IN_USE << " or " << NOT_IN_USE;

				throw Exception(buffer.str());
			}
		}

		template <typename Result, typename Grammar, typename Iterator>
		Result SpiritParser::Impl::Read(Grammar& grammar, Iterator& input_begin_pos, Iterator& input_end_pos)
		{
			try {
				Result obj;
				auto result = qi::parse(input_begin_pos, input_end_pos, grammar, obj);
				if (result) {
					return obj;
				}

				LOG_ERROR << "Parsing failed" << endl;

				const auto& pos = input_begin_pos.get_position();
				LOG_ERROR <<
					"Error at offset " << pos.offset << endl <<
					"'" << input_begin_pos.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here";

				throw exceptions::Exception("Parsing failed");
			}
			catch (const qi::expectation_failure<pos_iterator_type>& exception) {
				LOG_ERROR << "Parsing failed" << endl;

				auto pos_begin = exception.first;
				const auto& pos = pos_begin.get_position();
				LOG_ERROR <<
					"Error at offset " << pos.offset << endl <<
					"Expecting " << exception.what() << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here";

				throw;
			}
		}

		files::Xref SpiritParser::ReadXref(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadXref();
		}

		files::Xref SpiritParser::ReadXref(void)
		{
			if (PeekTokenType() == Token::Type::INTEGER_OBJECT) {
				XrefStreamPtr result = XrefStreamPtr(files::XrefStream());

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
							entry->SetFile(_impl->_file);
							subsection->Add(entry);
							break;
						}
						case 1:
						{
							XrefUsedEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
							entry->SetFile(_impl->_file);
							subsection->Add(entry);
							break;
						}
						case 2:
						{
							XrefCompressedEntryPtr entry(*subsection_index + idx, static_cast<types::ushort>(0), field2, field3);
							entry->SetFile(_impl->_file);
							subsection->Add(entry);
							break;
						}
						default:
							throw exceptions::Exception("Unknown field in cross reference stream");
						}
					}

					result->Add(subsection);
				}

				result->SetFile(_impl->_file);
				result->SetTrailerDictionary(header);
				return result;
			} else {
#ifdef OLD_STYLE_PARSING
				auto start = omp_get_wtime();

				XrefTablePtr table(new files::XrefTable());

				ReadTokenWithType(Token::Type::XREF_MARKER);
				ReadTokenWithType(Token::Type::EOL);

				while (PeekTokenType() != Token::Type::TRAILER) {
					IntegerObject revision, numberOfObjects;
					*this >> revision >> numberOfObjects;

					ReadTokenWithType(Token::Type::EOL);

					XrefSubsectionPtr subsection(new files::XrefSubsection(revision, numberOfObjects));

					
					for (types::integer i = 0; i < numberOfObjects; ++i) {
						auto entry = _impl->ReadTableEntry(*this, SafeAddition(revision, i));
						subsection->Add(entry);
					}

					table->Add(subsection);
				}

				table->SetFile(_impl->_file);

				auto end = omp_get_wtime();
				LOG_WARNING << "Xref parsing time: " << end - start;
				return table;
#else
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			auto start = omp_get_wtime();
			auto result = _impl->Read<files::Xref>(_impl->_xref_grammar(_impl->_file), input_begin_pos, input_end_pos);
			auto end = omp_get_wtime();

			LOG_WARNING << "Xref parsing time: " << end - start;
			return result;
#endif
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

		types::integer SpiritParser::ReadLastXrefOffset()
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
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

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_obj_stream_grammar(size);
			return _impl->Read<std::vector<ObjectStreamHeader>>(gram, input_begin_pos, input_end_pos);
		}

		DirectObject SpiritParser::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, std::ios::beg);
			return ReadDirectObject();
		}

		DirectObject SpiritParser::ReadDirectObject(void)
		{
			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			types::stream_offset offset = tellg();

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _impl->_file->GetFilename(), 1, 1, offset);
			pos_iterator_type input_end_pos;

			const auto& gram = _impl->_direct_grammar(_impl->_file, offset);
			return _impl->Read<DirectObject>(gram, input_begin_pos, input_end_pos);
		}
	}
}
