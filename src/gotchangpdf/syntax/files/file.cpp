#include "precompiled.h"
#include "file.h"

#include "raw_reverse_stream.h"
#include "parser.h"
#include "reverse_parser.h"
#include "exception.h"

#include "xref_chain.h"
#include "header.h"

#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		static const int invalid_xref_offset = -1;

		std::shared_ptr<File> File::Open(const std::string& path)
		{
			return std::shared_ptr<File>(new File(path));
		}

		File::File(const std::string& path)
			: _full_path(path)
		{
			_filename = extract_filename(path);
			LOG_WARNING_GLOBAL << "File constructor " << _filename;
		}

		File::~File(void)
		{
			LOG_WARNING_GLOBAL << "File destructor " << _filename;

			if (nullptr != _input)
			{
				_input->close();
				_input = nullptr;
			}

			_cache.clear();
		}

		void File::Initialize()
		{
			LOG_DEBUG(_filename) << "Initialize";

			if (_initialized)
				return;

			_input = make_shared<FileDevice>();
			_input->open(_full_path,
				ios_base::in | ios_base::out | ios_base::binary | ios::ate);

			if (!_input || !_input->good())
				throw GeneralException("Could not open file");

			auto file_size = _input->tellg();
			auto stream = Parser(shared_from_this(), *_input);

			_header = stream.ReadHeader(0);
			auto offset = GetLastXrefOffset(file_size);

			if (invalid_xref_offset == offset) {
				auto xref = stream.FindAllObjects();
				_xref->Append(xref);
				_initialized = true;
				return;
			}

			try
			{
				ReadXref(offset);
			}
			catch (NotSupportedException&) {
				throw;
			}
			catch (...) {
				auto xref = stream.FindAllObjects();
				_xref->Append(xref);
			}

			_initialized = true;

			//std::string dest("C:\\Users\\Gotcha\\Documents\\");
			//dest += _filename;
			//SaveAs(dest);
		}

		void File::ReadXref(types::stream_offset offset)
		{
			auto stream = Parser(shared_from_this(), *_input);

			do {
				auto xref = stream.ReadXref(offset);
				_xref->Append(xref);

				if (xref->GetTrailerDictionary()->Contains(constant::Name::Encrypt)) {
					throw NotSupportedException("Encrypted files are not yet supported");
				}

				if (xref->GetTrailerDictionary()->Contains(constant::Name::XRefStm)) {
					auto stm_offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::XRefStm)->Value();
					auto xref_stm = stream.ReadXref(stm_offset);

					assert(!xref_stm->GetTrailerDictionary()->Contains(constant::Name::Prev));
					_xref->Append(xref_stm);
				}

				if (!xref->GetTrailerDictionary()->Contains(constant::Name::Prev)) {
					break;
				}

				offset = xref->GetTrailerDictionary()->FindAs<IntegerObjectPtr>(constant::Name::Prev)->Value();
			} while (true);
		}

		types::stream_offset File::GetLastXrefOffset(types::stream_size file_size)
		{
			try
			{
				ReverseStream raw_reversed(*_input, file_size);
				auto reverse_stream = ReverseParser(raw_reversed);
				return reverse_stream.ReadLastXrefOffset();
			}
			catch (...) {
				LOG_ERROR(_filename) << "Could not find xref offset, using fallback mechanism";
				return invalid_xref_offset;
			}
		}

		ObjectPtr File::GetIndirectObject(types::big_uint objNumber,
			types::ushort genNumber)
		{
			LOG_DEBUG(_filename) << "GetIndirectObject " << objNumber << " and " << genNumber;

			if (!_initialized)
				throw FileNotInitializedException(_filename);

			auto item = _xref->GetXrefEntry(objNumber, genNumber);

			if (!item->InUse())
				throw ObjectMissingException(objNumber, genNumber);

			switch (item->GetUsage()) {
			case XrefEntryBase::Usage::Used:
			{
				auto used = XrefUtils::ConvertTo<XrefUsedEntryPtr>(item);
				return used->GetReference();
			}
			case XrefEntryBase::Usage::Compressed:
			{
				auto compressed = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(item);
				return compressed->GetReference();
			}
			case XrefEntryBase::Usage::Null:
				LOG_ERROR(_filename) << "Xref entry type is null for object " << objNumber << " " << genNumber;
				throw ObjectMissingException(objNumber, genNumber);
			case XrefEntryBase::Usage::Free:
				LOG_ERROR(_filename) << "Xref entry type is free for object " << objNumber << " " << genNumber << " and InUse() returned true";
				assert(!"Current entry is supposed to be InUse(), while it's type is Free");
				throw ObjectMissingException(objNumber, genNumber);
			default:
				throw GeneralException("Unknown entry type");
			}
		}

		HeaderPtr File::GetHeader(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _header;
		}

		XrefChainPtr File::GetXrefChain(void) const
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			return _xref;
		}

		// experimental
		void File::SaveAs(const std::string& path)
		{
			if (!_initialized)
				throw FileNotInitializedException(_filename);

			std::fstream output;
			output.open(path, ios_base::out | ios_base::binary);

			if (!output || !output.good())
				throw GeneralException("Could not open file");

			auto ver = _header->GetVersion();
			output << "%PDF-1." << static_cast<int>(ver) << endl;

			auto end = _xref->End();
			for (auto it = _xref->Begin(); *it != *end; ++(*it)) {
				auto xref_base = it->Value();

				if (xref_base->GetType() == XrefBase::Type::Stream)
					continue;

				auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref_base);
				auto table_size = xref_table->Size();
				auto table_items = xref_table->Entries();
				for (decltype(table_size) i = 0; i < table_size; ++i) {
					auto entry = table_items[i];

					if (!entry->InUse())
						continue;

					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					auto obj = used_entry->GetReference();
					auto obj_str = obj->ToPdf();

					//TODO: This is kinda hacky
					// make new xref table for storing modified values
					auto obj_offset = output.tellg();
					used_entry->SetOffset(obj_offset);
					obj->SetOffset(obj_offset);

					output << obj->GetObjectNumber() << " " << obj->GetGenerationNumber() << " " << "obj" << endl;
					output << obj_str << endl;
					output << "endobj" << endl;
				}

				auto last_offset = output.tellg();
				output << "xref" << endl;

				for (decltype(table_size) i = 0; i < table_size; ++i) {
					auto first = table_items[i];
					auto subsection_idx = first->GetObjectNumber();

					size_t subsection_size = 0;
					for (decltype(i) j = i; j < table_size; ++j, ++subsection_size) {
						auto next_entry = table_items[j];
						if (next_entry->GetObjectNumber() != gotchangpdf::SafeAddition<types::big_uint>(subsection_idx, j)) {
							break;
						}
					}

					output << subsection_idx << " " << subsection_size << endl;
					for (decltype(subsection_size) j = i; j < subsection_size; ++j) {
						auto entry = table_items[j];
						if (!entry->InUse()) {
							output << setfill('0') << setw(10) << 0;
							output << ' ';
							output << setfill('0') << setw(10) << 65535;
							output << ' ';
							output << 'f';
							output << ' ';
							output << '\n';
							continue;
						}

						auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
						output << setfill('0') << setw(10) << used_entry->GetOffset();
						output << ' ';
						output << setfill('0') << setw(10) << used_entry->GetGenerationNumber();
						output << ' ';
						output << 'n';
						output << ' ';
						output << '\n';
					}

					i += subsection_size;
				}

				auto trailer = xref_table->GetTrailerDictionary();
				output << "trailer" << endl;
				output << trailer->ToPdf() << endl;
				output << "startxref" << endl;
				output << last_offset << endl;
				output << "%%EOF" << endl;
			}

			output.flush();
			output.close();
		}
	}
}
