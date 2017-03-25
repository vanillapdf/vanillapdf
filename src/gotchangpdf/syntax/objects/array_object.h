#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/mixed_array_object.h"

#include <algorithm>
#include <functional>

namespace gotchangpdf {
namespace syntax {

template <typename T>
class DeferredArrayObject : public DeferredContainer<ArrayObject<T>> {
public:
	using DeferredContainer<ArrayObject<T>>::DeferredContainer;

	operator MixedArrayObjectPtr() {
		return DeferredContainer<ArrayObject<T>>::get()->Data();
	}

	operator ContainableObjectPtr() {
		return DeferredContainer<ArrayObject<T>>::get()->Data();
	}
};

template <typename T>
class ArrayObjectIterator : public virtual IUnknown, public std::iterator<std::input_iterator_tag, T> {
public:
	ArrayObjectIterator(std::function<T(const ContainableObjectPtr& obj)> conversion);
	ArrayObjectIterator(typename MixedArrayObject::iterator it, const std::function<T(const ContainableObjectPtr& obj)>& conversion);

	const ArrayObjectIterator& operator++();
	const ArrayObjectIterator operator++(int);

	bool operator==(const ArrayObjectIterator& other) const;
	bool operator!=(const ArrayObjectIterator& other) const;

	T operator*() const;

private:
	typename MixedArrayObject::iterator _it;
	std::function<T(const ContainableObjectPtr& obj)> _conversion;
};

// This class shall act as a front-end above mixed array
template <typename T>
class ArrayObject : public virtual IUnknown, public IWeakReferenceable<ArrayObject<T>> {
public:
	static_assert(instantiation_of<Deferred, T>::value ||
		std::is_base_of<Object, typename T::deferred_ptr_type>::value,
		"Array object requires template parameter to be either Deferred instance or derived from Object");

public:
	typedef MixedArrayObjectPtr list_type;
	typedef typename list_type::value_type value_type;
	typedef ArrayObjectIterator<T> iterator;
	typedef ArrayObjectIterator<T> const_iterator;
	typedef typename list_type::size_type size_type;
	typedef typename list_type::reference reference;
	typedef typename list_type::const_reference const_reference;

	template <typename U>
	friend class ArrayObject;

public:
	ArrayObject();
	explicit ArrayObject(MixedArrayObjectPtr other);
	explicit ArrayObject(const std::initializer_list<T>& list);
	ArrayObject(const list_type& list, std::function<T(const ContainableObjectPtr& obj)> conversion);
	ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion);
	ArrayObject(const MixedArrayObject& other, std::function<T(const ContainableObjectPtr& obj)> conversion);

	template <typename U>
	ArrayObject(ArrayObject<U> other, std::function<T(const U& obj)> new_conversion);

	MixedArrayObjectPtr Data(void) const;
	size_t Size(void) const;
	const T operator[](size_t i) const;
	T operator[](size_t i);
	const T At(size_t at) const;
	T At(size_t at);

	void Append(const T& value);
	void Insert(const T& value, size_t at);
	bool Remove(size_t at);

	std::string ToString(void) const;

	template <typename U>
	ArrayObjectPtr<U> Convert(std::function<U(const T& obj)> f) const;

	// stl compatibility
	ArrayObjectIterator<T> begin();
	const ArrayObjectIterator<T> begin() const;
	ArrayObjectIterator<T> end();
	const ArrayObjectIterator<T> end() const;

private:
	MixedArrayObjectPtr _list;
	std::function<T(const ContainableObjectPtr& obj)> _conversion;

	static T DefaultConversion(const ContainableObjectPtr& obj);
};

#pragma region ArrayObjectIterator

template <typename T>
ArrayObjectIterator<T>::ArrayObjectIterator(
	std::function<T(const ContainableObjectPtr& obj)> conversion)
	: _conversion(conversion) {
}

template <typename T>
ArrayObjectIterator<T>::ArrayObjectIterator(
	typename MixedArrayObject::iterator it,
	const std::function<T(const ContainableObjectPtr& obj)>& conversion)
	: _it(it), _conversion(conversion) {
}

template <typename T>
const ArrayObjectIterator<T>& ArrayObjectIterator<T>::operator++() {
	++_it;
	return *this;
}

template <typename T>
const ArrayObjectIterator<T> ArrayObjectIterator<T>::operator++(int) {
	ArrayObjectIterator<T> temp(_it);
	++_it;
	return temp;
}

template <typename T>
bool ArrayObjectIterator<T>::operator==(const ArrayObjectIterator<T>& other) const {
	return _it == other._it;
}

template <typename T>
bool ArrayObjectIterator<T>::operator!=(const ArrayObjectIterator<T>& other) const {
	return _it != other._it;
}

template <typename T>
T ArrayObjectIterator<T>::operator*() const {
	return _conversion(_it.operator*());
}

#pragma endregion

#pragma region ArrayObject

template <typename T>
ArrayObject<T>::ArrayObject() : _conversion(&DefaultConversion) {
}

template <typename T>
ArrayObject<T>::ArrayObject(MixedArrayObjectPtr other)
	: _list(other), _conversion(&DefaultConversion) {
}

template <typename T>
ArrayObject<T>::ArrayObject(const std::initializer_list<T>& list) : _conversion(&DefaultConversion) {
	for (auto item : list) {
		_list->push_back(item);
	}
}

template <typename T>
ArrayObject<T>::ArrayObject(const list_type& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
	: _conversion(conversion) {
	for (auto item : list) {
		_list->push_back(item);
	}
}

template <typename T>
ArrayObject<T>::ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
	: _conversion(conversion) {
	for (auto item : list) {
		_list->push_back(item);
	}
}

template <typename T>
ArrayObject<T>::ArrayObject(const MixedArrayObject& other, std::function<T(const ContainableObjectPtr& obj)> conversion)
	: _list(other), _conversion(conversion) {
	for (auto item : other) {
		_conversion(item);
	}
}

template <typename T>
template <typename U>
ArrayObject<T>::ArrayObject(ArrayObject<U> other, std::function<T(const U& obj)> new_conversion) {
	auto other_conversion = other._conversion;
	_conversion = [other_conversion, new_conversion](const ContainableObjectPtr& obj) { return new_conversion(other_conversion(obj)); };
	for (auto item : other) {
		_list->push_back(item);
	}
}

template <typename T>
MixedArrayObjectPtr ArrayObject<T>::Data(void) const {
	return _list;
}

template <typename T>
size_t ArrayObject<T>::Size(void) const {
	return _list->Size();
}

template <typename T>
const T ArrayObject<T>::operator[](size_t i) const {
	return _conversion((*_list)[i]);
}

template <typename T>
T ArrayObject<T>::operator[](size_t i) {
	return _conversion((*_list)[i]);
}

template <typename T>
const T ArrayObject<T>::At(size_t at) const {
	return _conversion(_list->At(at));
}

template <typename T>
T ArrayObject<T>::At(size_t at) {
	return _conversion(_list->At(at));
}

template <typename T>
void ArrayObject<T>::Append(const T& value) {
	_list->Append(value);
}

template <typename T>
void ArrayObject<T>::Insert(const T& value, size_t at) {
	_list->Insert(value, at);
}

template <typename T>
bool ArrayObject<T>::Remove(size_t at) {
	return _list->Remove(at);
}

template <typename T>
std::string ArrayObject<T>::ToString(void) const {
	return _list->ToString();
}

template <typename T>
template <typename U>
ArrayObjectPtr<U> ArrayObject<T>::Convert(std::function<U(const T& obj)> f) const {
	return make_deferred<ArrayObject<U>>(*this, f);
}

template <typename T>
ArrayObjectIterator<T> ArrayObject<T>::begin() {
	return ArrayObjectIterator<T>(_list->begin(), _conversion);
}

template <typename T>
const ArrayObjectIterator<T> ArrayObject<T>::begin() const {
	return ArrayObjectIterator<T>(_list->begin(), _conversion);
}

template <typename T>
ArrayObjectIterator<T> ArrayObject<T>::end() {
	return ArrayObjectIterator<T>(_list->end(), _conversion);
}

template <typename T>
const ArrayObjectIterator<T> ArrayObject<T>::end() const {
	return ArrayObjectIterator<T>(_list->end(), _conversion);
}

template <typename T>
T ArrayObject<T>::DefaultConversion(const ContainableObjectPtr& obj) {
	return ObjectUtils::ConvertTo<T>(obj);
}

#pragma endregion

} // syntax
} // gotchangpdf

#endif /* _ARRAY_OBJECT_H */
