#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/utils/output_pointer.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/streams/output_stream_interface.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

DictionaryObject::DictionaryObject() {
	m_access_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

DictionaryObject* DictionaryObject::Clone(void) const {
	DictionaryObjectPtr result(pdf_new DictionaryObject(), false);

	ACCESS_LOCK_GUARD(m_access_lock);

	for (auto& item : _list) {
		auto name = ObjectUtils::Clone<NameObjectPtr>(item.first);
		auto value = ObjectUtils::Clone<ContainableObjectPtr>(item.second);
		result->Insert(name, value);
	}

	CloneBaseProperties(result);
	return result.detach();
}

void DictionaryObject::SetFile(WeakReference<File> file) {

	ACCESS_LOCK_GUARD(m_access_lock);

	Object::SetFile(file);
	for (auto it = _list.begin(); it != _list.end(); ++it) {
		auto item = it->second;
		item->SetFile(file);
	}
}

void DictionaryObject::SetInitialized(bool initialized) {
	ACCESS_LOCK_GUARD(m_access_lock);

	IModifyObservable::SetInitialized(initialized);
	for (auto it = _list.begin(); it != _list.end(); ++it) {
		auto item = it->second;
		item->SetInitialized(initialized);
	}
}

std::string DictionaryObject::ToString(void) const {
	ACCESS_LOCK_GUARD(m_access_lock);

	std::stringstream ss;
	ss << "<<" << std::endl;
	for (auto item : _list) {
		ss << item.first->ToString() << " " << item.second->ToString() << std::endl;
	}

	ss << ">>";
	return ss.str();
}

void DictionaryObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	ACCESS_LOCK_GUARD(m_access_lock);

	output << "<<";
	bool first = true;
	for (auto item : _list) {
		output << (first ? "" : " ");
		item.first->ToPdfStream(output);
		output << " ";
		item.second->ToPdfStream(output);
		first = false;
	}

	output << ">>";
}

void DictionaryObject::ToPdfStreamUpdateOffset(IOutputStreamPtr output) {
	ACCESS_LOCK_GUARD(m_access_lock);

	UpdateOffset(output);

	// If the object contains attribute, that controls it's serialization
	if (HasOverrideAttribute()) {
		auto override_attribute = GetOverrideAttribute();
		output->Write(override_attribute);
		return;
	}

	output << "<<";
	bool first = true;
	for (std::pair<NameObjectPtr, ContainableObjectPtr> item : _list) {
		output << (first ? "" : " ");
		item.first->ToPdfStreamUpdateOffset(output);
		output << " ";
		item.second->ToPdfStreamUpdateOffset(output);
		first = false;
	}

	output << ">>";
}

ContainableObjectPtr DictionaryObject::Find(const NameObject& name) const {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return Find(temp);
}

ContainableObjectPtr DictionaryObject::Find(const NameObjectPtr name) const {

	ACCESS_LOCK_GUARD(m_access_lock);

	auto result = _list.find(name);
	if (result == _list.end()) {
		throw GeneralException("Item with name " + name->ToString() + " was not found in dictionary");
	}

	return result->second;
}

bool DictionaryObject::TryFind(const NameObject& name, OutputContainableObjectPtr& result) const {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return TryFind(temp, result);
}

bool DictionaryObject::TryFind(const NameObjectPtr name, OutputContainableObjectPtr& result) const {

	ACCESS_LOCK_GUARD(m_access_lock);

	auto item = _list.find(name);
	if (item == _list.end()) {
		return false;
	}

	result = item->second;
	return true;
}

bool DictionaryObject::Remove(const NameObject& name) {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return Remove(temp);
}

bool DictionaryObject::Remove(const NameObjectPtr name) {

	ACCESS_LOCK_GUARD(m_access_lock);

	auto found = _list.find(name);
	if (found == _list.end()) {
		return false;
	}

	// The trick here is that assignment
	// creates a non-const copy.
	// Be careful to preserve
	// the object state
	auto found_key = found->first;
	auto found_value = found->second;

	found_key->ClearOwner();
	found_value->ClearOwner();
	found_key->Unsubscribe(this);
	found_value->Unsubscribe(this);
	_list.erase(found);
	OnChanged();

	return true;
}

void DictionaryObject::Insert(const NameObject& name, ContainableObjectPtr value, bool overwrite) {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	Insert(temp, value, overwrite);
}

void DictionaryObject::Insert(NameObjectPtr name, ContainableObjectPtr value, bool overwrite) {

	ACCESS_LOCK_GUARD(m_access_lock);

	if (!overwrite) {
		auto found = _list.find(name);
		if (found != _list.end()) {
			throw DuplicateKeyException("The key " + name->ToString() + " was already present in the dictionary");
		}
	}

	auto pair = std::make_pair(name, value);
	auto result = _list.insert(pair);

	// The pair::second element in the pair is set to true if a new element was inserted or false if an equivalent key already existed.
	if (!result.second) {

		auto log_writer = LOG_INFO_GLOBAL;

		auto weak_file = GetFile();
		if (!weak_file.IsEmpty() && weak_file.IsActive()) {
			auto file = weak_file.GetReference();
			auto log_scope = file->GetFilenameString();
			log_writer = LOG_INFO(log_scope);
		}

		log_writer->GetOutputStream() << "Overwriting dictionary entry for key: " << name->ToString();
	}

	name->SetOwner(Object::GetWeakReference());
	value->SetOwner(Object::GetWeakReference());

	name->Subscribe(this);
	value->Subscribe(this);

	OnChanged();
}

bool DictionaryObject::Contains(const NameObject& name) const {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return Contains(temp);
}

bool DictionaryObject::Contains(const NameObjectPtr name) const {

	ACCESS_LOCK_GUARD(m_access_lock);
	return (_list.find(name) != _list.end());
}

DictionaryObject::~DictionaryObject() {
	ACCESS_LOCK_GUARD(m_access_lock);

	for (auto item : _list) {

		// The trick here is that assignment
		// creates a non-const copy.
		// Be careful to preserve
		// the object state
		auto item_key = item.first;
		auto item_value = item.second;

		item_key->Unsubscribe(this);
		item_value->Unsubscribe(this);
	}
}

void DictionaryObject::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

void DictionaryObject::OnChanged() {
	Object::OnChanged();

	m_hash_cache = 0;
}

size_t DictionaryObject::Hash() const {
	ACCESS_LOCK_GUARD(m_access_lock);

	if (m_hash_cache != 0) {
		return m_hash_cache;
	}

	size_t result = 0;
	for (auto item : _list) {
		result ^= item.first->Hash();
		result ^= item.second->Hash();
	}

	m_hash_cache = result;
	return m_hash_cache;
}

bool DictionaryObject::Equals(ObjectPtr other) const {
	ACCESS_LOCK_GUARD(m_access_lock);

	if (!ObjectUtils::IsType<DictionaryObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<DictionaryObjectPtr>(other);

	auto first_size = size();
	auto second_size = other_obj->size();
	if (first_size != second_size) {
		return false;
	}

	auto this_iterator = _list.begin();
	auto other_iterator = other_obj.begin();

	for (; this_iterator != _list.end(); this_iterator++, other_iterator++) {
		auto this_pair = *this_iterator;
		auto other_pair = *other_iterator;

		auto this_name = this_pair.first;
		auto other_name = other_pair.first;

		if (!this_name->Equals(*other_name)) {
			return false;
		}

		auto this_value = this_pair.second;
		auto other_value = other_pair.second;

		if (!this_value->Equals(other_value)) {
			return false;
		}
	}

	return true;
}

void DictionaryObject::Merge(const DictionaryObject& other) {
	ACCESS_LOCK_GUARD(m_access_lock);

	// Simple insert overriding conflicting entries
	for (auto item : other) {
		_list.insert(item);
	}
}

void DictionaryObject::Clear() {
	ACCESS_LOCK_GUARD(m_access_lock);
	_list.clear();
}

DictionaryObject::size_type DictionaryObject::GetSize() const {
	ACCESS_LOCK_GUARD(m_access_lock);
	return _list.size();
}

} // syntax
} // vanillapdf
