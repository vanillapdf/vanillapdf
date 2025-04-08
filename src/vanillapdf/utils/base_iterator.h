#ifndef _BASE_ITERATOR_H
#define _BASE_ITERATOR_H

#include "utils/unknown_interface.h"

namespace vanillapdf {

template <typename T>
class BaseIterator : public IUnknown {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;

public:
    BaseIterator() = default;
    explicit BaseIterator(T current, T invalid);

    typename std::iterator_traits<T>::value_type Value() const;
    typename std::iterator_traits<T>::value_type operator*();

    bool operator==(const BaseIterator& other) const;
    bool operator!=(const BaseIterator& other) const;
    bool IsValid() const;

    T RawIterator() const;

protected:
    T m_current;
    T m_invalid;
};

template <typename T>
BaseIterator<T>::BaseIterator(T current, T invalid) : m_current(current), m_invalid(invalid) {
}

template <typename T>
typename std::iterator_traits<T>::value_type BaseIterator<T>::Value() const {
    return *m_current;
}

template <typename T>
bool BaseIterator<T>::operator==(const BaseIterator& other) const {
    return (m_current == other.m_current);
}

template <typename T>
bool BaseIterator<T>::operator!=(const BaseIterator& other) const {
    return (m_current != other.m_current);
}

template <typename T>
bool BaseIterator<T>::IsValid() const {
    return (m_current != m_invalid);
}

template <typename T>
typename std::iterator_traits<T>::value_type BaseIterator<T>::operator*() {
    return *m_current;
}

template <typename T>
T BaseIterator<T>::RawIterator() const {
    return m_current;
}

} // vanillapdf

#endif /* _BASE_ITERATOR_H */
