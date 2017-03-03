#ifndef _BASE_ITERATOR_H
#define _BASE_ITERATOR_H

#include "unknown_interface.h"

namespace gotchangpdf {

template <typename T>
class BaseIterator : public virtual IUnknown, public std::iterator<std::input_iterator_tag, T> {
public:
	BaseIterator() = default;
	BaseIterator(T it);

	typename std::iterator_traits<T>::value_type Value() const;
	typename std::iterator_traits<T>::value_type operator*();

	bool operator==(const BaseIterator& other) const;
	bool operator!=(const BaseIterator& other) const;

	T RawIterator() const;

protected:
	T m_it;
};

template <typename T>
BaseIterator<T>::BaseIterator(T it) : m_it(it) {
}

template <typename T>
typename std::iterator_traits<T>::value_type BaseIterator<T>::Value() const {
	return *m_it;
}

template <typename T>
bool BaseIterator<T>::operator==(const BaseIterator& other) const {
	return (m_it == other.m_it);
}

template <typename T>
bool BaseIterator<T>::operator!=(const BaseIterator& other) const {
	return (m_it != other.m_it);
}

template <typename T>
typename std::iterator_traits<T>::value_type BaseIterator<T>::operator*() {
	return *m_it;
}

template <typename T>
T BaseIterator<T>::RawIterator() const {
	return m_it;
}

} // gotchangpdf

#endif /* _BASE_ITERATOR_H */
