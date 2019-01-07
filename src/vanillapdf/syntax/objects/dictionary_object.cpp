#include "precompiled.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/utils/output_pointer.h"

#include "utils/streams/output_stream_interface.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

DictionaryObject* DictionaryObject::Clone(void) const {
	DictionaryObjectPtr result(pdf_new DictionaryObject(), false);

	for (auto& item : _list) {
		auto name = ObjectUtils::Clone<NameObjectPtr>(item.first);
		auto value = ObjectUtils::Clone<ContainableObjectPtr>(item.second);
		result->Insert(name, value);
	}

	CloneBaseProperties(result);
	return result.detach();
}

void DictionaryObject::SetFile(WeakReference<File> file) {
	Object::SetFile(file);

	for (auto it = _list.begin(); it != _list.end(); ++it) {
		auto item = it->second;
		item->SetFile(file);
	}
}

void DictionaryObject::SetInitialized(bool initialized) {
	IModifyObservable::SetInitialized(initialized);

	for (auto it = _list.begin(); it != _list.end(); ++it) {
		auto item = it->second;
		item->SetInitialized(initialized);
	}
}

std::string DictionaryObject::ToString(void) const {
	std::stringstream ss;
	ss << "<<" << std::endl;
	for (auto item : _list) {
		ss << item.first->ToString() << " " << item.second->ToString() << std::endl;
	}

	ss << ">>";
	return ss.str();
}

void DictionaryObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
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

bool DictionaryObject::Insert(const NameObject& name, ContainableObjectPtr value) {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return Insert(temp, value);
}

bool DictionaryObject::Insert(NameObjectPtr name, ContainableObjectPtr value) {
	auto pair = std::make_pair(name, value);
	auto result = _list.insert(pair);
	if (result.second == false) {
		return false;
	}

	name->SetOwner(Object::GetWeakReference());
	value->SetOwner(Object::GetWeakReference());

	name->Subscribe(this);
	value->Subscribe(this);

	//assert(result.second && "Key was already in the dictionary");
	OnChanged();

	return true;
}

bool DictionaryObject::Contains(const NameObject& name) const {
	NameObjectPtr temp = make_deferred<NameObject>(name);
	return Contains(temp);
}

bool DictionaryObject::Contains(const NameObjectPtr name) const {
	return (_list.find(name) != _list.end());
}

DictionaryObject::~DictionaryObject() {
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

void DictionaryObject::OnChanged() const {
	Object::OnChanged();

	m_hash_cache = 0;
}

size_t DictionaryObject::Hash() const {
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

	// Simple insert overriding conflicting entries
	for (auto item : other) {
		_list.insert(item);
	}
}

} // syntax
} // vanillapdf
