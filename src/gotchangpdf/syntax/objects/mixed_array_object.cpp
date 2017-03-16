#include "precompiled.h"

#include "syntax/objects/mixed_array_object.h"

namespace gotchangpdf {
namespace syntax {

MixedArrayObject::MixedArrayObject(const list_type& list) : _list(list) {
	for (auto item : _list) {
		item->SetOwner(Object::GetWeakReference());
		item->Subscribe(this);
	}
}

MixedArrayObject::MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list) : _list(list) {
	for (auto item : _list) {
		item->SetOwner(Object::GetWeakReference());
		item->Subscribe(this);
	}
}

MixedArrayObject::MixedArrayObject(const ContainableObject& other, list_type& list)
	: ContainableObject(other), _list(list) {
	for (auto item : _list) {
		item->SetOwner(Object::GetWeakReference());
		item->Subscribe(this);
	}
}

void MixedArrayObject::SetFile(WeakReference<File> file) noexcept {
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
	MixedArrayObjectPtr result(pdf_new MixedArrayObject(), false);

	result->SetFile(m_file);

	for (auto item : _list) {
		auto cloned = ObjectUtils::Clone<ContainableObjectPtr>(item);
		result->Append(cloned);
	}

	return result.detach();
}

void MixedArrayObject::Append(ContainableObjectPtr value) {
	_list.push_back(value);
	value->SetOwner(Object::GetWeakReference());
	value->Subscribe(this);
	OnChanged();
}

void MixedArrayObject::Insert(ContainableObjectPtr value, size_t at) {
	_list.insert(_list.begin() + at, value);
	value->SetOwner(Object::GetWeakReference());
	value->Subscribe(this);
	OnChanged();
}

bool MixedArrayObject::Remove(size_t at) {
	auto item = _list.begin() + at;
	if (item == _list.end()) {
		return false;
	}

	(*item)->ClearOwner();
	(*item)->Unsubscribe(this);
	_list.erase(item);
	OnChanged();

	return true;
}

// stl compatibility
void MixedArrayObject::push_back(ContainableObjectPtr value) {
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

size_t MixedArrayObject::Hash() const {
	size_t result = 0;
	for (auto item : _list) {
		result ^= item->Hash();
	}

	return result;
}

bool MixedArrayObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<MixedArrayObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(other);

	auto first_size = Size();
	auto second_size = other_obj->Size();
	if (first_size != second_size) {
		return false;
	}

	for (unsigned int i = 0; i < first_size; ++i) {
		auto first_obj = At(i);
		auto second_obj = other_obj->At(i);
		if (!first_obj->Equals(second_obj)) {
			return false;
		}
	}

	return true;
}

} // syntax
} // gotchangpdf
