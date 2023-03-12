#include "precompiled.h"

#include "syntax/files/xref.h"
#include "syntax/utils/name_constants.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/math_utils.h"

#include <bitset>

namespace vanillapdf {
namespace syntax {

XrefStream::~XrefStream() {
	_stream->Unsubscribe(this);
}

XrefBase::~XrefBase() {
	for (auto item : _entries) {
		item->Unsubscribe(this);
	}
}

void XrefBase::OnChanged() {
	if (m_initialized) {
		SetDirty();
	}

	IModifyObservable::OnChanged();
}

void XrefBase::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

void XrefStream::RecalculateContent() {

	// Recalculate only for changed streams
	if (!IsDirty()) {
		return;
	}

	auto header = _stream->GetHeader();
	if (!header->Contains(constant::Name::W)) {
		throw GeneralException("Stream header does not contain width");
	}

	auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);

	assert(fields->GetSize() == 3);
	if (fields->GetSize() != 3) {
		throw GeneralException("Xref stream width does not contain three integers");
	}

	auto field1_size = fields->GetValue(0);
	auto field2_size = fields->GetValue(1);
	auto field3_size = fields->GetValue(2);

	auto sorted_entries = Entries();
	std::sort(sorted_entries.begin(), sorted_entries.end());

	// Check and update field sizes according to maximum value
	IntegerObjectPtr field2_max;
	IntegerObjectPtr field3_max;

	for (auto entry : sorted_entries) {
		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);

			types::big_uint field2_bytes = (MostSignificantBit(free_entry->GetNextFreeObjectNumber()) / 8) + 1;
			types::big_uint field3_bytes = (MostSignificantBit(free_entry->GetGenerationNumber()) / 8) + 1;

			if (field2_bytes > field2_max->GetUnsignedIntegerValue()) {
				*field2_max = field2_bytes;
			}

			if (field3_bytes > field3_max->GetUnsignedIntegerValue()) {
				*field3_max = field3_bytes;
			}
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

			types::big_uint field2_bytes = (MostSignificantBit(used_entry->GetOffset()) / 8) + 1;
			types::big_uint field3_bytes = (MostSignificantBit(used_entry->GetGenerationNumber()) / 8) + 1;

			if (field2_bytes > field2_max->GetUnsignedIntegerValue()) {
				*field2_max = field2_bytes;
			}

			if (field3_bytes > field3_max->GetUnsignedIntegerValue()) {
				*field3_max = field3_bytes;
			}
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);

			types::big_uint field2_bytes = (MostSignificantBit(compressed_entry->GetObjectStreamNumber()) / 8) + 1;
			types::big_uint field3_bytes = (MostSignificantBit(compressed_entry->GetIndex()) / 8) + 1;

			if (field2_bytes > field2_max->GetUnsignedIntegerValue()) {
				*field2_max = field2_bytes;
			}

			if (field3_bytes > field3_max->GetUnsignedIntegerValue()) {
				*field3_max = field3_bytes;
			}
		}
	}

	if (field2_max->GetUnsignedIntegerValue() > field2_size->GetUnsignedIntegerValue()) {
		*field2_size = field2_max->GetUnsignedIntegerValue();
	}

	if (field3_max->GetUnsignedIntegerValue() > field3_size->GetUnsignedIntegerValue()) {
		*field3_size = field3_max->GetUnsignedIntegerValue();
	}

	ArrayObjectPtr<IntegerObjectPtr> subsections;
	types::big_uint section_index = 0;
	types::big_uint section_size = 0;
	types::big_uint prev_number = 0;

	std::stringstream ss;
	for (auto entry : sorted_entries) {
		types::big_uint current_number = entry->GetObjectNumber();

		if (section_size != 0) {
			auto continuous_number = SafeAddition<types::big_uint>(prev_number, 1);
			if (current_number != continuous_number) {
				IntegerObjectPtr section_index_obj = make_deferred<IntegerObject>(section_index);
				IntegerObjectPtr section_length_obj = make_deferred<IntegerObject>(section_size);
				subsections->Append(section_index_obj);
				subsections->Append(section_length_obj);

				section_index = current_number;
				section_size = 0;
			}
		}

		// Store number of previous object
		prev_number = current_number;
		section_size++;

		if (entry->GetUsage() == XrefEntryBase::Usage::Free) {
			auto free_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefFreeEntryPtr>(entry);

			WriteValue(ss, 0, *field1_size);
			WriteValue(ss, free_entry->GetNextFreeObjectNumber(), *field2_size);
			WriteValue(ss, free_entry->GetGenerationNumber(), *field3_size);
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
			auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);

			WriteValue(ss, 1, *field1_size);
			WriteValue(ss, used_entry->GetOffset(), *field2_size);
			WriteValue(ss, used_entry->GetGenerationNumber(), *field3_size);
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
			auto compressed_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(entry);

			WriteValue(ss, 2, *field1_size);
			WriteValue(ss, compressed_entry->GetObjectStreamNumber(), *field2_size);
			WriteValue(ss, compressed_entry->GetIndex(), *field3_size);
			continue;
		}

		assert(false && "Incorrect entry type");
	}

	// Insert last remaining section
	IntegerObjectPtr section_index_obj = make_deferred<IntegerObject>(section_index);
	IntegerObjectPtr section_length_obj = make_deferred<IntegerObject>(section_size);
	subsections->Append(section_index_obj);
	subsections->Append(section_length_obj);

	if (header->Contains(constant::Name::Index)) {
		bool removed = header->Remove(constant::Name::Index);
		assert(removed); UNUSED(removed);
	}

	// Insert subsection info
	header->Insert(constant::Name::Index, subsections->Data());

	// TODO unable to encode prediction algorithms
	// so I just have to remove them
	if (header->Contains(constant::Name::DecodeParms)) {
		bool removed = header->Remove(constant::Name::DecodeParms);
		assert(removed && "Could not remove item from dictionary"); UNUSED(removed);
	}

	auto new_data_string = ss.str();
	BufferPtr new_data = make_deferred_container<Buffer>(new_data_string.begin(), new_data_string.end());
	_stream->SetBody(new_data);
}

void XrefStream::WriteValue(std::ostream& dest, types::big_uint value, int64_t width) {

	// Check if the value fits inside width
	//auto shifted_value = value >> (width * 8);

	// The statement 9 >> (8 * 8) actually results in value 9.
	// C++ for some reason does ROR bits instead of SHR
	// Let's handle this using bitset, which does work properly
	std::bitset<sizeof(value) * CHAR_BIT> value_bitstet(value);
	auto shifted_value_bitset = value_bitstet >> (width * 8);
	auto shifted_value = shifted_value_bitset.to_ullong();

	// This means, that the operation would overflow
	assert(shifted_value == 0 && "Xref stream value overflow");
	if (shifted_value != 0) {
		throw GeneralException("Xref stream width is too small");
	}

	// Writes <value> as a sequence of <width> bytes into <dest>
	// starting with most significant byte
	for (decltype(width) i = width - 1; i >= 0; --i) {
		unsigned char raw_byte = (value >> (i * 8)) & 0xFF;
		dest << raw_byte;
	}
}

void XrefTable::Add(XrefEntryBasePtr entry) {
	bool is_free = ConvertUtils<XrefEntryBasePtr>::IsType<XrefFreeEntryPtr>(entry);
	bool is_used = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(entry);

	// Xref table can only stored free and used entries
	assert((is_free || is_used) && "Adding unsupported entry type into the xref table");
	if (!is_free && !is_used) {
		throw GeneralException("Adding unsupported entry type into the xref table");
	}

	// Perform the addition
	XrefBase::Add(entry);
}

void XrefBase::Add(XrefEntryBasePtr entry) {
	auto found = _entries.find(entry);
	if (found != _entries.end()) {
		_entries.erase(found);
	}

	_entries.insert(entry);
	entry->Subscribe(this);
	OnChanged();
}

bool XrefBase::Remove(XrefEntryBasePtr entry) {
	auto found = _entries.find(entry);
	if (found == _entries.end()) {
		return false;
	}

	// Store the item reference
	auto backup_item = *found;

	// Erase the entry
	_entries.erase(found);

	// Access the backed up item
	// After erase the "found" is no longer available
	backup_item->Unsubscribe(this);

	OnChanged();
	return true;
}

types::size_type XrefBase::GetSize(void) const noexcept {
	return _entries.size();
}

XrefEntryBasePtr XrefBase::Find(types::big_uint obj_number) const {
	XrefFreeEntryPtr temp = make_deferred<XrefFreeEntry>(obj_number, static_cast<types::ushort>(0));

	auto found = _entries.find(temp);
	if (found == _entries.end()) {
		throw ObjectMissingException(obj_number);
	}

	return *found;
}

bool XrefBase::Contains(types::big_uint obj_number) const {
	XrefFreeEntryPtr temp = make_deferred<XrefFreeEntry>(obj_number, static_cast<types::ushort>(0));

	auto found = _entries.find(temp);
	return (found != _entries.end());
}

std::vector<XrefEntryBasePtr> XrefBase::Entries(void) const {
	std::vector<XrefEntryBasePtr> result;
	result.reserve(_entries.size());
	std::for_each(_entries.begin(), _entries.end(), [&result](const XrefEntryBasePtr& item) { result.push_back(item); });

	// Since we moved to unordered map, sort is required
	std::sort(
		result.begin(),
		result.end(),
		[](const XrefEntryBasePtr& left, const XrefEntryBasePtr& right) {
		return left->GetObjectNumber() < right->GetObjectNumber();
	});

	return result;
}

XrefBase::iterator XrefBase::begin() {
	return _entries.begin();
}

XrefBase::const_iterator XrefBase::begin() const {
	return _entries.begin();
}

XrefBase::iterator XrefBase::end() {
	return _entries.end();
}

XrefBase::const_iterator XrefBase::end() const {
	return _entries.end();
}

XrefBase::iterator XrefBase::erase(const_iterator pos) {
	return _entries.erase(pos);
}

XrefEntryBasePtr XrefTable::Find(types::big_uint obj_number) const {
	bool contains = XrefBase::Contains(obj_number);
	if (!contains && HasHybridStream()) {
		return m_xref_stm->Find(obj_number);
	}

	return XrefBase::Find(obj_number);
}

bool XrefTable::Contains(types::big_uint obj_number) const {
	bool contains = XrefBase::Contains(obj_number);
	if (!contains && HasHybridStream()) {
		return m_xref_stm->Contains(obj_number);
	}

	return contains;
}

DictionaryObjectPtr XrefTable::GetTrailerDictionary(void) const {
	return m_trailer_dictionary;
}

void XrefTable::SetTrailerDictionary(DictionaryObjectPtr dictionary) {
	m_trailer_dictionary = dictionary;
}

bool XrefTable::HasHybridStream(void) const {
	return !m_xref_stm.empty();
}

XrefStreamPtr XrefTable::GetHybridStream(void) const {
	bool has_stream = HasHybridStream();

	assert(has_stream && "Getting unset value"); UNUSED(has_stream);
	return m_xref_stm;
}

void XrefTable::SetHybridStream(XrefStreamPtr stream) {
	m_xref_stm = stream;
}

void XrefStream::SetFile(WeakReference<File> file) noexcept {
	auto stream = GetStreamObject();
	return stream->SetFile(file);
}

WeakReference<File> XrefStream::GetFile() const noexcept {
	auto stream = GetStreamObject();
	return stream->GetFile();
}

types::stream_offset XrefStream::GetOffset() const {
	auto stream = GetStreamObject();
	return stream->GetOffset();
}

void XrefStream::SetOffset(types::stream_offset offset) {
	auto stream = GetStreamObject();
	stream->SetOffset(offset);
}

bool XrefStream::IsDirty(void) const noexcept {
	auto stream = GetStreamObject();
	return stream->IsDirty();
}

void XrefStream::SetDirty(bool dirty) noexcept {
	auto stream = GetStreamObject();
	return stream->SetDirty(dirty);
}

DictionaryObjectPtr XrefStream::GetTrailerDictionary(void) const {
	auto stream = GetStreamObject();
	return stream->GetHeader();
}

void XrefStream::SetTrailerDictionary(DictionaryObjectPtr dictionary) {
	auto stream = GetStreamObject();
	stream->SetHeader(dictionary);
}

StreamObjectPtr XrefStream::GetStreamObject(void) const {
	bool has_stream = !_stream.empty();

	assert(has_stream && "Stream not yet initialized"); UNUSED(has_stream);
	return _stream;
}

void XrefStream::SetStreamObject(StreamObjectPtr stream) {
	_stream->Unsubscribe(this);
	_stream = stream;
	_stream->Subscribe(this);
}

} // syntax
} // vanillapdf
