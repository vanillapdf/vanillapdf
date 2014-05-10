#ifndef _OBJECT_REFERENCE_WRAPPER_H
#define _OBJECT_REFERENCE_WRAPPER_H

#include "fwd.h"
#include "intrusive.h"

#include "boost/intrusive_ptr.hpp"

#include <cassert>

namespace gotchangpdf
{
	template <typename T>
	class ObjectReferenceWrapper
	{
	public:
		ObjectReferenceWrapper() : _data(nullptr) {}

		template <typename U>
		ObjectReferenceWrapper(U* data) : _data(boost::intrusive_ptr<T>(dynamic_cast<T*>(data))) {}

		template <typename U>
		ObjectReferenceWrapper(boost::intrusive_ptr<U> data) : _data(boost::dynamic_pointer_cast<T>(data)) {}

		template <typename U>
		ObjectReferenceWrapper(const ObjectReferenceWrapper<U>& other) : ObjectReferenceWrapper(other.Get()) {}

		//ObjectReferenceWrapper& operator=(const ObjectReferenceWrapper& other) : _data(other._data) { return *this; }

		T* AddRefGet(void) const
		{
			auto ptr = _data.get();
			boost::intrusive_ptr_add_ref(ptr);
			return ptr;
		}

		T* Get(void) const { return _data.get(); }

		template <typename U>
		U* GetAs(void) const { return dynamic_cast<U*>(_data.get()); }

		T& operator*() const
		{
			assert(_data != nullptr);
			return *_data.get();
		}

		T* operator->() const
		{
			assert(_data != nullptr);
			return _data.get();
		}

	private:
		boost::intrusive_ptr<T> _data;
	};

	template<class T> inline bool operator==(ObjectReferenceWrapper<T> const &p, std::nullptr_t) { return p.Get() == nullptr; }
	template<class T> inline bool operator==(std::nullptr_t, ObjectReferenceWrapper<T> const &p) { return p.Get() == nullptr; }

	template<class T, class U> ObjectReferenceWrapper<T> static_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return static_cast<T*>(p.Get()); }
	template<class T, class U> ObjectReferenceWrapper<T> const_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return const_cast<T*>(p.Get()); }
	template<class T, class U> ObjectReferenceWrapper<T> dynamic_wrapper_cast(ObjectReferenceWrapper<U> const &p) { return dynamic_cast<T*>(p.Get()); }
}

#endif /* _OBJECT_REFERENCE_WRAPPER_H */
