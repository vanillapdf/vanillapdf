#include "precompiled.h"

#include "syntax/files/xref.h"
#include "syntax/exceptions/syntax_exceptions.h"


namespace gotchangpdf {
namespace syntax {

XrefStream::~XrefStream() {
	_stream->Unsubscribe(this);
}

XrefBase::~XrefBase() {
	for (auto item : _entries) {
		item->Unsubscribe(this);
	}
}

void XrefBase::ObserveeChanged(IModifyObservable*) {
	if (m_initialized) {
		SetDirty();
	}

	// Notify observers
	OnChanged();
}

void XrefStream::RecalculateContent() {
	// Recalculate only for changed streams
	if (!m_dirty) {
		return;
	}

	auto header = _stream->GetHeader();
	if (!header->Contains(constant::Name::W)) {
		throw GeneralException("Stream header does not contain width");
	}

	auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);

	assert(fields->Size() == 3);
	if (fields->Size() != 3) {
		throw GeneralException("Xref stream width does not contain three integers");
	}

	auto field1_size = fields->At(0);
	auto field2_size = fields->At(1);
	auto field3_size = fields->At(2);

	auto sorted_entries = Entries();
	std::sort(sorted_entries.begin(), sorted_entries.end());

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
		assert(removed); removed;
	}

	// Insert subsection info
	header->Insert(constant::Name::Index, subsections->Data());

	// TODO unable to encode prediction algorithms
	// so I just have to remove them
	if (header->Contains(constant::Name::DecodeParms)) {
		bool removed = header->Remove(constant::Name::DecodeParms);
		assert(removed && "Could not remove item from dictionary"); removed;
	}

	auto new_data_string = ss.str();
	BufferPtr new_data = make_deferred<Buffer>(new_data_string.begin(), new_data_string.end());
	_stream->SetBody(new_data);
}

void XrefStream::WriteValue(std::ostream& dest, types::big_uint value, int64_t width) {
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
	assert(is_free || is_used && "Adding invalid entry type to xref table");
	is_free; is_used;

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

size_t XrefBase::Size(void) const noexcept {
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

} // syntax
} // gotchangpdf
