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
			auto source_end = source_xref->end();
			for (auto it = source_xref->begin(); it != source_end; ++it) {
				auto original_xref = *it;

				// Deep clone of original xref
				auto new_xref = CloneXref(destination, original_xref);

				// Insert cloned table to destination xref chain
				// NOTE: This is required, so that indirect references are accessible
				dest_xref->Append(new_xref);
			}

			auto source_iterator = source_xref->begin();
			for (auto dest_iterator = dest_xref->begin(); dest_iterator != dest_xref->end(); ++dest_iterator, ++source_iterator) {
				auto original_xref = *source_iterator;
				auto new_xref = *dest_iterator;

				// Xref streams content needs to be recalculated
				if (new_xref->GetType() != XrefBase::Type::Stream) {
					continue;
				}

				auto source_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(original_xref);
				auto dest_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(new_xref);

				auto stream_obj = source_xref_stream->GetStreamObject();
				auto stream_object_number = stream_obj->GetObjectNumber();
				auto stream_generation_number = stream_obj->GetGenerationNumber();

				assert(dest_xref->Contains(stream_object_number, stream_generation_number)
					&& "Xref stream reference was not found in cloned xref");

				// Get cloned stream based on same object and generation number from cloned xref
				auto new_stream_entry = dest_xref->GetXrefEntry(stream_object_number, stream_generation_number);
				auto new_stream_used_entry = XrefUtils::ConvertTo<XrefUsedEntryBasePtr>(new_stream_entry);
				auto new_stream_obj = new_stream_used_entry->GetReference();
				auto new_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(new_stream_obj);

				// Fix stream object reference and recalculate stream content
				dest_xref_stream->SetStreamObject(new_stream);
				dest_xref_stream->RecalculateContent();
			}

			// All xref entries are stored in reversed order than it is written to file
			dest_xref->Reverse();

			bool first_xref = true;
			auto last_xref_iterator = dest_xref->begin();
			for (auto it = dest_xref->begin(); it != dest_xref->end(); ++it) {
				auto new_xref = *it;

				if (!first_xref) {
					auto prev_xref = *last_xref_iterator;
					auto prev_xref_offset = prev_xref->GetLastXrefOffset();

					auto trailer_dictionary = new_xref->GetTrailerDictionary();
					if (!trailer_dictionary->Contains(constant::Name::Prev)) {
						IntegerObjectPtr xref_offset(prev_xref_offset);
						trailer_dictionary->Insert(constant::Name::Prev, xref_offset);
					}

					// Fix prev entry for all following trailers
					auto prev = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Prev);
					prev->SetValue(prev_xref_offset);

					last_xref_iterator++;
				}

				// Stream length to be adjusted according to calculated size
				if (m_recalculate_stream_size) {
					RecalculateStreamsLength(new_xref);
				}

				if (m_recalculate_xref_size) {
					auto xref_size = new_xref->Size();
					auto trailer_dictionary = new_xref->GetTrailerDictionary();

					if (!trailer_dictionary->Contains(constant::Name::Size)) {
						IntegerObjectPtr xref_size_obj(xref_size);
						trailer_dictionary->Insert(constant::Name::Size, xref_size_obj);
					}

					auto size = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Size);
					if (size->GetValue() != xref_size) {
						size->SetValue(xref_size);
					}
				}

				// Write all body objects
				WriteXrefObjects(destination, new_xref);

				// Write xref to output
				WriteXref(*output, new_xref);

				first_xref = false;
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

			// Deep clone of original xref
			auto new_xref = CloneXref(destination, tmp_xref);

			// Insert cloned table to destination xref chain
			// NOTE: This is required, so that indirect references are accessible
			auto dest_xref = destination->GetXrefChain();
			dest_xref->Append(new_xref);

			// Stream length to be adjusted according to calculated size
			if (m_recalculate_stream_size) {
				RecalculateStreamsLength(new_xref);
			}

			// Write xref objects to output
			WriteXrefObjects(destination, new_xref);

			// Write xref itself to output
			WriteXref(*output, new_xref);

			// Cleanup
			output->flush();
		}

		void FileWriter::RecalculateStreamLength(ObjectPtr obj)
		{
			if (!ObjectUtils::IsType<StreamObjectPtr>(obj)) {
				return;
			}

			auto stream_obj = ObjectUtils::ConvertTo<StreamObjectPtr>(obj);
			auto stream_data = stream_obj->GetBodyEncoded();
			auto stream_header = stream_obj->GetHeader();

			if (!stream_header->Contains(constant::Name::Length)) {
				IntegerObjectPtr new_length(stream_data->size());
				stream_header->Insert(constant::Name::Length, new_length);
				return;
			}

			auto length_obj = stream_header->FindAs<IntegerObjectPtr>(constant::Name::Length);
			if (length_obj->GetValue() != stream_data->size()) {
				length_obj->SetValue(stream_data->size());
			}
		}

		void FileWriter::RecalculateStreamsLength(XrefBasePtr source)
		{
			auto table_size = source->Size();
			auto table_items = source->Entries();

			for (decltype(table_size) i = 0; i < table_size; ++i) {
				auto entry = table_items[i];

				if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					auto new_obj = used_entry->GetReference();
					RecalculateStreamLength(new_obj);

					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
					auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);
					auto new_obj = compressed_entry->GetReference();
					RecalculateStreamLength(new_obj);

					continue;
				}

				assert(false && "Uncrecognized entry type");
			}
		}

		XrefBasePtr FileWriter::CloneXref(std::shared_ptr<File> destination, XrefBasePtr source)
		{
			// Create cloned xref table
			// Xref base has no default constructor, therefore it is initialized with table
			XrefBasePtr result = XrefTablePtr();

			if (source->GetType() == XrefBase::Type::Stream) {
				result = XrefStreamPtr();
			}

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
					new_obj->SetFile(destination);

					// Create new entry in our cloned table
					XrefUsedEntryPtr new_entry(
						new_obj_number,
						new_gen_number,
						used_entry->GetOffset());

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
					new_obj->SetFile(destination);

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

		void FileWriter::WriteXrefObjects(std::shared_ptr<File> destination, XrefBasePtr source)
		{
			auto table_size = source->Size();
			auto table_items = source->Entries();

			for (decltype(table_size) i = 0; i < table_size; ++i) {
				auto entry = table_items[i];

				if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
					auto new_obj = used_entry->GetReference();

					auto output = destination->GetInputStream();
					if (m_recalculate_offset) {
						auto new_offset = output->tellg();
						new_obj->SetOffset(new_offset);
						used_entry->SetOffset(new_offset);
					}

					// Write new object into destination file
					WriteObject(*output, new_obj);
					continue;
				}

				if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
					auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);
					auto new_obj = compressed_entry->GetReference();

					auto output = destination->GetInputStream();
					if (m_recalculate_offset) {
						auto new_offset = output->tellg();
						new_obj->SetOffset(new_offset);
					}

					// Write new object into destination file
					WriteObject(*output, new_obj);
					continue;
				}

				assert(false && "Uncrecognized entry type");
			}
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
				auto stream_obj = xref_stream->GetStreamObject();
				auto stream_offset = stream_obj->GetOffset();

				if (m_recalculate_offset) {
					xref_stream->SetLastXrefOffset(stream_offset);
				}

				// Xref streams shall be referenced either from themselves
				// or from a different xref. Therefore it is not needed to
				// serialize them in any fashion

				WriteXrefOffset(output, stream_offset);
			}
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
