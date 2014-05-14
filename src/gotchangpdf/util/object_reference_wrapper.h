#ifndef _OBJECT_REFERENCE_WRAPPER_H
#define _OBJECT_REFERENCE_WRAPPER_H

#include "fwd.h"
#include "intrusive.h"

#include "boost/intrusive_ptr.hpp"

#include <cassert>

namespace gotchangpdf
{
	template <typename T>
	class ObjectReferenceWrapper : public boost::intrusive_ptr<T>
	{
	public:
		ObjectReferenceWrapper() : boost::intrusive_ptr<T>() {}
		ObjectReferenceWrapper(T* data) : boost::intrusive_ptr<T>(data) {}

		template <typename U>
		ObjectReferenceWrapper(const ObjectReferenceWrapper<U>& other) : boost::intrusive_ptr<T>(other.get()) {}

		//ObjectReferenceWrapper& operator=(const ObjectReferenceWrapper& other) : _data(other._data) { return *this; }

		T* AddRefGet(void) const
		{
			auto ptr = boost::intrusive_ptr<T>::get();
			boost::intrusive_ptr_add_ref(ptr);
			return ptr;
		}

		template <typename U>
		inline U* GetAs(void) const { return dynamic_cast<U*>(boost::intrusive_ptr<T>::get()); }
	};

	template<class T, class U> ObjectReferenceWrapper<T> static_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return static_cast<T*>(p.get()); }
	template<class T, class U> ObjectReferenceWrapper<T> const_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return const_cast<T*>(p.get()); }
	template<class T, class U> ObjectReferenceWrapper<T> dynamic_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return dynamic_cast<T*>(p.get()); }
}

#endif /* _OBJECT_REFERENCE_WRAPPER_H */
