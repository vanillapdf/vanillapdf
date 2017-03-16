#include "precompiled.h"
#include "syntax/files/xref.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"
#include "syntax/parsers/parser.h"
#include "syntax/streams/input_stream.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include <iomanip>

namespace gotchangpdf {
namespace syntax {

XrefEntryBase::XrefEntryBase(types::big_uint obj_number, types::ushort gen_number)
	: _obj_number(obj_number), _gen_number(gen_number) {
}

XrefFreeEntry::XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number)
	: XrefEntryBase(obj_number, gen_number) {
}

XrefUsedEntry::XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset)
	: XrefUsedEntryBase(obj_number, gen_number), _offset(offset) {
	_reference.reset();
}

XrefCompressedEntry::XrefCompressedEntry(
	types::big_uint obj_number,
	types::ushort gen_number,
	types::big_uint object_stream_number,
	types::size_type index)
	: XrefUsedEntryBase(obj_number, gen_number),
	_object_stream_number(object_stream_number),
	_index(index) {
	_reference.reset();
}

XrefStream::~XrefStream() {
	_stream->Unsubscribe(this);
}

XrefBase::~XrefBase() {
	for (auto item : _entries) {
		item.second->Unsubscribe(this);
	}
}

XrefUsedEntryBase::~XrefUsedEntryBase() {
	ReleaseReference(false);
}

void XrefUsedEntryBase::ObserveeChanged(IModifyObservable*) {
	if (m_initialized) {
		SetDirty();
	}

	// Notify observers
	OnChanged();
}

bool XrefEntryBase::operator<(const XrefEntryBase& other) const {
	if (_obj_number != other._obj_number) {
		return _obj_number < other._obj_number;
	}

	if (_gen_number != other._gen_number) {
		return _gen_number < other._gen_number;
	}

	return false;
}

void XrefUsedEntryBase::SetReference(ObjectPtr ref) {
	auto weak_ref_xref = ref->GetXrefEntry();

	if (weak_ref_xref.IsActive() && !weak_ref_xref.IsEmpty()) {
		auto ref_xref = weak_ref_xref.GetReference();
		assert(ref_xref == this && "Reference another owner");
	}

	ReleaseReference(true);

	_reference = ref;
	m_used = true;

	_reference->Subscribe(this);
	_reference->SetXrefEntry(this);

	if (IsInitialized()) {
		SetDirty();
	}
}

ObjectPtr XrefUsedEntryBase::GetReference(void) {
	Initialize();

	if (!InUse()) {
		return NullObject::GetInstance();
	}

	return _reference;
}

bool XrefUsedEntryBase::InUse(void) const noexcept {
	if (!m_used) {
		assert(_reference.empty() && "Unused entry contains reference");
	}

	return m_used;
}

void XrefUsedEntryBase::ReleaseReference(bool check_object_xref) {
	// Unused entries have nothing to release
	if (!InUse()) {
		return;
	}

	bool unsubscribed = _reference->Unsubscribe(this);
	assert(unsubscribed && "Could not unsubscribe"); unsubscribed;

	if (check_object_xref) {
		auto weak_ref_entry = _reference->GetXrefEntry();
		if (weak_ref_entry.IsActive() && !weak_ref_entry.IsEmpty()) {
			auto ref_entry = weak_ref_entry.GetReference();
			assert(ref_entry == this && "Reference entry has changed");
		}

		_reference->ClearXrefEntry(false);
	}

	_reference.reset();
	m_used = false;
}

void XrefUsedEntry::Initialize(void) {
	if (IsInitialized()) {
		return;
	}

	if (!_file.IsActive()) {
		throw FileDisposedException();
	}

	auto locked_file = _file.GetReference();
	auto input = locked_file->GetInputStream();
	auto rewind_pos = input->GetPosition();

	// We want to capture input by value, because it might be out of scope
	// In order to call non-const method we have to tag the lambda mutable
	auto cleanup_lambda = [input, rewind_pos]() mutable {
		input->SetPosition(rewind_pos);
	};

	SCOPE_GUARD(cleanup_lambda);

	Parser parser(_file, input);

	types::big_uint obj_number = 0;
	types::ushort gen_number = 0;
	auto object = parser.ReadIndirectObject(_offset, obj_number, gen_number);
	assert(_obj_number == obj_number);
	assert(_gen_number == gen_number);

	SetReference(object);
	SetInitialized();
}

void XrefCompressedEntry::Initialize(void) {
	if (IsInitialized()) {
		return;
	}

	if (!_file.IsActive()) {
		throw FileDisposedException();
	}

	auto locked_file = _file.GetReference();
	auto chain = locked_file->GetXrefChain();
	auto stm = locked_file->GetIndirectObject(_object_stream_number, 0);

	auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(stm);
	auto header = converted->GetHeader();
	auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
	auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
	auto body = converted->GetBody();

	auto stream = body->ToStringStream();

	InputStreamPtr input_stream(stream);
	Parser parser(_file, input_stream);
	auto stream_entries = parser.ReadObjectStreamEntries(first->GetUnsignedIntegerValue(), size->SafeConvert<size_t>());
	for (auto stream_entry : stream_entries) {
		auto entry_object_number = stream_entry.object_number;
		auto entry_object = stream_entry.object;

		auto stream_entry_xref = chain->GetXrefEntry(entry_object_number, 0);

		assert(stream_entry_xref->GetUsage() == XrefEntryBase::Usage::Compressed && "Stream entry has different usage than Compressed");
		if (stream_entry_xref->GetUsage() != XrefEntryBase::Usage::Compressed) {
			continue;
		}

		auto stream_compressed_entry_xref = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(stream_entry_xref);
		stream_compressed_entry_xref->SetReference(entry_object);
		stream_compressed_entry_xref->SetInitialized();
	}

	if (!m_initialized) {
		auto scope = locked_file->GetFilename();
		LOG_WARNING(scope) <<
			"Compressed entry number " <<
			std::to_string(GetObjectNumber()) <<
			" " <<
			GetGenerationNumber()
			<< " was supposed to be found in object stream number "
			<< GetObjectStreamNumber()
			<< " but was not found";

		LOG_WARNING(scope) << "Treating as NULL reference";
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
				IntegerObjectPtr section_index_obj(section_index);
				IntegerObjectPtr section_length_obj(section_size);
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
	IntegerObjectPtr section_index_obj(section_index);
	IntegerObjectPtr section_length_obj(section_size);
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
		header->Remove(constant::Name::DecodeParms);
	}

	auto new_data_string = ss.str();
	BufferPtr new_data(new_data_string.begin(), new_data_string.end());
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

void XrefBase::Add(XrefEntryBasePtr entry) {
	types::big_uint obj_number = entry->GetObjectNumber();
	auto found = _entries.find(obj_number);
	if (found != _entries.end()) {
		_entries.erase(found);
	}

	std::pair<types::big_uint, XrefEntryBasePtr> pair(obj_number, entry);
	_entries.insert(pair);
	entry->Subscribe(this);
	OnChanged();
}

bool XrefBase::Remove(types::big_uint obj_number) {
	auto found = _entries.find(obj_number);
	if (found == _entries.end()) {
		return false;
	}

	// Store the item reference
	auto backup_item = found->second;

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
	auto found = _entries.find(obj_number);
	if (found == _entries.end()) {
		throw ObjectMissingException(obj_number);
	}

	return found->second;
}

bool XrefBase::Contains(types::big_uint obj_number) const {
	auto found = _entries.find(obj_number);
	return (found != _entries.end());
}

std::vector<XrefEntryBasePtr> XrefBase::Entries(void) const {
	std::vector<XrefEntryBasePtr> result;
	result.reserve(_entries.size());
	std::for_each(_entries.begin(), _entries.end(), [&result](const std::pair<types::big_uint, XrefEntryBasePtr> pair) { result.push_back(pair.second); });

	// Since we moved to unordered map, sort is required
	std::sort(
		result.begin(),
		result.end(),
		[](const XrefEntryBasePtr& left, const XrefEntryBasePtr& right) {
		return left->GetObjectNumber() < right->GetObjectNumber();
	});

	return result;
}

} // syntax
} // gotchangpdf
