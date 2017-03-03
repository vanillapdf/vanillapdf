#include "precompiled.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/utils/output_pointer.h"

namespace gotchangpdf {
namespace syntax {

DictionaryObject* DictionaryObject::Clone(void) const {
	DictionaryObjectPtr result;

	result->SetFile(m_file);

	for (auto item : _list) {
		auto name = ObjectUtils::Clone<NameObjectPtr>(item.first);
		auto value = ObjectUtils::Clone<ContainableObjectPtr>(item.second);
		result->Insert(name, value);
	}

	return result.detach();
}

void DictionaryObject::SetFile(WeakReference<File> file) noexcept {
	Object::SetFile(file);

	for (auto it = _list.begin(); it != _list.end(); ++it) {
		auto item = it->second;
		item->SetFile(file);
	}
}

void DictionaryObject::SetInitialized(bool initialized) noexcept {
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

std::string DictionaryObject::ToPdf(void) const {
	std::stringstream ss;
	ss << "<<";
	bool first = true;
	for (auto item : _list) {
		ss << (first ? "" : " ") << item.first->ToPdf() << " " << item.second->ToPdf();
		first = false;
	}
	ss << ">>";
	return ss.str();
}

ContainableObjectPtr DictionaryObject::Find(const NameObject& name) const {
	auto result = _list.find(name);
	if (result == _list.end()) {
		throw GeneralException("Item with name " + name.ToString() + " was not found in dictionary");
	}

	return result->second;
}

bool DictionaryObject::TryFind(const NameObject& name, OutputContainableObjectPtr& result) const {
	auto item = _list.find(name);
	if (item == _list.end()) {
		return false;
	}

	result = item->second;
	return true;
}

bool DictionaryObject::Remove(const NameObject& name) {
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

void DictionaryObject::Insert(NameObjectPtr name, ContainableObjectPtr value) {
	std::pair<const NameObjectPtr, ContainableObjectPtr> pair(name, value);
	auto result = _list.insert(pair);
	name->SetOwner(Object::GetWeakReference());
	value->SetOwner(Object::GetWeakReference());

	name->Subscribe(this);
	value->Subscribe(this);

	//assert(result.second && "Key was already in the dictionary");
	OnChanged();
}

bool DictionaryObject::Contains(const NameObject& name) const {
	return (_list.find(name) != _list.end());
}

std::vector<ContainableObjectPtr> DictionaryObject::Values() const {
	std::vector<ContainableObjectPtr> result;
	std::for_each(_list.begin(), _list.end(),
		[&result](const std::pair<NameObjectPtr, ContainableObjectPtr>& item) {
		result.push_back(item.second);
	});

	return result;
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

bool DictionaryObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<DictionaryObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<DictionaryObjectPtr>(other);

	auto first_vals = Values();
	auto second_vals = other_obj->Values();
	auto first_vals_size = first_vals.size();
	auto second_vals_size = second_vals.size();
	if (first_vals_size != second_vals_size) {
		return false;
	}

	for (unsigned int i = 0; i < second_vals_size; ++i) {
		auto first_obj = first_vals[i];
		auto second_obj = second_vals[i];
		if (!first_obj->Equals(second_obj)) {
			return false;
		}
	}

	return true;
}

} // syntax
} // gotchangpdf
