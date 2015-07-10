#ifndef _SMART_PTR
#define _SMART_PTR

#include "fwd.h"

#include <boost/smart_ptr/intrusive_ptr.hpp>

//#include "boost/static_assert.hpp"

#include <cassert>

namespace gotchangpdf
{
	template <typename T>
	class SmartPtr : public boost::intrusive_ptr<T>
	{
	public:
		typedef typename T value_type;

	public:
		SmartPtr() : boost::intrusive_ptr<T>() {}
		SmartPtr(T* data) : boost::intrusive_ptr<T>(data) {}

		template <typename U>
		SmartPtr(const SmartPtr<U>& other) : boost::intrusive_ptr<T>(other.get()) {}

		//ObjectReferenceWrapper& operator=(const ObjectReferenceWrapper& other) : _data(other._data) { return *this; }

		T* AddRefGet(void) const
		{
			auto ptr = get();
			ptr->AddRef();
			return ptr;
		}

		template <typename U>
		inline U* GetAs(void) const { return dynamic_cast<U*>(get()); }

	private:
		//BOOST_STATIC_ASSERT((std::is_base_of<boost::intrusive_ref_counter<T>, T>::value));
	};

	template<typename T,
	typename... ArgTypes> inline
	SmartPtr<T> make_intrusive(ArgTypes&&... args)
	{
		auto tmp = new T(args...);
		return SmartPtr<T>(tmp);
	}

	template<class T, class U> SmartPtr<T> static_wrapper_cast(SmartPtr<U> const &p) { return static_cast<T*>(p.get()); }
	template<class T, class U> SmartPtr<T> const_wrapper_cast(SmartPtr<U> const &p) { return const_cast<T*>(p.get()); }
	template<class T, class U> SmartPtr<T> dynamic_wrapper_cast(SmartPtr<U> const &p) { return dynamic_cast<T*>(p.get()); }
}

#endif /* _SMART_PTR */
