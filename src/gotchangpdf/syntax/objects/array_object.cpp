#include "precompiled.h"
#include "syntax/objects/array_object.h"

namespace gotchangpdf {
namespace syntax {

MixedArrayObject::MixedArrayObject(const list_type& list) : _list(list) {
	for (auto item : _list) {
		item->SetOwner(GetWeakReference<Object>());
		item->Subscribe(this);
	}
}

MixedArrayObject::MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list) : _list(list) {
	for (auto item : _list) {
		item->SetOwner(GetWeakReference<Object>());
		item->Subscribe(this);
	}
}

MixedArrayObject::MixedArrayObject(const ContainableObject& other, list_type& list)
	: ContainableObject(other), _list(list) {
	for (auto item : _list) {
		item->SetOwner(GetWeakReference<Object>());
		item->Subscribe(this);
	}
}

void MixedArrayObject::SetFile(std::weak_ptr<File> file) noexcept {
	Object::SetFile(file);

	auto size = _list.size();
	for (decltype(size) i = 0; i < size; ++i) {
		auto item = _list[i];
		item->SetFile(file);
	}
}

void MixedArrayObject::SetInitialized(bool initialized) noexcept {
	IModifyObservable::SetInitialized(initialized);

	auto size = _list.size();
	for (decltype(size) i = 0; i < size; ++i) {
		auto item = _list[i];
		item->SetInitialized(initialized);
	}
}

void MixedArrayObject::ObserveeChanged(IModifyObservable*) { OnChanged(); }

MixedArrayObject* MixedArrayObject::Clone(void) const {
	std::unique_ptr<MixedArrayObject> result(new MixedArrayObject());
	for (auto item : _list) {
		auto cloned = ObjectUtils::Clone<ContainableObjectPtr>(item);
		result->Append(cloned);
	}

	return result.release();
}

void MixedArrayObject::Append(const ContainableObjectPtr& value) {
	_list.push_back(value);
	value->SetOwner(GetWeakReference<Object>());
	value->Subscribe(this);
	OnChanged();
}

void MixedArrayObject::Insert(const ContainableObjectPtr& value, size_t at) {
	_list.insert(_list.begin() + at, value);
	value->SetOwner(GetWeakReference<Object>());
	value->Subscribe(this);
	OnChanged();
}

void MixedArrayObject::Remove(size_t at) {
	auto item = _list.begin() + at;
	(*item)->ClearOwner();
	(*item)->Unsubscribe(this);
	_list.erase(item);
	OnChanged();
}

// stl compatibility
void MixedArrayObject::push_back(const value_type& value) {
	_list.push_back(value);
	value->Subscribe(this);
	OnChanged();
}

std::string MixedArrayObject::ToString(void) const {
	std::stringstream ss;
	ss << "[";
	bool first = true;
	for (auto item : _list) {
		ss << (first ? "" : " ") << item->ToString();
		first = false;
	}

	ss << "]";
	return ss.str();
}

std::string MixedArrayObject::ToPdf(void) const {
	std::stringstream ss;
	ss << "[";
	bool first = true;
	for (auto item : _list) {
		ss << (first ? "" : " ") << item->ToPdf();
		first = false;
	}

	ss << "]";
	return ss.str();
}

MixedArrayObject::~MixedArrayObject() {
	for (auto item : _list) {
		item->Unsubscribe(this);
	}
}

} // syntax
} // gotchangpdf
