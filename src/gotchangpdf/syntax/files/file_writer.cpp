#include "precompiled.h"
#include "file_writer.h"
#include "file.h"
#include "xref.h"

#include <fstream>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		// experimental
		void FileWriter::Write(const std::shared_ptr<File> source, std::shared_ptr<File> destination)
		{
			if (!source->IsInitialized())
				throw FileNotInitializedException(source->GetFilename());

			auto input = source->GetInputStream();
			auto output = destination->GetInputStream();

			auto header = source->GetHeader();
			auto ver = header->GetVersion();
			*output << "%PDF-1." << static_cast<int>(ver) << endl;

			auto source_xref = source->GetXrefChain();
			auto dest_xref = destination->GetXrefChain();
			auto end = source_xref->End();
			for (auto it = source_xref->Begin(); *it != *end; ++(*it)) {
				auto original_xref = it->Value();
				auto new_xref = WriteXrefObjects(destination, original_xref);

				// Write xref to output
				WriteXref(*output, new_xref);

				// Insert cloned table to destination xref chain
				dest_xref->Append(new_xref);
			}

			// Cleanup
			output->flush();
		}

		void FileWriter::WriteIncremental(const std::shared_ptr<File> source, std::shared_ptr<File> destination)
		{
			if (!source->IsInitialized())
				throw FileNotInitializedException(source->GetFilename());

			auto input = source->GetInputStream();
			auto output = destination->GetInputStream();

			// Write original data
			input->seekg(0);
			std::copy(istreambuf_iterator<char>(*input),
				istreambuf_iterator<char>(),
				ostreambuf_iterator<char>(*output));

			// Get all changed entries
			auto tmp_xref = CreateIncrementalXref(source, destination);

			// Write xref objects to output
			auto new_xref = WriteXrefObjects(destination, tmp_xref);

			// Write xref itself to output
			WriteXref(*output, new_xref);

			// Insert cloned table to destination xref chain
			auto dest_xref = destination->GetXrefChain();
			dest_xref->Append(new_xref);

			// Cleanup
			output->flush();
		}

		XrefBasePtr FileWriter::WriteXrefObjects(std::shared_ptr<File> destination, XrefBasePtr source)
		{
			// Create cloned xref table
			// Xref base has no default constructor, therefore it is initialized with table
			XrefBasePtr result = XrefTablePtr();

			if (source->GetType() == XrefBase::Type::Stream)
				result = XrefStreamPtr();

			auto table_size = source->Size();
			auto table_items = source->Entries();

			// Clone original table trailer
			auto new_trailer = ObjectUtils::Clone<DictionaryObjectPtr>(source->GetTrailerDictionary());
			new_trailer->SetFile(destination);

			result->SetTrailerDictionary(new_trailer);
			result->SetOffset(source->GetOffset());

			// TODO mayble also recalculate?
			result->SetLastXrefOffset(source->GetLastXrefOffset());

			for (decltype(table_size) i = 0; i < table_size; ++i) {
				auto entry = table_items[i];

				auto new_obj_number = entry->GetObjectNumber();
				auto new_gen_number = entry->GetGenerationNumber();

				if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
					XrefFreeEntryPtr new_entry(
						new_obj_number,
						new_gen_number);

					new_entry->SetFile(destination);
					result->Add(new_entry);
					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					auto original_obj = used_entry->GetReference();

					// Deep clone existing object
					auto new_obj = ObjectUtils::Clone<ObjectPtr>(original_obj);

					// Set new object offset in the destination file
					new_obj->SetObjectNumber(new_obj_number);
					new_obj->SetGenerationNumber(new_gen_number);
					new_obj->SetFile(destination);

					auto output = destination->GetInputStream();
					if (m_recalculate_offset) {
						auto new_offset = output->tellg();
						new_obj->SetOffset(new_offset);
					}

					// Write new object into destination file
					WriteObject(*output, new_obj);

					// Create new entry in our cloned table
					XrefUsedEntryPtr new_entry(
						new_obj_number,
						new_gen_number,
						new_obj->GetOffset());

					new_entry->SetFile(destination);
					new_entry->SetReference(new_obj);
					new_entry->SetInitialized();
					result->Add(new_entry);
					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
					auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);
					auto original_obj = compressed_entry->GetReference();

					// Deep clone existing object
					auto new_obj = ObjectUtils::Clone<ObjectPtr>(original_obj);

					// Set new object offset in the destination file
					new_obj->SetObjectNumber(new_obj_number);
					new_obj->SetGenerationNumber(new_gen_number);
					new_obj->SetFile(destination);

					auto output = destination->GetInputStream();
					if (m_recalculate_offset) {
						auto new_offset = output->tellg();
						new_obj->SetOffset(new_offset);
					}

					// Write new object into destination file
					WriteObject(*output, new_obj);

					auto new_obj_stream_number = compressed_entry->GetObjectStreamNumber();
					auto new_index = compressed_entry->GetIndex();

					// Create new entry in our cloned table
					XrefCompressedEntryPtr new_entry(
						new_obj_number,
						new_gen_number,
						new_obj_stream_number,
						new_index);

					new_entry->SetFile(destination);
					new_entry->SetReference(new_obj);
					new_entry->SetInitialized();
					result->Add(new_entry);
					continue;
				}

				assert(false && "Uncrecognized entry type");
			}

			return result;
		}

		XrefBasePtr FileWriter::CreateIncrementalXref(std::shared_ptr<File> source, std::shared_ptr<File> destination)
		{
			XrefChainPtr chain = source->GetXrefChain();

			XrefTablePtr new_table;
			XrefFreeEntryPtr free_entry(0, (types::ushort)65535);
			free_entry->SetFile(source);
			new_table->Add(free_entry);

			bool new_entries = false;
			auto end = chain->End();
			for (auto it = chain->Begin(); *it != *end; ++(*it)) {
				auto xref = it->Value();
				auto table_size = xref->Size();
				auto table_items = xref->Entries();
				for (decltype(table_size) i = 0; i < table_size; ++i) {
					auto entry = table_items[i];

					if (!entry->InUse())
						continue;

					if (!entry->IsDirty())
						continue;

					auto new_obj_number = entry->GetObjectNumber();
					auto new_gen_number = entry->GetGenerationNumber();

					if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
						auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
						auto obj = used_entry->GetReference();
						auto new_offset = used_entry->GetOffset();

						XrefUsedEntryPtr new_entry(
							new_obj_number,
							new_gen_number,
							new_offset);

						new_entry->SetFile(source);
						new_entry->SetReference(obj);
						new_entry->SetInitialized();
						new_table->Add(new_entry);
						continue;
					}

					if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
						auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);
						auto obj = compressed_entry->GetReference();
						auto new_obj_stream_number = compressed_entry->GetObjectStreamNumber();
						auto new_index = compressed_entry->GetIndex();

						XrefCompressedEntryPtr new_entry(
							new_obj_number,
							new_gen_number,
							new_obj_stream_number,
							new_index);

						new_entry->SetFile(source);
						new_entry->SetReference(obj);
						new_entry->SetInitialized();
						new_table->Add(new_entry);
						continue;
					}

					assert(false && "Uncrecognized entry type");
				}
			}

			// Construct new trailer for latest xref table
			DictionaryObjectPtr new_trailer;
			new_trailer->SetFile(destination);

			// Set size of new entries
			IntegerObjectPtr new_size(new_table->Size());
			new_trailer->Insert(constant::Name::Size, new_size);

			XrefBasePtr prev_table = chain->Begin()->Value();
			auto prev_trailer = prev_table->GetTrailerDictionary();

			// Set document ID
			if (prev_trailer->Contains(constant::Name::ID)) {
				ContainableObjectPtr id = prev_trailer->Find(constant::Name::ID);
				ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
				new_trailer->Insert(constant::Name::ID, cloned);
			}

			// Set document Info
			if (prev_trailer->Contains(constant::Name::Info)) {
				ContainableObjectPtr id = prev_trailer->Find(constant::Name::Info);
				ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
				new_trailer->Insert(constant::Name::Info, cloned);
			}

			// Skip table, if there were no dirty entries
			if (new_entries) {
				auto last_xref_offset = prev_table->GetOffset();
				IntegerObjectPtr new_offset(last_xref_offset);
				new_trailer->Insert(constant::Name::Prev, new_offset);
			}

			// Add encryption entry to trailer
			if (source->IsEncrypted()) {
				ObjectPtr obj = source->GetEncryptionDictionary();

				if (ObjectUtils::IsType<DictionaryObjectPtr>(obj)) {
					DictionaryObjectPtr encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(obj);
					DictionaryObjectPtr cloned = ObjectUtils::Clone<DictionaryObjectPtr>(encryption_dictionary);
					new_trailer->Insert(constant::Name::Encrypt, encryption_dictionary);
				}
			}

			// Set trailer
			new_table->SetTrailerDictionary(new_trailer);

			return new_table;
		}

		void FileWriter::WriteObject(std::ostream& output, const Object& obj)
		{
			output << obj.GetObjectNumber() << " " << obj.GetGenerationNumber() << " " << "obj" << endl;
			output << obj.ToPdf() << endl;
			output << "endobj" << endl;
		}

		void FileWriter::WriteXrefOffset(std::ostream& output, types::stream_offset offset)
		{
			output << "startxref" << endl;
			output << offset << endl;
			output << "%%EOF" << endl;
		}

		void FileWriter::WriteXref(std::iostream& output, XrefBasePtr xref)
		{
			if (xref->GetType() == XrefBase::Type::Table) {

				// Convert to xref table
				auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref);

				// Seperate table from content
				output << '\n';

				// Write the table
				WriteXrefTable(output, xref_table);
				WriteXrefOffset(output, xref_table->GetLastXrefOffset());
			}

			if (xref->GetType() == XrefBase::Type::Stream) {
				auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(xref);
				auto stream_offset = output.tellg();
				WriteXrefStream(output, xref_stream);
				WriteXrefOffset(output, stream_offset);
			}
		}

		void FileWriter::WriteXrefStream(std::iostream& output, XrefStreamPtr xref_stream)
		{
			auto stream = xref_stream->GetStreamObject();
			WriteObject(output, stream);
		}

		void FileWriter::WriteXrefTable(std::iostream& output, XrefTablePtr xref_table)
		{
			if (m_recalculate_offset) {
				auto offset = output.tellg();
				xref_table->SetLastXrefOffset(offset);
			}

			auto table_size = xref_table->Size();
			auto table_items = xref_table->Entries();

			output << "xref" << endl;

			for (decltype(table_size) i = 0; i < table_size;) {
				auto first = table_items[i];
				auto subsection_idx = first->GetObjectNumber();

				size_t subsection_size = 1;
				for (decltype(i) j = 1; j < table_size - i; ++j, ++subsection_size) {
					auto next_entry = table_items[i + j];
					if (next_entry->GetObjectNumber() != gotchangpdf::SafeAddition<types::big_uint>(subsection_idx, j)) {
						break;
					}
				}

				output << subsection_idx << " " << subsection_size << endl;
				for (decltype(subsection_size) j = 0; j < subsection_size; ++j) {
					auto entry = table_items[i + j];
					if (!entry->InUse()) {
						output << setfill('0') << setw(10) << 0;
						output << ' ';
						output << setfill('0') << setw(5) << 65535;
						output << ' ';
						output << 'f';
						output << ' ';
						output << '\n';
						continue;
					}

					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					output << setfill('0') << setw(10) << used_entry->GetOffset();
					output << ' ';
					output << setfill('0') << setw(5) << used_entry->GetGenerationNumber();
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
		}
	}
}
