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

	auto locked_file = _file.GetReference();
	auto log_scope = locked_file->GetFilenameString();
	auto input = locked_file->GetInputStream();

	LOG_DEBUG(log_scope)
		<< "Initializing xref USED entry "
		<< std::to_string(_obj_number)
		<< " "
		<< std::to_string(_gen_number)
		<< " at offset "
		<< std::to_string(_offset);

	input->ExclusiveInputLock();

	auto rewind_pos = input->GetInputPosition();

	// We want to capture input by value, because it might be out of scope
	// In order to call non-const method we have to tag the lambda mutable
	auto cleanup_lambda = [input, rewind_pos]() mutable {
		input->SetInputPosition(rewind_pos);
		input->ExclusiveInputUnlock();
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
	auto log_scope = locked_file->GetFilenameString();
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

	LOG_DEBUG(log_scope)
		<< "Initializing xref COMPRESSED entry "
		<< std::to_string(_obj_number)
		<< " "
		<< std::to_string(_gen_number)
		<< " inside object stream "
		<< std::to_string(_object_stream_number);

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
			auto scope = locked_file->GetFilenameString();
			LOG_WARNING(scope) <<
				"Object stream " <<
				std::to_string(_object_stream_number) <<
				" contains unused xref entry " <<
				std::to_string(entry_object_number);

			continue;
		}

		auto stream_used_entry_xref = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(stream_entry_xref);
		stream_used_entry_xref->SetReference(entry_object);
		stream_used_entry_xref->SetInitialized();
	}

	if (!m_initialized) {
		auto scope = locked_file->GetFilenameString();
		LOG_WARNING(scope) <<
			"Compressed entry number " <<
			std::to_string(GetObjectNumber()) <<
			" " <<
			GetGenerationNumber()
			<< " was supposed to be found in object stream number "
			<< GetObjectStreamNumber()
			<< ", but was not found";

		LOG_WARNING(scope) << "Treating as NULL reference";
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
