#ifndef _DEFERRED_H
#define _DEFERRED_H

#include "utils_fwd.h"
#include "unknown_interface.h"

#include <type_traits>
#include <initializer_list>
#include <utility>
#include <functional>
#include <cassert>

namespace gotchangpdf {

/*!
* \class DeferredWrapperBase
* \brief DeferredWrapperBase construction container for AST nodes.
*
* This class is used to speed up the construction of the AST. The construction of the node is only done when an access to the data is issued.
* This code has been taken from the Eddi Compiler project (https://github.com/wichtounet/eddic/) and has been adapted a little.
*/
template <typename T>
class DeferredWrapperBase {
public:
	typedef T deferred_ptr_type;

public:
	template <
		// Lets have a talk about SFINAE
		// SFINAE only works for deduced template arguments.
		// Because class template type T does not participate
		// in deduction and is specified by user.
		// Therefore I create a new unspecified type U
		// with default value of type T.
		// New type U is now "deduced" and can perform SFINAE

		// NOTE: I believe you cannot specify template arguments for
		// constructors. If you can, make sure user did not pass any
		// arguments overriding our defaulted type U.
		// Example: template<typename... Dummy, typename U = T>
		typename U = T,
		typename = typename std::enable_if<std::is_default_constructible<U>::value>::type
	>
	DeferredWrapperBase() noexcept : m_ptr(nullptr) {}

	DeferredWrapperBase(T* value, bool add_ref) noexcept : m_ptr(value) {
		if (m_ptr && add_ref) {
			m_ptr->AddRef();
		}
	}

	DeferredWrapperBase(T* value) noexcept : DeferredWrapperBase(value, true) {}
	DeferredWrapperBase(const DeferredWrapperBase& rhs) noexcept : DeferredWrapperBase(rhs.m_ptr, true) {}
	DeferredWrapperBase(DeferredWrapperBase&& rhs) noexcept : DeferredWrapperBase(rhs.m_ptr, false) { rhs.m_ptr = nullptr; }

	template <typename U, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
	DeferredWrapperBase(const DeferredWrapperBase<U>& rhs) : DeferredWrapperBase(rhs.get(), true) {}

	template <typename... Parameters, typename = typename std::enable_if<std::is_constructible<T, Parameters...>::value>::type>
	DeferredWrapperBase(const Parameters&... p) : DeferredWrapperBase(pdf_new T(p...), true) {}

	template <typename U, typename = typename std::enable_if<std::is_constructible<T, std::initializer_list<U>>::value>::type>
	DeferredWrapperBase(std::initializer_list<U> list) : DeferredWrapperBase(pdf_new T(list), true) {}

	operator T&() { return *get(); }
	operator T&() const { return *get(); }
	T& operator*() const { return *get(); }
	T* operator->() const { return get(); }

	template <
		// Thank you C++ gods, that I have to declare even more fake arguments
		typename U = T, // again SFINAE only on deduced arguments
		typename = typename std::enable_if<std::is_base_of<IWeakReferenceable<U>, T>::value>::type
	>
	WeakReference<U> GetWeakReference() { return get()->template GetWeakReference<U>(); }

	template <
		// Thank you C++ gods, that I have to declare even more fake arguments
		typename U = T, // again SFINAE only on deduced arguments
		typename = typename std::enable_if<std::is_base_of<IWeakReferenceable<U>, T>::value>::type
	>
	operator WeakReference<U>() { return GetWeakReference<U>(); }

	DeferredWrapperBase& operator=(const DeferredWrapperBase& rhs) {
		DeferredWrapperBase(rhs).swap(*this);
		return *this;
	}

	DeferredWrapperBase& operator=(DeferredWrapperBase&& rhs) {
		DeferredWrapperBase(std::move(rhs)).swap(*this);
		return *this;
	}

	bool empty(void) const {
		return m_ptr == nullptr;
	}

	T* get(void) const {
		return get_internal();
	}

	void reset() {
		DeferredWrapperBase().swap(*this);
	}

	void reset(T* rhs) {
		DeferredWrapperBase(rhs).swap(*this);
	}

	T* detach() {
		T* result = get();
		m_ptr = nullptr;
		return result;
	}

	T* AddRefGet(void) {
		T* result = get();

		// Require IUnknown to be present
		result->AddRef();

		return result;
	}

	void swap(DeferredWrapperBase& rhs) noexcept {
		T* tmp = m_ptr;
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = tmp;
	}

	virtual ~DeferredWrapperBase() {
		if (m_ptr) {
			m_ptr->Release();
		}
	}

protected:

	// C++11 14.7.3/16: In an explicit specialization declaration
	// for a member of a class template or a member template that
	// appears in namespace scope, the member template and some of
	// its enclosing class templates may remain unspecialized,
	// except that the declaration shall not explicitly specialize
	// a class member template if its enclosing class templates
	// are not explicitly specialized as well.

	// Visual studio supports member function template specialization
	// inside class, but unfortunately gcc does not.

	// I was not able to move get_internal template specialization
	// because I needed enclosing class remain unspecialized.
	// Therefore I moved template specialization to wrapping class
	// named DeferredWrapper which does specialize on
	// depending type using std::is_is_default_constructible<T>
	virtual T* get_internal() const { return m_ptr; }
	mutable T* m_ptr = nullptr;
};

template <
	typename T,
	bool constructible
>
class DeferredWrapper : public DeferredWrapperBase<T> {
public:
	using DeferredWrapperBase<T>::DeferredWrapperBase;

protected:
	virtual T* get_internal(void) const override {
		assert(DeferredWrapperBase<T>::m_ptr);
		if (!DeferredWrapperBase<T>::m_ptr) {
			return nullptr;
		}

		return DeferredWrapperBase<T>::m_ptr;
	}
};

template <typename T>
class DeferredWrapper<T, true> : public DeferredWrapperBase<T> {
public:
	using DeferredWrapperBase<T>::DeferredWrapperBase;

protected:
	virtual T* get_internal(void) const override {
		if (!DeferredWrapperBase<T>::m_ptr) {
			DeferredWrapperBase<T>::m_ptr = pdf_new T();
			DeferredWrapperBase<T>::m_ptr->AddRef();
		}

		return DeferredWrapperBase<T>::m_ptr;
	}
};

template <typename T>
class Deferred : public DeferredWrapper<T, std::is_default_constructible<T>::value> {
public:
	using DeferredWrapper<T, std::is_default_constructible<T>::value>::DeferredWrapper;
};

template <typename T>
class DeferredContainer : public Deferred<T> {
public:
	typedef typename T::value_type value_type;
	typedef typename T::iterator iterator;
	typedef typename T::const_iterator const_iterator;
	typedef typename T::size_type size_type;
	typedef typename T::reference reference;
	typedef typename T::const_reference const_reference;

public:
	using Deferred<T>::Deferred;

	// Support insertion as if this were itself a container
	void insert(const iterator& pos, const value_type& value) {
		Deferred<T>::get()->insert(pos, value);
	}

	// Retrieve a starting iterator as if this were itself a container
	iterator begin() {
		return Deferred<T>::get()->begin();
	}

	const_iterator begin() const {
		return Deferred<T>::get()->begin();
	}

	// Retrieve an ending iterator as if this were itself a container
	iterator end() {
		return Deferred<T>::get()->end();
	}

	const_iterator end() const {
		return Deferred<T>::get()->end();
	}

	// Check if the wrapped container is empty
	bool empty() const {
		return Deferred<T>::get()->empty();
	}

	const_reference operator[](size_type i) const {
		return Deferred<T>::get()->operator[](i);
	}

	reference operator[](size_type i) {
		return Deferred<T>::get()->operator[](i);
	}
};

template <typename T>
class DeferredIterator : public Deferred<T> {
public:
	typedef typename T::value_type value_type;
	typedef typename T::difference_type difference_type;
	typedef typename T::pointer pointer;
	typedef typename T::reference reference;

public:
	using Deferred<T>::Deferred;
};

// conversion operators

template<class T, class U>
Deferred<T> static_pointer_cast(const Deferred<U>& p) {
	return static_cast<T*>(p.get());
}

template<class T, class U>
Deferred<T> const_pointer_cast(const Deferred<U>& p) {
	return const_cast<T*>(p.get());
}

template<class T, class U>
Deferred<T> dynamic_pointer_cast(const Deferred<U>& p) {
	return dynamic_cast<T*>(p.get());
}

// comparison operators

template <typename T, typename U>
inline bool operator==(const Deferred<T>& left, const Deferred<U>& right) {
	return (left.get() == right.get());
}

template <typename T, typename U>
inline bool operator!=(const Deferred<T>& left, const Deferred<U>& right) {
	return (left.get() != right.get());
}

template <typename T, typename U>
inline bool operator==(const Deferred<T>& left, U* right) {
	return (left.get() == right);
}

template <typename T, typename U>
inline bool operator!=(const Deferred<T>& left, U* right) {
	return (left.get() != right);
}

template <typename T, typename U>
inline bool operator==(U* left, const Deferred<U>& right) {
	return (left == right.get());
}

template <typename T, typename U>
inline bool operator!=(U* left, const Deferred<U>& right) {
	return (left != right.get());
}

template <typename T>
inline bool operator==(const Deferred<T>& left, std::nullptr_t) noexcept {
	return (nullptr == left.get());
}

template <typename T>
inline bool operator==(std::nullptr_t, const Deferred<T>& right) noexcept {
	return (nullptr == right.get());
}

template <typename T>
inline bool operator!=(const Deferred<T>& left, std::nullptr_t) noexcept {
	return (nullptr != left.get());
}

template <typename T>
inline bool operator!=(std::nullptr_t, const Deferred<T>& right) noexcept {
	return (nullptr != right.get());
}

template <typename T>
inline bool operator<(const Deferred<T>& left, const Deferred<T>& right) {
	return std::less<T*>()(left.get(), right.get());
}

// swap
template <typename T>
void swap(Deferred<T>& lhs, Deferred<T>& rhs) {
	lhs.swap(rhs);
}

} // gotchangpdf

#endif /* _DEFERRED_H */
