#include "precompiled.h"
#include "syntax/files/xref.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"
#include "syntax/parsers/parser.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include <iomanip>

namespace gotchangpdf {
namespace syntax {

XrefStream::~XrefStream() {
	_stream->Unsubscribe(this);
}

XrefBase::~XrefBase() {
	for (auto item : _entries) {
		item.second->Unsubscribe(this);
	}
}

bool XrefEntryBase::operator<(const XrefEntryBase& other) const {
	if (_obj_number != other._obj_number)
		return _obj_number < other._obj_number;

	if (_gen_number != other._gen_number)
		return _gen_number < other._gen_number;

	return false;
}

void XrefUsedEntry::SetReference(ObjectPtr ref) {
	_reference->Unsubscribe(this);
	_reference->ClearXrefEntry();

	_reference = ref;

	_reference->Subscribe(this);
	_reference->SetXrefEntry(this);

	if (IsInitialized())
		SetDirty();
}

void XrefUsedEntry::Initialize(void) {
	if (IsInitialized()) {
		return;
	}

	auto locked_file = _file.lock();
	if (!locked_file)
		throw FileDisposedException();

	auto input = locked_file->GetInputStream();
	auto rewind_pos = input->tellg();
	SCOPE_GUARD_CAPTURE_VALUES(input->seekg(rewind_pos));
	Parser parser(_file, *input);

	types::big_uint obj_number = 0;
	types::ushort gen_number = 0;
	auto object = parser.ReadIndirectObject(_offset, obj_number, gen_number);
	assert(_obj_number == obj_number);
	assert(_gen_number == gen_number);

	SetReference(object);
	SetInitialized();
}

void XrefCompressedEntry::SetReference(ObjectPtr ref) {
	_reference->Unsubscribe(this);
	_reference->ClearXrefEntry();

	_reference = ref;

	_reference->Subscribe(this);
	_reference->SetXrefEntry(this);

	if (IsInitialized())
		SetDirty();
}

void XrefCompressedEntry::Initialize(void) {
	if (IsInitialized()) {
		return;
	}

	auto locked_file = _file.lock();
	if (!locked_file)
		throw FileDisposedException();

	auto chain = locked_file->GetXrefChain();
	auto stm = locked_file->GetIndirectObject(_object_stream_number, 0);

	auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(stm);
	auto header = converted->GetHeader();
	auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
	auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
	auto body = converted->GetBody();

	auto stream = body->ToStringStream();
	Parser parser(_file, stream);
	auto stream_entries = parser.ReadObjectStreamEntries(first->GetUnsignedIntegerValue(), size->SafeConvert<size_t>());
	for (auto stream_entry : stream_entries) {
		auto entry_object_number = stream_entry.object_number;
		auto entry_object = stream_entry.object;

		auto stream_entry_xref = chain->GetXrefEntry(entry_object_number, 0);

		assert(stream_entry_xref->GetUsage() == XrefEntryBase::Usage::Compressed && "Stream entry has different usage than Compressed");
		if (stream_entry_xref->GetUsage() != XrefEntryBase::Usage::Compressed) {
			continue;
		}

		auto stream_compressed_entry_xref = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(stream_entry_xref);
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
			auto free_entry = XrefUtils::ConvertTo<XrefFreeEntryPtr>(entry);

			WriteValue(ss, 0, *field1_size);
			WriteValue(ss, free_entry->GetNextFreeObjectNumber(), *field2_size);
			WriteValue(ss, free_entry->GetGenerationNumber(), *field3_size);
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Used) {
			auto used_entry = XrefUtils::ConvertTo<XrefUsedEntryPtr>(entry);

			WriteValue(ss, 1, *field1_size);
			WriteValue(ss, used_entry->GetOffset(), *field2_size);
			WriteValue(ss, used_entry->GetGenerationNumber(), *field3_size);
			continue;
		}

		if (entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
			auto compressed_entry = XrefUtils::ConvertTo<XrefCompressedEntryPtr>(entry);

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

	_entries.erase(found);
	found->second->Unsubscribe(this);
	OnChanged();
	return true;
}

size_t XrefBase::Size(void) const noexcept { return _entries.size(); }
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
