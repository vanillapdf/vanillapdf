#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/object_utils.h"

#include "syntax/objects/mixed_array_object.h"

#include <algorithm>
#include <functional>

namespace vanillapdf {
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

    const T operator[](size_type i) const {
        return DeferredContainer<ArrayObject<T>>::get()->operator[](i);
    }

    T operator[](size_type i) {
        return DeferredContainer<ArrayObject<T>>::get()->operator[](i);
    }
};

template <typename T>
class ArrayObjectIterator : public IUnknown {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;

public:
    ArrayObjectIterator(std::function<T(const ContainableObjectPtr& obj)> conversion);
    ArrayObjectIterator(typename MixedArrayObject::const_iterator it, const std::function<T(const ContainableObjectPtr& obj)>& conversion);

    const ArrayObjectIterator& operator++();
    const ArrayObjectIterator operator++(int);

    bool operator==(const ArrayObjectIterator& other) const;
    bool operator!=(const ArrayObjectIterator& other) const;

    T operator*() const;

private:
    typename MixedArrayObject::const_iterator _it;
    std::function<T(const ContainableObjectPtr& obj)> _conversion;
};

// This class shall act as a front-end above mixed array
template <typename T>
class ArrayObject : public IUnknown {
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
    typedef typename list_type::difference_type difference_type;

    template <typename U>
    friend class ArrayObject;

public:
    ArrayObject();
    explicit ArrayObject(MixedArrayObjectPtr other);
    explicit ArrayObject(const std::initializer_list<T>& list);
    ArrayObject(const list_type& list, std::function<T(const ContainableObjectPtr& obj)> conversion);
    ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion);
    ArrayObject(MixedArrayObjectPtr other, std::function<T(const ContainableObjectPtr& obj)> conversion);

    template <typename U>
    ArrayObject(const ArrayObject<U>& other, std::function<T(const U& obj)> new_conversion);

    MixedArrayObjectPtr Data(void) const;
    size_type GetSize(void) const;
    const T operator[](size_type i) const;
    T operator[](size_type i);
    const T GetValue(size_type at) const;
    T GetValue(size_type at);
    void SetValue(size_type at, T value);

    void AddAttribute(BaseAttributePtr attribute);
    bool RemoveAttribute(BaseAttribute::Type type);
    bool ContainsAttribute(BaseAttribute::Type type) const;
    BaseAttributePtr GetAttribute(BaseAttribute::Type type);

    void Append(const T& value);
    void Insert(size_type at, const T& value);
    bool Remove(size_type at);
    void Clear();

    std::string ToString(void) const;
    std::string ToPdf(void) const;

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

    template <typename U = ObjectUtils>
    static T DefaultConversionTemplate(const ContainableObjectPtr& obj) {
        return U::template ConvertTo<T>(obj);
    }

    static T DefaultConversion(const ContainableObjectPtr& obj) {
        return DefaultConversionTemplate(obj);
    }
};

#pragma region ArrayObjectIterator

template <typename T>
ArrayObjectIterator<T>::ArrayObjectIterator(
    std::function<T(const ContainableObjectPtr& obj)> conversion)
    : _conversion(conversion) {
}

template <typename T>
ArrayObjectIterator<T>::ArrayObjectIterator(
    typename MixedArrayObject::const_iterator it,
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
    : _conversion(conversion), _list(list) {
}

template <typename T>
ArrayObject<T>::ArrayObject(const std::initializer_list<T>& list, std::function<T(const ContainableObjectPtr& obj)> conversion)
    : _conversion(conversion) {
    for (auto item : list) {
        _list->push_back(item);
    }
}

template <typename T>
ArrayObject<T>::ArrayObject(MixedArrayObjectPtr other, std::function<T(const ContainableObjectPtr& obj)> conversion)
    : _list(other), _conversion(conversion) {
}

template <typename T>
template <typename U>
ArrayObject<T>::ArrayObject(const ArrayObject<U>& other, std::function<T(const U& obj)> new_conversion) {
    auto other_conversion = other._conversion;

    _list = other._list;
    _conversion = [other_conversion, new_conversion](const ContainableObjectPtr& obj) {
        return new_conversion(other_conversion(obj));
    };
}

template <typename T>
void ArrayObject<T>::AddAttribute(BaseAttributePtr attribute) {
    _list->AddAttribute(attribute);
}

template <typename T>
bool ArrayObject<T>::RemoveAttribute(BaseAttribute::Type type) {
    return _list->RemoveAttribute(type);
}

template <typename T>
bool ArrayObject<T>::ContainsAttribute(BaseAttribute::Type type) const {
    return _list->ContainsAttribute(type);
}

template <typename T>
BaseAttributePtr ArrayObject<T>::GetAttribute(BaseAttribute::Type type) {
    return _list->GetAttribute(type);
}

template <typename T>
MixedArrayObjectPtr ArrayObject<T>::Data(void) const {
    return _list;
}

template <typename T>
typename ArrayObject<T>::size_type ArrayObject<T>::GetSize(void) const {
    return _list->GetSize();
}

template <typename T>
const T ArrayObject<T>::operator[](size_type i) const {
    return _conversion((*_list)[i]);
}

template <typename T>
T ArrayObject<T>::operator[](size_type i) {
    return _conversion((*_list)[i]);
}

template <typename T>
const T ArrayObject<T>::GetValue(size_type at) const {
    return _conversion(_list->GetValue(at));
}

template <typename T>
T ArrayObject<T>::GetValue(size_type at) {
    return _conversion(_list->GetValue(at));
}

template <typename T>
void ArrayObject<T>::SetValue(size_type at, T value) {
    return _list->SetValue(at, value);
}

template <typename T>
void ArrayObject<T>::Append(const T& value) {
    _list->Append(value);
}

template <typename T>
void ArrayObject<T>::Insert(size_type at, const T& value) {
    _list->Insert(at, value);
}

template <typename T>
bool ArrayObject<T>::Remove(size_type at) {
    return _list->Remove(at);
}

template <typename T>
void ArrayObject<T>::Clear() {
    return _list->Clear();
}

template <typename T>
std::string ArrayObject<T>::ToString(void) const {
    return _list->ToString();
}

template <typename T>
std::string ArrayObject<T>::ToPdf(void) const {
    return _list->ToPdf();
}

template <typename T>
template <typename U>
ArrayObjectPtr<U> ArrayObject<T>::Convert(std::function<U(const T& obj)> f) const {
    return ArrayObjectPtr<U>(pdf_new ArrayObject<U>(*this, f));
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

#pragma endregion

} // syntax
} // vanillapdf

#endif /* _ARRAY_OBJECT_H */
