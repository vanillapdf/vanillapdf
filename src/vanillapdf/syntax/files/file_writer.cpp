#include "precompiled.h"

#include "utils/character.h"
#include "utils/math_utils.h"
#include "utils/license_info.h"

#include "syntax/files/file_writer.h"
#include "syntax/files/file.h"
#include "syntax/files/xref.h"

#include "syntax/utils/output_pointer.h"
#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/serialization_override_attribute.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operations.h"
#include "contents/content_stream_objects.h"

#include <fstream>
#include <unordered_set>
#include <iomanip>

namespace vanillapdf {
namespace syntax {

using std::setfill;
using std::setw;

IFileWriterObserver::~IFileWriterObserver() {}
IFileWriterObservable::~IFileWriterObservable() {}

void FileWriter::Write(FilePtr source, FilePtr destination) {

	// Terminate if the source file was not initialized
	if (!source->IsInitialized()) {
		throw FileNotInitializedException(source->GetFilename());
	}

	std::string reason;

	// Verify that configuration flags are valid
	bool valid_configuration = ValidateConfiguration(source, reason);
	if (!valid_configuration) {
		throw GeneralException(reason);
	}

	// Get destination output stream
	auto output = destination->GetInputOutputStream();

	// Write the file header
	auto header = source->GetHeader();
	WriteHeader(output, header);

	// Deeo copy of all contents from the original source document
	auto new_chain = CloneXrefChain(source, destination);

	// Compress and optimize here
	CompressAndOptimize(new_chain);

	// Apply watermark
	ApplyWatermark(new_chain);

	// Dump whole cloned contents to the output
	WriteXrefChain(output, new_chain);

	// Cleanup
	output->Flush();
}

void FileWriter::WriteIncremental(FilePtr source, FilePtr destination) {

	// Terminate if the source file was not initialized
	if (!source->IsInitialized()) {
		throw FileNotInitializedException(source->GetFilename());
	}

	auto input = source->GetInputStream();
	auto output = destination->GetInputOutputStream();

	// Write original data
	CopyStreamContent(input, output);

	// TODO check the there is newline at the end

	// Make an extra newline before starting new xref section
	output->Write(WhiteSpace::LINE_FEED);

	// Get all changed entries
	auto incremental_xref = CreateIncrementalXref(source, destination);

	// Deep clone of original xref
	auto new_xref = CloneXref(destination, incremental_xref);

	// Insert cloned table to destination xref chain
	// NOTE: This is required, so that indirect references are accessible
	auto dest_chain = destination->GetXrefChain();
	dest_chain->Append(new_xref);

	// Object stream contents has to be adjusted separately
	RecalculateObjectStreamContent(dest_chain, new_xref);

	// Stream length to be adjusted according to calculated size
	RecalculateStreamsLength(new_xref);

	// Xref contains the value that indicates number of entries contained
	RecalculateXrefSize(dest_chain, new_xref);

	// Write all body objects
	WriteXrefObjects(output, new_xref);

	// Write xref itself to output
	WriteXref(output, new_xref);

	// Cleanup
	output->Flush();
}

bool FileWriter::ValidateConfiguration(FilePtr source, std::string& reason) const {
	auto source_xref_chain = source->GetXrefChain();

	// Removing freed objects creates gaps in 
	if (m_remove_freed && !m_squash_table_space) {
		for (auto& xref : source_xref_chain) {
			if (ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(xref)) {

				std::stringstream ss;
				ss << "Flag squash table space is disabled, while the source file contains cross-reference streams." << std::endl;
				ss << "Either disable also flag remove freed objects, or enable squashing table space.";

				reason = ss.str();
				return false;
			}
		}
	}

	return true;
}

void FileWriter::RecalculateObjectStreamContent(XrefChainPtr chain, XrefBasePtr source) {

	// Disabled by the control flag
	if (!m_recalculate_object_stream_content) {
		return;
	}

	// Only matters for xref streams
	if (!ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(source)) {
		return;
	}

	// Convert to stream
	auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(source);

	// Store array of objects for every object stream
	std::unordered_map<types::big_uint, std::vector<XrefCompressedEntryPtr>> object_stream_map;

	for (auto entry : xref_stream) {

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

		// 7.5.7 Object Streams: "The generation number of an object stream and of any compressed object shall be zero."
		bool object_stream_found = chain->Contains(object_stream_number, 0);

		assert(object_stream_found && "Object stream was not found");
		if (!object_stream_found) {
			throw GeneralException("Object stream was not found");
		}

		auto object_stream_entry = chain->GetXrefEntry(object_stream_number, 0);
		bool object_stream_entry_used = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(object_stream_entry);

		assert(object_stream_entry_used && "Object stream is not in use");
		if (!object_stream_entry_used) {
			throw GeneralException("Object stream is not in use");
		}

		auto object_stream_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(object_stream_entry);
		auto object_stream_object = object_stream_used_entry->GetReference();

		// This can be caused by invalid source file or programming error as well
		// Let me know, if you find a document with this condition
		bool is_stream = ObjectUtils::IsType<StreamObjectPtr>(object_stream_object);

		assert(is_stream && "Object stream has incorrect type");
		if (!is_stream) {
			throw GeneralException("Object stream has incorrect type");
		}

		auto object_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(object_stream_object);

		// Sort compressed entries by index
		auto sorting_predicate = [](const XrefCompressedEntryPtr& left, const XrefCompressedEntryPtr& right) {
			return (left->GetIndex() < right->GetIndex());
		};

		std::sort(entries.begin(), entries.end(), sorting_predicate);

		// Verify there are no gaps
		types::size_type current_verify_index = 0;
		for (auto it = entries.begin(); it != entries.end(); ++it, ++current_verify_index) {
			auto entry = *it;

			// How to fix this?
			auto entry_index = entry->GetIndex();
			assert(current_verify_index == entry_index && "Entry index is invalid"); UNUSED(entry_index);
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

void FileWriter::FixStreamReferences(XrefChainPtr source, XrefChainPtr destination) {

	auto source_iterator = source->begin();
	auto dest_iterator = destination->begin();

	// Following for loop would crash if this is not true
	assert(source->Size() == destination->Size() && "Error in xref cloning");

	for (; dest_iterator != destination->end(); ++dest_iterator, ++source_iterator) {
		auto original_xref = *source_iterator;
		auto new_xref = *dest_iterator;

		bool is_original_stream = ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(original_xref);
		bool is_new_stream = ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(new_xref);

		// Xref streams content needs to be recalculated
		assert(!(is_original_stream ^ is_new_stream));
		if (!is_original_stream || !is_new_stream) {
			continue;
		}

		auto source_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(original_xref);
		auto dest_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(new_xref);

		auto stream_obj = source_xref_stream->GetStreamObject();
		auto stream_object_number = stream_obj->GetObjectNumber();
		auto stream_generation_number = stream_obj->GetGenerationNumber();

		assert(destination->Contains(stream_object_number, stream_generation_number)
			&& "Xref stream reference was not found in cloned xref");

		// Get cloned stream based on same object and generation number from cloned xref
		auto new_stream_entry = destination->GetXrefEntry(stream_object_number, stream_generation_number);
		auto new_stream_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(new_stream_entry);
		auto new_stream_obj = new_stream_used_entry->GetReference();
		auto new_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(new_stream_obj);

		// Fix stream object reference
		dest_xref_stream->SetStreamObject(new_stream);
	}
}

void FileWriter::RecalculateXrefPrevOffset(XrefBasePtr source, XrefBasePtr prev) {

	// Disabled by control flag
	if (!m_recalculate_offset) {
		return;
	}

	auto prev_xref_offset = prev->GetOffset();

	// Offset should be set and positive
	assert(prev_xref_offset >= 0 && "Invalid xref offset");

	auto trailer_dictionary = source->GetTrailerDictionary();
	if (!trailer_dictionary->Contains(constant::Name::Prev)) {
		IntegerObjectPtr xref_offset = make_deferred<IntegerObject>(prev_xref_offset);
		trailer_dictionary->Insert(constant::Name::Prev, xref_offset);
	}

	// Fix prev entry for all following trailers
	auto prev_entry = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Prev);
	prev_entry->SetValue(prev_xref_offset);
}

void FileWriter::RecalculateXrefHybridOffset(XrefTablePtr source) {

	// Disabled by control flag
	if (!m_recalculate_offset) {
		return;
	}

	// Nothing to update
	if (!source->HasHybridStream()) {
		return;
	}

	auto xref_stm = source->GetHybridStream();
	auto xref_stm_offset = xref_stm->GetOffset();

	// Offset should be set and positive
	assert(xref_stm_offset >= 0 && "Invalid xref offset");

	auto trailer_dictionary = source->GetTrailerDictionary();
	if (!trailer_dictionary->Contains(constant::Name::XRefStm)) {
		IntegerObjectPtr xref_offset = make_deferred<IntegerObject>(xref_stm_offset);
		trailer_dictionary->Insert(constant::Name::Prev, xref_offset);
	}

	// Fix prev entry for all following trailers
	auto xref_stm_entry = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::XRefStm);
	xref_stm_entry->SetValue(xref_stm_offset);
}

void FileWriter::RecalculateXrefStreamWidth(XrefStreamPtr source) {

	// Disabled by the control flag
	if (!m_recalculate_xref_stream_width) {
		return;
	}

	types::big_uint field1_max = 0;
	types::big_uint field2_max = 0;
	types::big_uint field3_max = 0;

	auto sorted_entries = source->Entries();
	for (auto entry : sorted_entries) {
		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);

			field1_max = std::max<decltype(field1_max)>(field1_max, 1);
			field2_max = std::max<decltype(field2_max)>(field2_max, free_entry->GetNextFreeObjectNumber());
			field3_max = std::max<decltype(field3_max)>(field3_max, free_entry->GetGenerationNumber());
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

			field1_max = std::max<decltype(field1_max)>(field1_max, 1);
			field2_max = std::max<decltype(field2_max)>(field2_max, used_entry->GetOffset());
			field3_max = std::max<decltype(field3_max)>(field3_max, used_entry->GetGenerationNumber());
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);

			field1_max = std::max<decltype(field1_max)>(field1_max, 1);
			field2_max = std::max<decltype(field2_max)>(field2_max, compressed_entry->GetObjectStreamNumber());
			field3_max = std::max<decltype(field3_max)>(field3_max, compressed_entry->GetIndex());
			continue;
		}

		assert(false && "Incorrect entry type");
	}

	types::big_uint field1_msb = MostSignificantBit(field1_max);
	types::big_uint field2_msb = MostSignificantBit(field2_max);
	types::big_uint field3_msb = MostSignificantBit(field3_max);

	types::big_uint field1_size = (field1_msb % 8 == 0) ? (field1_msb / 8) : ((field1_msb / 8) + 1);
	types::big_uint field2_size = (field2_msb % 8 == 0) ? (field2_msb / 8) : ((field2_msb / 8) + 1);
	types::big_uint field3_size = (field3_msb % 8 == 0) ? (field3_msb / 8) : ((field3_msb / 8) + 1);

	IntegerObjectPtr field1 = make_deferred<IntegerObject>(field1_size);
	IntegerObjectPtr field2 = make_deferred<IntegerObject>(field2_size);
	IntegerObjectPtr field3 = make_deferred<IntegerObject>(field3_size);

	ArrayObjectPtr<IntegerObjectPtr> new_width;
	new_width->Append(field1);
	new_width->Append(field2);
	new_width->Append(field3);

	auto stream_obj = source->GetStreamObject();
	auto stream_header = stream_obj->GetHeader();
	if (stream_header->Contains(constant::Name::Index)) {
		bool removed = stream_header->Remove(constant::Name::W);
		assert(removed); UNUSED(removed);
	}

	// Insert new width array
	stream_header->Insert(constant::Name::W, new_width);
}

void FileWriter::RecalculateXrefSize(XrefChainPtr chain, XrefBasePtr source) {

	// Disabled by the control flag
	if (!m_recalculate_xref_size) {
		return;
	}

	// Special case for hybrid-reference streams
	// 7.5.8.4 Compatibility with Applications That Do Not Support Compressed Reference Streams
	// The Size entry of the trailer shall be large enough to include all objects,
	// including those defined in the cross-reference stream referenced by the XRefStm entry.

	// Recalculate hybrid xref size as well
	types::size_type new_size = source->Size();
	if (ConvertUtils<XrefBasePtr>::IsType<XrefTablePtr>(source)) {
		auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(source);

		if (xref_table->HasHybridStream()) {
			auto hybrid_stream = xref_table->GetHybridStream();
			auto prev_xref = FindPreviousXref(chain, source);

			// This is ambiguous "large enough to include all objects"
			auto new_hybrid_size = prev_xref->Size();

			auto hybrid_stream_trailer = hybrid_stream->GetTrailerDictionary();
			if (!hybrid_stream_trailer->Contains(constant::Name::Size)) {
				IntegerObjectPtr new_hybrid_size_obj = make_deferred<IntegerObject>(new_hybrid_size);
				hybrid_stream_trailer->Insert(constant::Name::Size, new_hybrid_size_obj);
			}

			auto hybrid_size = hybrid_stream_trailer->FindAs<IntegerObjectPtr>(constant::Name::Size);
			hybrid_size->SetValue(new_hybrid_size);

			// I do not understand why this works, but all sample documents have this condition
			// Not a word is mentioned in the PDF specification and none of the other open-source
			// frameworks support writing hybrid xref sections.
			new_size += new_hybrid_size;
		}
	}

	auto trailer_dictionary = source->GetTrailerDictionary();
	if (!trailer_dictionary->Contains(constant::Name::Size)) {
		IntegerObjectPtr xref_size_obj = make_deferred<IntegerObject>(new_size);
		trailer_dictionary->Insert(constant::Name::Size, xref_size_obj);
	}

	auto size = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Size);
	size->SetValue(new_size);
}

void FileWriter::RecalculateStreamLength(StreamObjectPtr obj) {

	// Disabled by the control flag
	if (!m_recalculate_stream_size) {
		return;
	}

	auto stream_data = obj->GetBodyEncoded();
	auto stream_header = obj->GetHeader();

	if (!stream_header->Contains(constant::Name::Length)) {
		IntegerObjectPtr new_length = make_deferred<IntegerObject>(stream_data->size());
		stream_header->Insert(constant::Name::Length, new_length);
		return;
	}

	auto length_obj = stream_header->FindAs<IntegerObjectPtr>(constant::Name::Length);
	length_obj->SetValue(stream_data->size());
}

void FileWriter::RecalculateStreamsLength(XrefBasePtr source) {

	// Disabled by the control flag
	if (!m_recalculate_stream_size) {
		return;
	}

	// Iteration may be unordered, but I don't really care
	for (auto entry : source) {
		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
			auto new_obj = used_entry->GetReference();

			if (ObjectUtils::IsType<StreamObjectPtr>(new_obj)) {
				auto stream_obj = ObjectUtils::ConvertTo<StreamObjectPtr>(new_obj);
				RecalculateStreamLength(stream_obj);
			}

			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);
			auto new_obj = compressed_entry->GetReference();

			// 7.5.7 The following objects shall not be stored in an object stream: Stream objects
			assert(!ObjectUtils::IsType<StreamObjectPtr>(new_obj));

			if (ObjectUtils::IsType<StreamObjectPtr>(new_obj)) {
				throw GeneralException("Stream object should not be inside object stream");
			}

			continue;
		}

		assert(false && "Uncrecognized entry type");
	}
}

XrefBasePtr FileWriter::FindPreviousXref(XrefChainPtr chain, XrefBasePtr source) {

	bool first_xref = true;
	auto prev_xref_iterator = chain->begin();
	for (auto it = chain->begin(); it != chain->end(); ++it) {
		auto current_xref = *it;

		if (Identity(current_xref, source)) {
			if (first_xref) {
				throw GeneralException("First xref does not have a previous entry");
			}

			return *prev_xref_iterator;
		}

		if (!first_xref) {

			// Keep track of the previous entry
			prev_xref_iterator++;
		}

		first_xref = false;
	}

	throw GeneralException("Could not find previous xref");
}

XrefChainPtr FileWriter::CloneXrefChain(FilePtr source, FilePtr destination) {

	auto source_chain = source->GetXrefChain();
	auto dest_chain = destination->GetXrefChain();

	for (auto it = source_chain->begin(); it != source_chain->end(); ++it) {
		auto original_xref = *it;

		// Deep clone of original xref
		auto new_xref = CloneXref(destination, original_xref);

		// Insert cloned table to destination xref chain
		// NOTE: This is required, so that indirect references are accessible
		dest_chain->Append(new_xref);
	}

	// Stream object references needs to be fixed separately
	FixStreamReferences(source_chain, dest_chain);

	// All xref entries are stored in reversed order than it is written to file
	dest_chain->Reverse();

	return dest_chain;
}

XrefBasePtr FileWriter::CloneXref(FilePtr destination, XrefBasePtr source) {

	// Create cloned xref table
	// Xref base has no default constructor, therefore it is initialized with table
	XrefBasePtr result = XrefTablePtr();

	if (source->GetType() == XrefBase::Type::Stream) {
		auto source_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(source);
		auto source_stream_obj = source_stream->GetStreamObject();

		// We are reusing the original stream object

		XrefStreamPtr new_stream;
		new_stream->SetStreamObject(source_stream_obj);

		result = new_stream;
	}

	auto table_size = source->Size();
	auto table_items = source->Entries();

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

	if (source->GetType() == XrefBase::Type::Table) {
		auto source_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(source);
		auto result_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(result);

		if (source_table->HasHybridStream()) {
			auto source_hybrid_stream = source_table->GetHybridStream();
			auto source_hybrid_stream_object = source_hybrid_stream->GetStreamObject();
			auto source_hybrid_stream_object_number = source_hybrid_stream_object->GetObjectNumber();
			//auto source_hybrid_stream_generation_number = source_hybrid_stream_object->GetGenerationNumber();

			// TODO the entry may be somewhere in the chain
			//auto destination_xref_chain = destination->GetXrefChain();
			//auto destination_hybrid_entry = destination_xref_chain->GetXrefEntry(source_hybrid_stream_object_number, source_hybrid_stream_generation_number);
			auto destination_hybrid_entry = result_table->Find(source_hybrid_stream_object_number);
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(destination_hybrid_entry)) {
				throw GeneralException("Destination xref is not in use");
			}

			auto destination_hybrid_used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(destination_hybrid_entry);
			auto destination_hybrid_object = destination_hybrid_used_entry->GetReference();
			if (!ConvertUtils<ObjectPtr>::IsType<StreamObjectPtr>(destination_hybrid_object)) {
				throw GeneralException("Cloned hybrid xref is not a stream");
			}

			auto cloned_hybrid_xref = CloneXref(destination, source_hybrid_stream);
			auto cloned_hybrid_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(cloned_hybrid_xref);

			auto destination_hybrid_stream = ConvertUtils<ObjectPtr>::ConvertTo<StreamObjectPtr>(destination_hybrid_object);
			cloned_hybrid_stream->SetStreamObject(destination_hybrid_stream);

			result_table->SetHybridStream(cloned_hybrid_stream);
		}
	}

	// Set trailer
	auto new_trailer = CloneTrailerDictionary(destination, source);

	result->SetTrailerDictionary(new_trailer);
	result->SetOffset(source->GetOffset());
	result->SetDirty(source->IsDirty());
	result->SetFile(destination);

	// TODO mayble also recalculate?
	result->SetLastXrefOffset(source->GetLastXrefOffset());
	result->SetInitialized();
	return result;
}

void FileWriter::WriteXrefObjects(IOutputStreamPtr output, XrefBasePtr source) {
	auto table_size = source->Size();
	auto table_items = source->Entries();

	for (decltype(table_size) i = 0; i < table_size; ++i) {
		auto entry = table_items[i];

		if (source->GetType() == XrefBase::Type::Table) {
			auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(source);

			if (xref_table->HasHybridStream()) {
				auto hybrid_stream = xref_table->GetHybridStream();
				auto hybrid_stream_object = hybrid_stream->GetStreamObject();

				// Skip xref stream object in this serialization
				// Xref stream object itself must be written after all objects have
				// been serialized, so that it's contents can be recalculated
				// with correct offsets to each object
				if (hybrid_stream_object->GetObjectNumber() == entry->GetObjectNumber()
					&& hybrid_stream_object->GetGenerationNumber() == entry->GetGenerationNumber()) {
					continue;
				}
			}
		}

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

		WriteEntry(output, used_entry);
		continue;
	}
}

DictionaryObjectPtr FileWriter::CloneTrailerDictionary(FilePtr source, XrefBasePtr xref) {

	// Construct new trailer for latest xref table
	DictionaryObjectPtr new_trailer;

	// Set size of new entries
	IntegerObjectPtr new_size = make_deferred<IntegerObject>(xref->Size());
	new_trailer->Insert(constant::Name::Size, new_size);

	auto source_trailer = xref->GetTrailerDictionary();

	// Set document ID
	if (source_trailer->Contains(constant::Name::ID)) {
		ContainableObjectPtr id = source_trailer->Find(constant::Name::ID);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(id);
		new_trailer->Insert(constant::Name::ID, cloned);
	}

	// Set document Info
	if (source_trailer->Contains(constant::Name::Info)) {
		ContainableObjectPtr info = source_trailer->Find(constant::Name::Info);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(info);
		new_trailer->Insert(constant::Name::Info, cloned);
	}

	// Set document Root
	if (source_trailer->Contains(constant::Name::Root)) {
		ContainableObjectPtr root = source_trailer->Find(constant::Name::Root);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(root);
		new_trailer->Insert(constant::Name::Root, cloned);
	}

	// Set reference to hybrid stream
	if (source_trailer->Contains(constant::Name::XRefStm)) {
		ContainableObjectPtr xref_stm = source_trailer->Find(constant::Name::XRefStm);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(xref_stm);
		new_trailer->Insert(constant::Name::XRefStm, cloned);
	}

	// Set reference to previous xref entry
	if (source_trailer->Contains(constant::Name::Prev)) {
		ContainableObjectPtr prev = source_trailer->Find(constant::Name::Prev);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(prev);
		new_trailer->Insert(constant::Name::Prev, cloned);
	}

	// Xref stream properties

	// Set the type of object - usually XRef
	if (source_trailer->Contains(constant::Name::Type)) {
		ContainableObjectPtr type = source_trailer->Find(constant::Name::Type);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(type);
		new_trailer->Insert(constant::Name::Type, cloned);
	}

	// Set index of objects contained within
	if (source_trailer->Contains(constant::Name::Index)) {
		ContainableObjectPtr index = source_trailer->Find(constant::Name::Index);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(index);
		new_trailer->Insert(constant::Name::Index, cloned);
	}

	// Set byte width of the fields within compressed stream
	if (source_trailer->Contains(constant::Name::W)) {
		ContainableObjectPtr width = source_trailer->Find(constant::Name::W);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(width);
		new_trailer->Insert(constant::Name::W, cloned);
	}

	// Set stream length
	if (source_trailer->Contains(constant::Name::Length)) {
		ContainableObjectPtr length = source_trailer->Find(constant::Name::Length);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(length);
		new_trailer->Insert(constant::Name::Length, cloned);
	}

	// Set compression filter
	if (source_trailer->Contains(constant::Name::Filter)) {
		ContainableObjectPtr filter = source_trailer->Find(constant::Name::Filter);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(filter);
		new_trailer->Insert(constant::Name::Filter, cloned);
	}

	// Set compression parameters
	if (source_trailer->Contains(constant::Name::DecodeParms)) {
		ContainableObjectPtr parms = source_trailer->Find(constant::Name::DecodeParms);
		ContainableObjectPtr cloned = ObjectUtils::Clone<ContainableObjectPtr>(parms);
		new_trailer->Insert(constant::Name::DecodeParms, cloned);
	}

	// Add encryption entry to trailer
	if (source->IsEncrypted()) {
		ObjectPtr obj = source->GetEncryptionDictionary();
		bool is_dictionary = ObjectUtils::IsType<DictionaryObjectPtr>(obj);

		assert(is_dictionary);
		if (is_dictionary) {
			DictionaryObjectPtr encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(obj);
			DictionaryObjectPtr cloned = ObjectUtils::Clone<DictionaryObjectPtr>(encryption_dictionary);
			new_trailer->Insert(constant::Name::Encrypt, encryption_dictionary);
		}
	}

	new_trailer->SetFile(source);
	return new_trailer;
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

	// Set trailer
	auto source_xref = chain->Begin()->Value();
	auto new_trailer = CloneTrailerDictionary(destination, source_xref);

	// Skip table, if there were no dirty entries
	if (new_entries) {
		auto last_xref_offset = source_xref->GetOffset();
		IntegerObjectPtr new_offset = make_deferred<IntegerObject>(last_xref_offset);
		new_trailer->Insert(constant::Name::Prev, new_offset);
	}

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

	// If the object contains attribute, that controls it's serialization
	if (obj->ContainsAttribute(IAttribute::Type::SerializationOverride)) {

		// This was first introduced with the digital signatures support
		// Whole file first has to be written, and at the end the byte range
		// and signature value will be updated. Since we don't know the values
		// we can either write the file multiple times OR we leave some space
		// for the the values, when we later update them.

		auto base_attribute = obj->GetAttribute(IAttribute::Type::SerializationOverride);
		auto override_attribute = ConvertUtils<decltype(base_attribute)>::ConvertTo<SerializationOverrideAttributePtr>(base_attribute);
		auto override_value = override_attribute->GetValue();

		// User the overriden value
		output->Write(override_value);
		return;
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

	assert(offset >= 0 && "Attempting to write negative offset");

	output->Write("startxref");
	output->Write(WhiteSpace::LINE_FEED);
	output->Write(offset);
	output->Write(WhiteSpace::LINE_FEED);
	output->Write("%%EOF");
	output->Write(WhiteSpace::LINE_FEED);
}

void FileWriter::WriteHeader(IOutputStreamPtr output, HeaderPtr header) {

	auto version = header->GetVersion();

	output->Write("%PDF-1.");
	output->Write(static_cast<int32_t>(version));
	output->Write(WhiteSpace::LINE_FEED);
}

void FileWriter::WriteXrefChain(IOutputStreamPtr output, XrefChainPtr chain) {

	bool first_xref = true;
	auto prev_xref_iterator = chain->begin();
	for (auto it = chain->begin(); it != chain->end(); ++it) {
		auto new_xref = *it;

		if (!first_xref) {

			// Update offset to the previous xref section
			RecalculateXrefPrevOffset(new_xref, *prev_xref_iterator);

			// Move the iterator to the next element
			prev_xref_iterator++;
		}

		// Object stream contents has to be adjusted separately
		RecalculateObjectStreamContent(chain, new_xref);

		// Stream length to be adjusted according to calculated size
		RecalculateStreamsLength(new_xref);

		// Xref contains the value that indicates number of entries contained
		RecalculateXrefSize(chain, new_xref);

		// Write all body objects
		WriteXrefObjects(output, new_xref);

		// Write xref to output
		WriteXref(output, new_xref);

		// This flag is used to skip the first iteration
		first_xref = false;
	}
}

void FileWriter::WriteXref(IOutputStreamPtr output, XrefBasePtr xref) {

	if (xref->GetType() == XrefBase::Type::Table) {

		// Convert to xref table
		auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref);

		if (xref_table->HasHybridStream()) {
			auto hybrid_stream = xref_table->GetHybridStream();
			auto hybrid_stream_object = hybrid_stream->GetStreamObject();

			// After all other objects have been written, we need
			// to recalculate contents of xref stream
			hybrid_stream->RecalculateContent();

			// Stream has its length specified in the dictionary
			RecalculateStreamLength(hybrid_stream_object);

			// Write the stream object itself
			WriteObject(output, hybrid_stream_object);

			// Xref contains the value that indicates number of entries contained
			// Hybrid offset has to be recalculated after all objects have been written
			RecalculateXrefHybridOffset(xref_table);
		}

		// Seperate table from content
		output << WhiteSpace::LINE_FEED;

		// Write the table
		WriteXrefTable(output, xref_table);
		WriteXrefOffset(output, xref_table->GetLastXrefOffset());
	}

	if (xref->GetType() == XrefBase::Type::Stream) {
		auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(xref);
		auto stream_obj = xref_stream->GetStreamObject();


		// Find new minimal width to fit offsets
		RecalculateXrefStreamWidth(xref_stream);

		// After all other objects have been written, we need
		// to recalculate contents of xref stream
		xref_stream->RecalculateContent();

		// Stream has its length specified in the dictionary
		RecalculateStreamLength(stream_obj);

		// Write the stream object itself
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
		xref_table->SetOffset(offset);
	}

	auto table_size = xref_table->Size();
	auto table_items = xref_table->Entries();

	output->Write("xref");
	output->Write(WhiteSpace::LINE_FEED);

	for (decltype(table_size) i = 0; i < table_size;) {
		auto first = table_items[i];
		auto subsection_idx = first->GetObjectNumber();

		types::size_type subsection_size = 1;
		for (decltype(i) j = 1; j < table_size - i; ++j, ++subsection_size) {
			auto next_entry = table_items[i + j];
			if (next_entry->GetObjectNumber() != vanillapdf::SafeAddition<types::big_uint>(subsection_idx, j)) {
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
	auto trailer_pdf = trailer->ToPdf();

	output->Write("trailer");
	output->Write(WhiteSpace::LINE_FEED);
	output->Write(trailer_pdf);
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

	// Block copy to destination
	Buffer buffer(constant::BUFFER_SIZE);

	for (;;) {
		auto read = source->Read(buffer, buffer.size());
		if (read == 0) {
			break;
		}

		destination->Write(buffer, read);
	}
}

void FileWriter::ApplyWatermark(XrefChainPtr chain) {

	// Watermark only for trial version
	if (LicenseInfo::IsValid()) {
		return;
	}

	auto xref = chain->Begin()->Value();
	auto trailer_dictionary = xref->GetTrailerDictionary();

	// File has not root entry
	if (!trailer_dictionary->Contains(constant::Name::Root)) {
		return;
	}

	// File has not root entry
	OutputDictionaryObjectPtr root;
	if (!trailer_dictionary->TryFindAs(constant::Name::Root, root)) {
		return;
	}

	OutputDictionaryObjectPtr pages;
	if (!root->TryFindAs(constant::Name::Pages, pages)) {
		return;
	}

	OutputArrayObjectPtr<DictionaryObjectPtr> kids;
	if (!pages->TryFindAs(constant::Name::Kids, kids)) {
		return;
	}

	for (auto kid : *kids) {
		ApplyWatermarkPageNode(kid);
	}
}

void FileWriter::ApplyWatermarkPageNode(DictionaryObjectPtr obj) {

	if (obj->Contains(constant::Name::Kids)) {
		auto kids = obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);

		for (auto kid : *kids) {
			ApplyWatermarkPageNode(kid);
		}
	}

	if (obj->Contains(constant::Name::Type)) {
		auto type = obj->FindAs<NameObjectPtr>(constant::Name::Type);
		auto equal = type->Equals(constant::Name::Page) || type->Equals(constant::Name::Pages);

		assert(equal && "Just take a look, the type should be page"); UNUSED(equal);
	}

	if (obj->Contains(constant::Name::Contents)) {
		auto contents = obj->Find(constant::Name::Contents);

		if (ObjectUtils::IsType<StreamObjectPtr>(contents)) {
			auto content_stream = ObjectUtils::ConvertTo<StreamObjectPtr>(contents);

			ApplyWatermarkContentStream(content_stream);
		}

		if (ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(contents)) {
			auto content_array = ObjectUtils::ConvertTo<ArrayObjectPtr<StreamObjectPtr>>(contents);
			auto content_array_size = content_array->Size();

			if (content_array_size > 0) {
				auto last_stream = content_array->At(content_array_size - 1);
				ApplyWatermarkContentStream(last_stream);
			}
		}
	}
}

void FileWriter::ApplyWatermarkContentStream(StreamObjectPtr obj) {

	auto body = obj->GetBody();
	auto input_stream = body->ToInputStream();

	contents::BaseInstructionCollectionPtr instructions;

	std::vector<ObjectPtr> begin_text_parameters;
	auto begin_text_operation = make_deferred<contents::OperationBeginText>(begin_text_parameters);
	instructions->push_back(begin_text_operation);

	auto watermark_text = make_deferred<LiteralStringObject>("This file dude!");

	std::vector<ObjectPtr> text_show_parameters;
	text_show_parameters.push_back(watermark_text);

	auto text_show_operation = make_deferred<contents::OperationTextShow>(text_show_parameters);
	instructions->push_back(text_show_operation);

	std::vector<ObjectPtr> end_text_parameters;
	auto end_text_operation = make_deferred<contents::OperationEndText>(end_text_parameters);
	instructions->push_back(end_text_operation);

	std::stringstream ss;
	for (auto instruction : instructions) {
		ss << instruction->ToPdf() << std::endl;
	}

	auto watermark_body = ss.str();
	body->insert(body.end(), watermark_body.begin(), watermark_body.end());
}

void FileWriter::CompressAndOptimize(XrefChainPtr xref) {

	// Stage: Squash xref chain into single element
	MergeXrefs(xref);

	// Stage: Remove unreferenced objects
	RemoveUnreferencedObjects(xref);

	// Stage: Search for duplicit direct objects and create indirect objects
	ExtractDuplicitDirectObjects(xref);

	// Stage: Remove duplicit indirect objects
	RemoveAllDuplicitIndirectObjects(xref);

	// Stage: Create object streams
	CompressObjects(xref);

	// Stage: Remove objects that were freed
	RemoveFreedObjects(xref);

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
		for (auto entry_iterator = sorted_entries.begin(); entry_iterator != sorted_entries.end(); ++entry_iterator) {
			auto current_entry = *entry_iterator;
			if (current_entry->InUse()) {
				continue;
			}

			// Remove if not used
			bool removed = current_xref->Remove(current_entry);
			assert(removed && "Could not release xref entry"); UNUSED(removed);
		}
	}
}

void FileWriter::MergeXrefs(XrefChainPtr xref) {

	// Disabled feature
	if (!m_merge_xrefs) {
		return;
	}

	// Nothing to do
	if (xref->Empty()) {
		return;
	}

	// Merge all items into single table
	XrefBasePtr new_xref = XrefTablePtr();

	// If the chain contains xref stream, switch to merged xref to stream as well
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current = *iterator;

		// Xref streams needs to be handled differenty.
		// They may contain compressed entries,
		// that cannot be stored int the table
		if (ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(current)) {
			auto last_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(current);
			auto last_xref_stream_object = last_xref_stream->GetStreamObject();

			// TODO
			// I am reusing existing stream object
			// It could cause some side-effects in the future,
			// so take a deeper look at the problems, that may arise.
			XrefStreamPtr new_xref_stream;
			new_xref_stream->SetStreamObject(last_xref_stream_object);

			new_xref = new_xref_stream;

			break;
		}

		if (ConvertUtils<XrefBasePtr>::IsType<XrefTablePtr>(current)) {
			auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(current);
			if (xref_table->HasHybridStream()) {
				auto hybrid_stream = xref_table->GetHybridStream();
				auto hybrid_stream_object = hybrid_stream->GetStreamObject();

				XrefStreamPtr new_xref_stream;
				new_xref_stream->SetStreamObject(hybrid_stream_object);
				new_xref = new_xref_stream;
				break;
			}
		}
	}

	// Merge all xref entries into freshly created one
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current = *iterator;

		// All conflicts are overwritten
		for (auto item : current) {
			new_xref->Add(item);
		}

		if (ConvertUtils<XrefBasePtr>::IsType<XrefTablePtr>(current)) {
			auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(current);
			if (xref_table->HasHybridStream()) {
				auto hybrid_stream = xref_table->GetHybridStream();

				for (auto item : hybrid_stream) {
					new_xref->Add(item);
				}
			}
		}
	}

	// Clone original table trailer
	auto last_xref = *xref->begin();
	auto weak_file = last_xref->GetFile();
	auto file = weak_file.GetReference();

	// The trailer should be correctly duplicated in the CloneXref method
	auto last_trailer = CloneTrailerDictionary(file, last_xref);
	auto new_trailer = new_xref->GetTrailerDictionary();

	// Merge contents of the last xref section into new trailer
	new_trailer->Merge(last_trailer);

	new_xref->SetFile(weak_file);
	new_xref->SetInitialized();

	// Release all other xrefs
	xref->Clear();

	// Insert merged one
	xref->Append(new_xref);
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

void FileWriter::RemoveAllDuplicitIndirectObjects(XrefChainPtr xref) {

	// Disabled feature
	if (!m_remove_duplicit_indirect_objects) {
		return;
	}

	for (;;) {

		// Repeat until no duplicit items are found
		bool found = RemoveDuplicitIndirectObjects(xref);
		if (!found) {
			break;
		}
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
				duplicit_list.insert(duplicit_item);
			}
		}
	}

	// Skip redirection, if there were no duplicities
	if (duplicit_list.size() == 0) {
		return false;
	}

	// Redirect all references to chosen unique item
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current_xref = *iterator;

		for (auto item : current_xref) {

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

		// Trailer may contain references to Root or Info dictionary
		auto trailer_dictionary = current_xref->GetTrailerDictionary();
		RedirectReferences(trailer_dictionary, duplicit_list);
	}

	// Release all duplicit items
	for (auto item : duplicit_list) {
		auto duplicit_item = item.first;
		auto duplicit_xref_weak = duplicit_item->GetXrefEntry();
		auto duplicit_xref = duplicit_xref_weak.GetReference();

		bool released = xref->ReleaseEntry(duplicit_xref);
		assert(released && "Could not release xref entry"); UNUSED(released);
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

			// Initialize used and compressed entries as well
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(item)) {
				continue;
			}

			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(item);
			auto object = used_entry->GetReference();

			// Calling this we force the reference initialization
			// Indirect references will point to their objects
			// Referenced objects keep reference to XrefEntry
			// After changing XrefEntry object number no additional
			// work should be needed.
			InitializeReferences(object);
		}

		// Initialize also trailer, which may contain references to Root or Info dictionary
		auto trailer_dictionary = current_xref->GetTrailerDictionary();
		InitializeReferences(trailer_dictionary);
	}

	// Maps the old object number to the updated ones
	std::unordered_map<types::big_uint, types::big_uint> squash_object_map;

	types::big_uint current_object_number = 0;
	for (auto iterator = xref->begin(); iterator != xref->end(); ++iterator) {
		auto current_xref = *iterator;

		auto sorted_entries = current_xref->Entries();
		for (auto it = sorted_entries.begin(); it != sorted_entries.end(); ++it, ++current_object_number) {
			auto entry = *it;

			auto original_object_number = entry->GetObjectNumber();
			auto new_object_number = current_object_number;

			if (original_object_number == new_object_number) {
				continue;
			}

			// Store the object mapping for later object number fixups
			auto remapped_object = std::make_pair(original_object_number, new_object_number);
			auto inserted = squash_object_map.insert(remapped_object);
			assert(inserted.second && "Could not insert object mapping"); UNUSED(inserted);

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

				bool removed = current_xref->Remove(entry);
				assert(removed && "Could not release xref entry"); UNUSED(removed);

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

				bool removed = current_xref->Remove(entry);
				assert(removed && "Could not release xref entry"); UNUSED(removed);

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

				bool removed = current_xref->Remove(entry);
				assert(removed && "Could not release xref entry"); UNUSED(removed);

				current_xref->Add(new_entry);
				continue;
			}

			assert(!"Unknown entry type");
		}
	}

	// Fix object stream numbers for compressed xref entries
	// We need to fix these after all objects have been moved
	for (auto& item : xref) {

		// For every xref
		for (auto& xref_entry : item) {

			// Accept only compressed entries
			if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefCompressedEntryPtr>(xref_entry)) {
				continue;
			}

			// Convert to compressed entry
			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(xref_entry);

			// Get the old object stream number
			auto old_object_stream_number = compressed_entry->GetObjectStreamNumber();

			// Find the mapped value
			auto new_object_stream_number = squash_object_map.find(old_object_stream_number);

			// Object stream was not remapped
			if (new_object_stream_number == squash_object_map.end()) {
				continue;
			}

			// Set the new object stream number
			compressed_entry->SetObjectStreamNumber(new_object_stream_number->second);
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
} // vanillapdf
