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
		U* GetAs(void) const { return dynamic_cast<U*>(boost::intrusive_ptr<T>::get()); }
	};
}

#endif /* _OBJECT_REFERENCE_WRAPPER_H */
