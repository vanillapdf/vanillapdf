#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_entry.h"

#include "syntax/parsers/parser.h"
#include "syntax/utils/name_constants.h"
#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

XrefEntryBase::XrefEntryBase(types::big_uint obj_number, types::ushort gen_number)
	: _obj_number(obj_number), _gen_number(gen_number) {
}

XrefUsedEntryBase::XrefUsedEntryBase(types::big_uint obj_number, types::ushort gen_number)
	: XrefEntryBase(obj_number, gen_number) {
	m_access_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

XrefFreeEntry::XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number)
	: XrefFreeEntry(obj_number, gen_number, 0) {
}

XrefFreeEntry::XrefFreeEntry(types::big_uint obj_number, types::ushort gen_number, types::big_uint next_free_object)
	: XrefEntryBase(obj_number, gen_number), m_next_free_object(next_free_object) {
}

XrefUsedEntry::XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number)
	: XrefUsedEntry(obj_number, gen_number, constant::BAD_OFFSET) {
}

XrefUsedEntry::XrefUsedEntry(types::big_uint obj_number, types::ushort gen_number, types::stream_offset offset)
	: XrefUsedEntryBase(obj_number, gen_number), _offset(offset) {
	_reference.reset();
}

XrefCompressedEntry::XrefCompressedEntry(types::big_uint obj_number, types::ushort gen_number)
	: XrefCompressedEntry(obj_number, gen_number, 0, 0) {
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

XrefUsedEntryBase::~XrefUsedEntryBase() {
	ReleaseReference(false);
}

void XrefUsedEntryBase::ObserveeChanged(const IModifyObservable*) {
	// Notify observers
	OnChanged();
}

void XrefUsedEntryBase::OnChanged() {
	if (m_initialized) {
		_dirty = true;
	}

	IModifyObservable::OnChanged();
}

bool XrefEntryBase::operator==(const XrefEntryBase& other) const {
	return (_obj_number == other._obj_number);
}

bool XrefEntryBase::operator!=(const XrefEntryBase& other) const {
	return (_obj_number != other._obj_number);
}

bool XrefEntryBase::operator<(const XrefEntryBase& other) const {
	return (_obj_number < other._obj_number);
}

void XrefUsedEntryBase::SetReference(ObjectPtr ref) {
	ACCESS_LOCK_GUARD(m_access_lock);

	auto weak_ref_xref = ref->GetXrefEntry();

	// TODO: This can happen with XRefStm for example
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

	ACCESS_LOCK_GUARD(m_access_lock);

	bool unsubscribed = _reference->Unsubscribe(this);
	assert(unsubscribed && "Could not unsubscribe"); UNUSED(unsubscribed);

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

	if (_offset == constant::BAD_OFFSET) {
		throw GeneralException("Xref entry data offset is not initialized");
	}

	ACCESS_LOCK_GUARD(m_access_lock);

	if (IsInitialized()) {
		return;
	}

	auto locked_file = _file.GetReference();
	auto input = locked_file->GetInputStream();

	spdlog::debug("Initializing xref USED entry {} {} at offset {}", _obj_number, _gen_number, _offset);

	// In case the we have a document with XRefStm, there are multiple entries for the same object.
	// What this means is that we are reading the same object multiple times and not sharing the reference.
	// We should try to search for such cases before actually reading from the disk.

	// Multiple xref entries needs to be synchronized locking shared property such as input stream.
	// This prevents the cases, where 2 xref entries simultaneously check if the object was loaded,
	// where both of them do not find anything yet and both of them load the indirect object from disk.
	input->ExclusiveInputLock();

	auto unlock_lambda = [&input]() {
		input->ExclusiveInputUnlock();
	};

	SCOPE_GUARD(unlock_lambda);

	auto xref_chain = locked_file->GetXrefChain(false);

	for (auto xref : xref_chain) {
		if (!xref->Contains(_obj_number)) {
			continue;
		}

		auto xref_entry = xref->Find(_obj_number);
		if (xref_entry->GetGenerationNumber() != _gen_number) {
			continue;
		}

		if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(xref_entry)) {
			continue;
		}

		auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(xref_entry);

		if (!used_entry->IsInitialized()) {
			continue;
		}

		// This could be a regular update in the increment xref section,
		// describing a new object with the same object and generation number.
		// The generation number AFAIK is only incremented when the object is released
		// and then reacquired.
		if (used_entry->GetOffset() != _offset) {
			spdlog::warn("Initializing xref USED entry {} {} from another XRef entry, however the offset is {} instead of {}", _obj_number, _gen_number, used_entry->GetOffset(), _offset);
			continue;
		}

		auto used_entry_reference = used_entry->GetReference();
		
		SetReference(used_entry_reference);
		SetInitialized();
		
		// The object reference was shared from another xref entry
		// which was already initialized, no need to read from disk
		return;
	}

	// The object was not yet loaded, read indirect object from disk

	auto rewind_pos = input->GetInputPosition();

	// I am not sure about this entire rewind functionality,
	// but it was done a long time ago by someone who was really clever
	auto rewind_lambda = [&input, rewind_pos]() {
		input->SetInputPosition(rewind_pos);
	};

	SCOPE_GUARD(rewind_lambda);

	Parser parser(_file, input);

	types::big_uint obj_number = 0;
	types::ushort gen_number = 0;
	auto object = parser.ReadIndirectObject(_offset, obj_number, gen_number);

	if (_obj_number != obj_number) {
		spdlog::warn("Object number in the XREF {} does not match the actual object {} at offset {}. Taking preference on the object", _obj_number, obj_number, _offset);
		_obj_number = obj_number;
	}

	if (_gen_number != gen_number) {
		spdlog::warn("Generation number in the XREF {} does not match the actual object {} at offset {}. Taking preference on the object", _gen_number, gen_number, _offset);
		_gen_number = gen_number;
	}

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

	ACCESS_LOCK_GUARD(m_access_lock);

	if (IsInitialized()) {
		return;
	}

	auto locked_file = _file.GetReference();
	auto chain = locked_file->GetXrefChain();
	auto stm = locked_file->GetIndirectObject(_object_stream_number, 0);

	auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(stm);
	auto header = converted->GetHeader();
	auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
	auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
	auto body = converted->GetBody();
	auto input_stream = body->ToInputStream();

	if (body->empty()) {
		throw GeneralException("Could not find data for the ObjStm " + std::to_string(_object_stream_number));
	}

	spdlog::debug("Initializing xref COMPRESSED entry {} {} inside object stream {}", _obj_number, _gen_number, _object_stream_number);

	Parser parser(_file, input_stream);
	auto stream_entries = parser.ReadObjectStreamEntries(first->GetUnsignedIntegerValue(), size->SafeConvert<types::size_type>());
	for (auto stream_entry : stream_entries) {
		auto entry_object_number = stream_entry.object_number;
		auto entry_object = stream_entry.object;

		// In the standard setup the standard search for xref entry GetXrefEntry should be sufficient.
		// However in the incrementally updated document there are multiple references to the same object,
		// so that we sometimes do get another entry, leaving this one uninitialized.
		// I could imagine a case where this is not correct, but for the sake of simplicity let's leave it out of scope.
		if (entry_object_number == GetObjectNumber() && GetGenerationNumber() == 0) {
			SetReference(entry_object);
			SetInitialized();
			continue;
		}

		auto stream_entry_xref = chain->GetXrefEntry(entry_object_number, 0);
		auto is_used = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(stream_entry_xref);

		if (!is_used) {
			spdlog::warn("Object stream {} contains unused xref entry {}", _object_stream_number, entry_object_number);
			continue;
		}

		auto stream_used_entry_xref = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(stream_entry_xref);
		stream_used_entry_xref->SetReference(entry_object);
		stream_used_entry_xref->SetInitialized();
	}

	if (!m_initialized) {
		spdlog::warn("Compressed entry number {} {} was supposed to be found in object stream number {}, but was not found. Treating as NULL reference", _obj_number, _gen_number, _object_stream_number);
	}
}

} // syntax
} // vanillapdf

namespace std {

size_t hash<vanillapdf::syntax::XrefEntryBasePtr>::operator()(const vanillapdf::syntax::XrefEntryBasePtr& entry) const {
	auto object_number = entry->GetObjectNumber();

	std::hash<decltype(object_number)> hasher;
	return hasher(object_number);
}

} // std
