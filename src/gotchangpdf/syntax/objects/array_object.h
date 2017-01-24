#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/mixed_array_object.h"

#include <algorithm>
#include <functional>

namespace gotchangpdf {
namespace syntax {

template <typename T>
class DeferredArray : public Deferred<ArrayObject<T>> {
public:
	using Deferred<ArrayObject<T>>::Deferred;

	operator MixedArrayObjectPtr() {
		return Deferred<ArrayObject<T>>::get()->Data();
	}

	operator ContainableObjectPtr() {
		return Deferred<ArrayObject<T>>::get()->Data();
	}
};

// This class shall act as a front-end above mixed array
template <typename T>
class ArrayObject : public IUnknown {
public:
	static_assert(instantiation_of<Deferred, T>::value ||
		std::is_base_of<Object, typename T::deferred_ptr_type>::value,
		"Array object requires template parameter to be either Deferred instance or derived from Object");

public:
	typedef std::vector<T> list_type;
	typedef typename list_type::value_type value_type;
	typedef typename list_type::iterator iterator;
	typedef typename list_type::const_iterator const_iterator;
	typedef typename list_type::size_type size_type;
	typedef typename list_type::reference reference;
	typedef typename list_type::const_reference const_reference;

	template <typename U>
	friend class ArrayObject;

public:
	ArrayObject() : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); }) {}

	explicit ArrayObject(MixedArrayObjectPtr other)
		: _list(other), _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); }) {
	}

	explicit ArrayObject(const list_type& list) : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); }) {
		for (auto item : list) _list->push_back(item);
	}

	explicit ArrayObject(const std::initializer_list<T>& list) : _conversion([](const ContainableObjectPtr& obj) { return ObjectUtils::ConvertTo<T>(obj); }) {
		for (auto item : list) _list->push_back(item);
	}

	//explicit ArrayObject(std::function<T(const ContainableObjectPtr& obj)> conversion) : _conversion(conversion) {}
	explicit ArrayObject(const list_type& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
		: _conversion(conversion) {
		for (auto item : list) _list->push_back(item);
	}

	explicit ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
		: _conversion(conversion) {
		for (auto item : list) _list->push_back(item);
	}

	ArrayObject(const MixedArrayObject& other, std::function<T(const ContainableObjectPtr& obj)> conversion)
		: _list(other), _conversion(conversion) {
		for (auto item : other) _conversion(item);
	}

	template <typename U>
	ArrayObject(const ArrayObject<U>& other, std::function<T(const U& obj)> new_conversion) {
		auto other_conversion = other._conversion;
		_conversion = [other_conversion, new_conversion](const ContainableObjectPtr& obj) { return new_conversion(other_conversion(obj)); };
		for (auto item : other) _list->push_back(item);
	}

	MixedArrayObjectPtr Data(void) const { return _list; }
	size_t Size(void) const { return _list->Size(); }
	const T operator[](size_t i) const { return _conversion((*_list)[i]); }
	T operator[](size_t i) { return _conversion((*_list)[i]); }
	const T At(size_t at) const { return _conversion(_list->At(at)); }
	T At(size_t at) { return _conversion(_list->At(at)); }

	void Append(const ContainableObjectPtr& value) { _list->Append(value); }
	void Insert(const ContainableObjectPtr& value, size_t at) { _list->Insert(value, at); }
	void Remove(size_t at) { _list->Remove(at); }

	std::string ToString(void) const {
		return _list->ToString();
	}

	template <typename U>
	ArrayObjectPtr<U> Convert(std::function<U(const T& obj)> f) const {
		return ArrayObjectPtr<U>(*this, f);
	}

public:
	class Iterator : public IUnknown, public std::iterator<std::input_iterator_tag, T> {
	public:
		Iterator(std::function<T(const ContainableObjectPtr& obj)> conversion) : _conversion(conversion) {};
		Iterator(typename MixedArrayObject::iterator it, const std::function<T(const ContainableObjectPtr& obj)>& conversion)
			: _it(it), _conversion(conversion) {
		}

		const Iterator& operator++() {
			++_it;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(_it);
			++_it;
			return temp;
		}

		bool operator==(const Iterator& other) const {
			return _it == other._it;
		}

		bool operator!=(const Iterator& other) const {
			return _it != other._it;
		}

		T operator*() const { return _conversion(_it.operator*()); }

	private:
		typename MixedArrayObject::iterator _it;
		std::function<T(const ContainableObjectPtr& obj)> _conversion;
	};

	Iterator begin() { return Iterator(_list->begin(), _conversion); }
	Iterator begin() const { return Iterator(_list->begin(), _conversion); }
	Iterator end() { return Iterator(_list->end(), _conversion); }
	Iterator end() const { return Iterator(_list->end(), _conversion); }

private:
	MixedArrayObjectPtr _list;
	std::function<T(const ContainableObjectPtr& obj)> _conversion;
};

} // syntax
} // gotchangpdf

#endif /* _ARRAY_OBJECT_H */
