#include "precompiled.h"

#include "utils/character.h"

#include "syntax/files/file_writer.h"
#include "syntax/files/file.h"
#include "syntax/files/xref.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include <fstream>
#include <unordered_set>
#include <iomanip>

namespace gotchangpdf {
namespace syntax {

using namespace std;

// experimental
void FileWriter::Write(FilePtr source, FilePtr destination) {
	if (!source->IsInitialized()) {
		throw FileNotInitializedException(source->GetFilename());
	}

	auto input = source->GetInputStream();
	auto output = destination->GetOutputStream();

	auto header = source->GetHeader();
	auto ver = header->GetVersion();

	output->Write("%PDF-1.");
	output->Write(static_cast<int>(ver));
	output->Write(WhiteSpace::LINE_FEED);

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
		auto new_stream_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(new_stream_entry);
		auto new_stream_obj = new_stream_used_entry->GetReference();
		auto new_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(new_stream_obj);

		// Fix stream object reference
		dest_xref_stream->SetStreamObject(new_stream);
	}

	// All xref entries are stored in reversed order than it is written to file
	dest_xref->Reverse();

	// Compress and optimize here
	CompressAndOptimize(dest_xref);

	bool first_xref = true;
	auto last_xref_iterator = dest_xref->begin();
	for (auto it = dest_xref->begin(); it != dest_xref->end(); ++it) {
		auto new_xref = *it;

		if (!first_xref) {
			auto prev_xref = *last_xref_iterator;
			auto prev_xref_offset = prev_xref->GetLastXrefOffset();

			auto trailer_dictionary = new_xref->GetTrailerDictionary();
			if (!trailer_dictionary->Contains(constant::Name::Prev)) {
				IntegerObjectPtr xref_offset = make_deferred<IntegerObject>(prev_xref_offset);
				trailer_dictionary->Insert(constant::Name::Prev, xref_offset);
			}

			// Fix prev entry for all following trailers
			auto prev = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Prev);
			prev->SetValue(prev_xref_offset);

			last_xref_iterator++;
		}

		// Object stream contents has to be adjusted separately
		if (m_recalculate_object_stream_content) {
			RecalculateObjectStreamContent(new_xref);
		}

		// Stream length to be adjusted according to calculated size
		if (m_recalculate_stream_size) {
			RecalculateStreamsLength(new_xref);
		}

		if (m_recalculate_xref_size) {
			auto xref_size = new_xref->Size();
			auto trailer_dictionary = new_xref->GetTrailerDictionary();

			if (!trailer_dictionary->Contains(constant::Name::Size)) {
				IntegerObjectPtr xref_size_obj = make_deferred<IntegerObject>(xref_size);
				trailer_dictionary->Insert(constant::Name::Size, xref_size_obj);
			}

			auto size = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Size);
			if (size->GetUnsignedIntegerValue() != xref_size) {
				size->SetValue(xref_size);
			}
		}

		// Write all body objects
		WriteXrefObjects(destination, new_xref);

		// Write xref to output
		WriteXref(output, new_xref);

		first_xref = false;
	}

	// Cleanup
	output->Flush();
}

void FileWriter::WriteIncremental(FilePtr source, FilePtr destination) {
	if (!source->IsInitialized()) {
		throw FileNotInitializedException(source->GetFilename());
	}

	auto input = source->GetInputStream();
	auto output = destination->GetOutputStream();

	// Write original data
	CopyStreamContent(input, output);

	// Make an extra newline before starting new xref section
	output->Write(WhiteSpace::LINE_FEED);

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
	WriteXref(output, new_xref);

	// Cleanup
	output->Flush();
}

void FileWriter::RecalculateObjectStreamContent(XrefBasePtr source) {
	// Only matters for xref streams
	if (!ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(source)) {
		return;
	}

	// Convert to stream
	auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(source);

	// Store array of objects for every object stream
	std::unordered_map<types::big_uint, std::vector<XrefCompressedEntryPtr>> object_stream_map;

	for (auto entry : source) {

		// Ignore non-compressed entries
		if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefCompressedEntryPtr>(entry)) {
			continue;
		}

		// Convert to compressed entry
		auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);

		// Obtain required informations
		auto object_stream_number = compressed_entry->GetObjectStreamNumber();

		// Insert compressed object into map
		object_stream_map[object_stream_number].push_back(compressed_entry);
	}

	// After all entries have been acquired we begin with the recalculation
	for (auto pair : object_stream_map) {
		auto object_stream_number = pair.first;
		auto entries = pair.second;

		// TODO This may not work in some cases
		// Rework using xref chain, to find the object stream number?
		auto object_stream_entry = source->Find(object_stream_number);
		if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(object_stream_entry)) {
			continue;
		}

		auto object_stream_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(object_stream_entry);
		auto object_stream_object = object_stream_used_entry->GetReference();

		// This can be caused by invalid source file or programming error as well
		// Let me know, if you find a document with this condition
		bool is_stream = ObjectUtils::IsType<StreamObjectPtr>(object_stream_object);
		assert(is_stream && "Object stream has incorrect type");
		if (!is_stream) {
			continue;
		}

		auto object_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(object_stream_object);

		// Sort compressed entries by index
		auto sorting_predicate = [](const XrefCompressedEntryPtr& left, const XrefCompressedEntryPtr& right) {
			return left->GetIndex() < right->GetIndex();
		};

		std::sort(entries.begin(), entries.end(), sorting_predicate);

		// Verify there are no gaps
		types::size_type current_verify_index = 0;
		for (auto it = entries.begin(); it != entries.end(); ++it, ++current_verify_index) {
			auto entry = *it;

			// How to fix this?
			auto entry_index = entry->GetIndex();
			assert(current_verify_index == entry_index && "Entry index is invalid"); entry_index;
		}

		// Initialize output streams
		std::stringstream header_stream;
		std::stringstream data_stream;

		// Reset counting indexes
		types::stream_offset current_offset = 0;

		// Store flag only for first entries
		bool first = true;

		for (auto it = entries.begin(); it != entries.end(); ++it) {
			auto entry = *it;

			auto entry_object = entry->GetReference();
			auto entry_object_string = entry_object->ToPdf();

			// Write index with offset into header stream
			if (!first) {
				header_stream << ' ';
			}

			header_stream << entry_object->GetObjectNumber();
			header_stream << ' ';
			header_stream << current_offset;

			// Write whole object into data stream
			if (!first) {
				data_stream << ' ';
			}

			data_stream << entry_object_string;

			// Advance counters
			current_offset += entry_object_string.size() + 1;

			first = false;
		}

		auto header_string = header_stream.str();
		auto data_string = data_stream.str();

		// Merge header with data
		BufferPtr new_body;
		new_body->reserve(header_string.size() + data_string.size() + 1);
		new_body->insert(new_body.end(), header_string.begin(), header_string.end());
		new_body->insert(new_body.end(), ' ');
		new_body->insert(new_body.end(), data_string.begin(), data_string.end());

		// Store recalculated data
		object_stream->SetBody(new_body);

		// Update stream metadata
		auto header = object_stream->GetHeader();

		IntegerObjectPtr new_count = make_deferred<IntegerObject>(entries.size());
		header->Remove(constant::Name::N);
		header->Insert(constant::Name::N, new_count);

		IntegerObjectPtr new_offset = make_deferred<IntegerObject>(header_string.size() + 1);
		header->Remove(constant::Name::First);
		header->Insert(constant::Name::First, new_offset);
	}
}

void FileWriter::RecalculateStreamLength(ObjectPtr obj) {
	if (!ObjectUtils::IsType<StreamObjectPtr>(obj)) {
		return;
	}

	auto stream_obj = ObjectUtils::ConvertTo<StreamObjectPtr>(obj);
	auto stream_data = stream_obj->GetBodyEncoded();
	auto stream_header = stream_obj->GetHeader();

	if (!stream_header->Contains(constant::Name::Length)) {
		IntegerObjectPtr new_length = make_deferred<IntegerObject>(stream_data->size());
		stream_header->Insert(constant::Name::Length, new_length);
		return;
	}

	auto length_obj = stream_header->FindAs<IntegerObjectPtr>(constant::Name::Length);
	if (length_obj->GetUnsignedIntegerValue() != stream_data->size()) {
		length_obj->SetValue(stream_data->size());
	}
}

void FileWriter::RecalculateStreamsLength(XrefBasePtr source) {

	// Iteration may be unordered, but I don't really care
	for (auto entry : source) {
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

XrefBasePtr FileWriter::CloneXref(FilePtr destination, XrefBasePtr source) {
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
	result->SetDirty(source->IsDirty());
	result->SetFile(source->GetFile());

	// TODO mayble also recalculate?
	result->SetLastXrefOffset(source->GetLastXrefOffset());

	for (decltype(table_size) i = 0; i < table_size; ++i) {
		auto entry = table_items[i];

		auto new_obj_number = entry->GetObjectNumber();
		auto new_gen_number = entry->GetGenerationNumber();

		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			XrefFreeEntryPtr new_entry = make_deferred<XrefFreeEntry>(
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
			XrefUsedEntryPtr new_entry = make_deferred<XrefUsedEntry>(
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
			XrefCompressedEntryPtr new_entry = make_deferred<XrefCompressedEntry>(
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

	result->SetInitialized();
	return result;
}

void FileWriter::WriteXrefObjects(FilePtr destination, XrefBasePtr source) {
	auto table_size = source->Size();
	auto table_items = source->Entries();

	for (decltype(table_size) i = 0; i < table_size; ++i) {
		auto entry = table_items[i];

		if (source->GetType() == XrefBase::Type::Stream) {
			auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(source);
			auto stream_obj = xref_stream->GetStreamObject();

			// Skip xref stream object in this serialization
			// Xref stream object itself must be written after all objects have
			// been serialized, so that it's contents can be recalculated
			// with correct offsets to each object
			if (stream_obj->GetObjectNumber() == entry->GetObjectNumber()
				&& stream_obj->GetGenerationNumber() == entry->GetGenerationNumber()) {
				continue;
			}
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			continue;
		}

		auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(entry);

		// Some used entries can be released, but they were not compacted
		if (!used_entry->InUse()) {
			continue;
		}

		auto output = destination->GetOutputStream();
		WriteEntry(output, used_entry);
		continue;
	}
}

XrefBasePtr FileWriter::CreateIncrementalXref(FilePtr source, FilePtr destination) {
	XrefFreeEntryPtr free_list_head_entry(0, constant::MAX_GENERATION_NUMBER);
	free_list_head_entry->SetFile(source);

	XrefTablePtr new_table;
	new_table->Add(free_list_head_entry);

	bool new_entries = false;
	auto chain = source->GetXrefChain();
	auto end = chain->End();
	for (auto it = chain->Begin(); *it != *end; ++(*it)) {
		auto xref = it->Value();

		auto table_size = xref->Size();
		auto table_items = xref->Entries();
		for (decltype(table_size) i = 0; i < table_size; ++i) {
			auto entry = table_items[i];

			if (!entry->IsDirty()) {
				continue;
			}

			// At least one dirty entry was found
			// meaning that a new xref section will
			// be created
			new_entries = true;

			auto new_obj_number = entry->GetObjectNumber();
			auto new_gen_number = entry->GetGenerationNumber();

			if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
				auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

				// Entry was release
				if (used_entry->InUse()) {
					continue;
				}

				auto obj = used_entry->GetReference();
				auto new_offset = used_entry->GetOffset();

				XrefUsedEntryPtr new_entry = make_deferred<XrefUsedEntry>(
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

				XrefCompressedEntryPtr new_entry = make_deferred<XrefCompressedEntry>(
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

			if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
				auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);

				XrefFreeEntryPtr new_entry = make_deferred<XrefFreeEntry>(
					new_obj_number,
					new_gen_number);

				new_entry->SetFile(source);
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
	IntegerObjectPtr new_size = make_deferred<IntegerObject>(new_table->Size());
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

	// Set document Root
	if (prev_trailer->Contains(constant::Name::Root)) {
		ContainableObjectPtr id = prev_trailer->Find(constant::Name::Root);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
		new_trailer->Insert(constant::Name::Root, cloned);
	}

	// Skip table, if there were no dirty entries
	if (new_entries) {
		auto last_xref_offset = prev_table->GetOffset();
		IntegerObjectPtr new_offset = make_deferred<IntegerObject>(last_xref_offset);
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

void FileWriter::WriteEntry(IOutputStreamPtr output, XrefUsedEntryBasePtr entry) {
	auto obj = entry->GetReference();

	if (m_recalculate_offset) {
		if (ConvertUtils<XrefUsedEntryBasePtr>::IsType<XrefUsedEntryPtr>(entry)) {
			auto used_entry = ConvertUtils<XrefUsedEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

			auto new_offset = output->GetOutputPosition();
			used_entry->SetOffset(new_offset);
		}
	}

	WriteObject(output, obj);
}

void FileWriter::WriteObject(IOutputStreamPtr output, ObjectPtr obj) {
	if (m_recalculate_offset) {
		auto new_offset = output->GetOutputPosition();
		obj->SetOffset(new_offset);
	}

	output->Write(obj->GetObjectNumber());
	output->Write(WhiteSpace::SPACE);
	output->Write(obj->GetGenerationNumber());
	output->Write(WhiteSpace::SPACE);
	output->Write("obj");
	output->Write(WhiteSpace::LINE_FEED);
	output->Write(obj->ToPdf());
	output->Write(WhiteSpace::LINE_FEED);
	output->Write("endobj");
	output->Write(WhiteSpace::LINE_FEED);
}

void FileWriter::WriteXrefOffset(IOutputStreamPtr output, types::stream_offset offset) {
	output->Write("startxref");
	output->Write(WhiteSpace::LINE_FEED);
	output->Write(offset);
	output->Write(WhiteSpace::LINE_FEED);
	output->Write("%%EOF");
	output->Write(WhiteSpace::LINE_FEED);
}

void FileWriter::WriteXref(IOutputStreamPtr output, XrefBasePtr xref) {
	if (xref->GetType() == XrefBase::Type::Table) {

		// Convert to xref table
		auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref);

		// Seperate table from content
		output << WhiteSpace::LINE_FEED;

		// Write the table
		WriteXrefTable(output, xref_table);
		WriteXrefOffset(output, xref_table->GetLastXrefOffset());
	}

	if (xref->GetType() == XrefBase::Type::Stream) {
		auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(xref);
		auto stream_obj = xref_stream->GetStreamObject();

		// After all other objects have been written, we need
		// to recalculate contents of xref stream
		xref_stream->RecalculateContent();
		RecalculateStreamLength(stream_obj);

		WriteObject(output, stream_obj);

		if (m_recalculate_offset) {
			auto stream_offset = stream_obj->GetOffset();
			xref_stream->SetLastXrefOffset(stream_offset);
		}

		WriteXrefOffset(output, xref_stream->GetLastXrefOffset());
	}
}

void FileWriter::WriteXrefTable(IOutputStreamPtr output, XrefTablePtr xref_table) {
	if (m_recalculate_offset) {
		auto offset = output->GetOutputPosition();
		xref_table->SetLastXrefOffset(offset);
	}

	auto table_size = xref_table->Size();
	auto table_items = xref_table->Entries();

	output->Write("xref");
	output->Write(WhiteSpace::LINE_FEED);

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

		output->Write(subsection_idx);
		output->Write(WhiteSpace::SPACE);
		output->Write(subsection_size);
		output->Write(WhiteSpace::LINE_FEED);

		for (decltype(subsection_size) j = 0; j < subsection_size; ++j) {
			auto entry = table_items[i + j];

			// Write free entry
			if (ConvertUtils<XrefEntryBasePtr>::IsType<XrefFreeEntryPtr>(entry)) {
				auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);
				auto formatted_object_number = GetFormattedObjectNumber(free_entry->GetNextFreeObjectNumber());
				auto formatted_generation_number = GetFormattedGenerationNumber(free_entry->GetGenerationNumber());

				output->Write(formatted_object_number);
				output->Write(WhiteSpace::SPACE);
				output->Write(formatted_generation_number);
				output->Write(WhiteSpace::SPACE);
				output->Write('f');
				output->Write(WhiteSpace::SPACE);
				output->Write(WhiteSpace::LINE_FEED);
				continue;
			}

			if (ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(entry)) {
				auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

				// Entry was released
				if (!used_entry->InUse()) {
					continue;
				}

				auto formatted_offset = GetFormattedOffset(used_entry->GetOffset());
				auto formatted_generation_number = GetFormattedGenerationNumber(used_entry->GetGenerationNumber());

				output->Write(formatted_offset);
				output->Write(WhiteSpace::SPACE);
				output->Write(formatted_generation_number);
				output->Write(WhiteSpace::SPACE);
				output->Write('n');
				output->Write(WhiteSpace::SPACE);
				output->Write(WhiteSpace::LINE_FEED);
				continue;
			}
		}

		i += subsection_size;
	}

	auto trailer = xref_table->GetTrailerDictionary();

	output->Write("trailer");
	output->Write(WhiteSpace::LINE_FEED);
	output->Write(trailer->ToPdf());
	output->Write(WhiteSpace::LINE_FEED);
}

std::string FileWriter::GetFormattedOffset(types::stream_offset offset) {
	std::stringstream ss;
	ss << setfill('0') << setw(10) << offset;
	return ss.str();
}

std::string FileWriter::GetFormattedGenerationNumber(types::ushort generation_number) {
	std::stringstream ss;
	ss << setfill('0') << setw(5) << generation_number;
	return ss.str();
}

std::string FileWriter::GetFormattedObjectNumber(types::big_uint object_number) {
	std::stringstream ss;
	ss << setfill('0') << setw(10) << object_number;
	return ss.str();
}

void FileWriter::CopyStreamContent(IInputStreamPtr source, IOutputStreamPtr destination) {
	// Determine the source file size
	source->SetPosition(0, std::ios::end);
	auto source_size_raw = source->GetPosition();
	auto source_size = ValueConvertUtils::SafeConvert<size_t>(source_size_raw);

	// Reset the positions
	source->SetPosition(0);
	destination->SetOutputPosition(0);

	// Block copy to destination
	Buffer buffer(constant::BUFFER_SIZE);
	for (size_t block_size = 0; source_size != 0; source_size -= block_size) {
		block_size = std::min<size_t>(source_size, constant::BUFFER_SIZE);
		source->Read(buffer, block_size);
		destination->Write(buffer, block_size);
	}
}

void FileWriter::CompressAndOptimize(XrefChainPtr xref) {
	// Stage: Remove objects that were freed
	RemoveFreedObjects(xref);

	// Stage: Squash xref chain into single element
	MergeXrefs(xref);

	// Stage: Remove unreferenced objects
	RemoveUnreferencedObjects(xref);

	// Stage: Search for duplicit direct objects and create indirect objects
	ExtractDuplicitDirectObjects(xref);

	// Stage: Remove duplicit indirect objects
	for (;;) {

		// Repeat until no duplicit items are found
		bool found = RemoveDuplicitIndirectObjects(xref);
		if (!found) {
			break;
		}
	}

	// Stage: Create object streams
	CompressObjects(xref);

	// Stage: Close the object number gaps
	SquashTableSpace(xref);

	// Stage: Compress xref
	CompressXref(xref);
}

void FileWriter::RemoveFreedObjects(XrefChainPtr xref) {
	// Disabled feature
	if (!m_remove_freed) {
		return;
	}

	// Iterate over all tables
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current_xref = *iterator;

		// Ignore first entry - it represents the free item linked list
		auto sorted_entries = current_xref->Entries();
		sorted_entries.erase(sorted_entries.begin());

		// For each xref entry
		for (auto entry_iterator = sorted_entries.begin(); entry_iterator != sorted_entries.end(); ) {
			auto current_entry = *entry_iterator;
			if (current_entry->InUse()) {
				entry_iterator++;
				continue;
			}

			// Remove if not used
			bool removed = current_xref->Remove(current_entry);
			assert(removed && "Could not release xref entry"); removed;

			entry_iterator = sorted_entries.erase(entry_iterator);
		}
	}
}

void FileWriter::MergeXrefs(XrefChainPtr xref) {
	// Disabled feature
	if (!m_merge_xrefs) {
		return;
	}

	// Clone original table trailer
	auto last_xref = *xref->begin();
	auto last_trailer = last_xref->GetTrailerDictionary();
	auto new_trailer = ObjectUtils::Clone<DictionaryObjectPtr>(last_trailer);
	new_trailer->SetFile(last_xref->GetFile());

	// Merge all items into single table
	XrefTablePtr new_table;
	new_table->SetTrailerDictionary(new_trailer);
	new_table->SetFile(last_xref->GetFile());

	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current = *iterator;

		// All conflicts are overwritten
		for (auto item : current) {
			new_table->Add(item);
		}
	}

	// Release all other xrefs
	xref->Clear();

	// Insert merged one
	xref->Append(new_table);
}

void FileWriter::RemoveUnreferencedObjects(XrefChainPtr xref) {
	// Disabled feature
	if (!m_remove_unreferenced) {
		return;
	}
}

void FileWriter::ExtractDuplicitDirectObjects(XrefChainPtr xref) {
	// Disabled feature
	if (!m_extract_duplicit_direct_objects) {
		return;
	}
}

bool FileWriter::RemoveDuplicitIndirectObjects(XrefChainPtr xref) {
	// Disabled feature
	if (!m_remove_duplicit_indirect_objects) {
		return false;
	}

	std::unordered_set<ObjectPtr> unique_set;
	std::unordered_map<ObjectPtr, ObjectPtr> duplicit_list;

	// Find all duplicit indirect objects
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current = *iterator;

		for (auto item : current) {

			// Accept either used and compressed entries
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(item)) {
				continue;
			}

			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(item);
			auto object = used_entry->GetReference();

			// Check if object is already in our unique list
			auto inserted = unique_set.insert(object);

			// New item was already in the set
			if (!inserted.second) {

				// In other case add to duplicit list
				auto duplicit_item = std::make_pair(object, *inserted.first);

				// The item may already be in the list, but we don't care
				auto duplicit_insert = duplicit_list.insert(duplicit_item);
			}
		}
	}

	// Skip redirection, if there were no duplicities
	if (duplicit_list.size() == 0) {
		return false;
	}

	// Redirect all references to chosen unique item
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current = *iterator;

		for (auto item : current) {

			// Accept either used and compressed entries
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(item)) {
				continue;
			}

			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(item);

			// Redirect the referenced object
			auto object = used_entry->GetReference();
			RedirectReferences(object, duplicit_list);

			// Handle compressed entry separately
			// Compressed entries have stored object number of the object stream
			// the reference is contained in. We have to redirect this reference.
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefCompressedEntryPtr>(item)) {
				continue;
			}

			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(item);
			auto original_object_stream_number = compressed_entry->GetObjectStreamNumber();
			auto original_object_stream_entry = xref->GetXrefEntry(original_object_stream_number, 0);

			// I believe that this should be true, but lets check, whether the entry is used
			// It is possible, that this won't work for some documents.
			// The reason may be, that there are more objects with the same ID and the xref chain is confused.
			bool is_used = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(original_object_stream_entry);
			assert(is_used && "Xref entry is not in use");

			if (!is_used) {
				continue;
			}

			auto original_object_stream_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(original_object_stream_entry);
			auto original_object_stream = original_object_stream_used_entry->GetReference();

			auto destination_object_iterator = duplicit_list.find(original_object_stream);
			if (destination_object_iterator == duplicit_list.end()) {
				continue;
			}

			// Remove this assertion after verifying this code really works
			assert(!"The code behind this point is not tested");

			auto destination_object = destination_object_iterator->second;
			auto destination_object_number = destination_object->GetObjectNumber();
			compressed_entry->SetObjectStreamNumber(destination_object_number);
		}
	}

	// Release all duplicit items
	for (auto item : duplicit_list) {
		auto duplicit_item = item.first;
		auto duplicit_xref_weak = duplicit_item->GetXrefEntry();
		auto duplicit_xref = duplicit_xref_weak.GetReference();

		bool released = xref->ReleaseEntry(duplicit_xref);
		assert(released && "Could not release xref entry"); released;
	}

	// Return true, if there was at least one duplicit item
	return (duplicit_list.size() != 0);
}

void FileWriter::CompressObjects(XrefChainPtr xref) {
	// Disabled feature
	if (!m_compress_objects) {
		return;
	}
}

void FileWriter::SquashTableSpace(XrefChainPtr xref) {
	// Disabled feature
	if (!m_squash_table_space) {
		return;
	}

	// Force the indirect reference initialization
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current_xref = *iterator;

		for (auto item : current_xref) {
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(item)) {
				continue;
			}

			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(item);
			auto object = used_entry->GetReference();

			// Calling this we force the reference initialization
			// Indirect references will point to their objects
			// Referenced objects keep reference to XrefEntry
			// After changing XrefEntry object number no additional
			// work should be needed.
			InitializeReferences(object);
		}
	}

	types::big_uint current_object_number = 0;
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current_xref = *iterator;

		auto sorted_entries = current_xref->Entries();
		for (auto it = sorted_entries.begin(); it != sorted_entries.end(); ++it, ++current_object_number) {
			auto entry = *it;

			auto original_object_number = entry->GetObjectNumber();
			auto new_object_number = current_object_number;

			if (original_object_number != new_object_number) {
				if (ConvertUtils<XrefEntryBasePtr>::IsType<XrefFreeEntryPtr>(entry)) {
					auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);

					auto generation_number = free_entry->GetGenerationNumber();
					auto next_free_object = free_entry->GetNextFreeObjectNumber();

					XrefFreeEntryPtr new_entry = make_deferred<XrefFreeEntry>(
						current_object_number,
						generation_number,
						next_free_object);

					new_entry->SetFile(entry->GetFile());
					new_entry->SetInitialized();

					current_xref->Remove(entry);
					current_xref->Add(new_entry);
					continue;
				}

				if (ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(entry)) {
					auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

					auto generation_number = used_entry->GetGenerationNumber();
					auto offset = used_entry->GetOffset();
					auto referenced_object = used_entry->GetReference();

					XrefUsedEntryPtr new_entry = make_deferred<XrefUsedEntry>(
						current_object_number,
						generation_number,
						offset);

					// Release the old entry before assigning the new one
					used_entry->ReleaseReference();

					new_entry->SetFile(entry->GetFile());
					new_entry->SetReference(referenced_object);
					new_entry->SetInitialized();

					current_xref->Remove(entry);
					current_xref->Add(new_entry);
					continue;
				}

				if (ConvertUtils<XrefEntryBasePtr>::IsType<XrefCompressedEntryPtr>(entry)) {
					auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);

					auto generation_number = compressed_entry->GetGenerationNumber();
					auto object_stream_number = compressed_entry->GetObjectStreamNumber();
					auto index = compressed_entry->GetIndex();
					auto referenced_object = compressed_entry->GetReference();

					XrefCompressedEntryPtr new_entry = make_deferred<XrefCompressedEntry>(
						current_object_number,
						generation_number,
						object_stream_number,
						index);

					// Release the old entry before assigning the new one
					compressed_entry->ReleaseReference();

					new_entry->SetFile(entry->GetFile());
					new_entry->SetReference(referenced_object);
					new_entry->SetInitialized();

					current_xref->Remove(entry);
					current_xref->Add(new_entry);
					continue;
				}

				assert(!"Unknown entry type");
			}
		}
	}
}

void FileWriter::CompressXref(XrefChainPtr xref) {
	// Disabled feature
	if (!m_compress_xref) {
		return;
	}
}

void FileWriter::InitializeReferences(ObjectPtr source) {
	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(source)) {
		auto source_ref = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(source);

		// This is the initialization
		auto referenced_object = source_ref->GetReferencedObject();
	} else if (ObjectUtils::IsType<MixedArrayObjectPtr>(source)) {
		auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(source);
		for (auto item : arr) {
			InitializeReferences(item);
		}
	} else if (ObjectUtils::IsType<DictionaryObjectPtr>(source)) {
		auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(source);
		for (auto item : dict) {
			InitializeReferences(item.second);
		}
	} else if (ObjectUtils::IsType<StreamObjectPtr>(source)) {
		auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(source);
		auto dict = stream->GetHeader();
		InitializeReferences(dict);
	}
}

void FileWriter::RedirectReferences(ObjectPtr source, const std::unordered_map<ObjectPtr, ObjectPtr>& duplicit_items) {
	if (ObjectUtils::IsType<IndirectObjectReferencePtr>(source)) {
		auto source_ref = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(source);
		auto referenced_object = source_ref->GetReferencedObject();

		auto found = duplicit_items.find(referenced_object);
		if (found != duplicit_items.end()) {
			source_ref->SetReferencedObject(found->second);
		}
	} else if (ObjectUtils::IsType<MixedArrayObjectPtr>(source)) {
		auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(source);
		for (auto item : arr) {
			RedirectReferences(item, duplicit_items);
		}
	} else if (ObjectUtils::IsType<DictionaryObjectPtr>(source)) {
		auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(source);
		for (auto item : dict) {
			RedirectReferences(item.second, duplicit_items);
		}
	} else if (ObjectUtils::IsType<StreamObjectPtr>(source)) {
		auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(source);
		auto dict = stream->GetHeader();
		RedirectReferences(dict, duplicit_items);
	}
}

} // syntax
} // gotchangpdf
