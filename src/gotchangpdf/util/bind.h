#ifndef _BIND_H
#define _BIND_H

#include "fwd.h"
#include "object_reference_wrapper.h"
#include "object.h"

//#include "boost/static_assert.hpp"

//#include <cassert>
#include <functional>

namespace gotchangpdf
{
	template <typename Container, typename Child>
	class Bind
	{
	public:
		typedef std::function<ObjectReferenceWrapper<Child>(ObjectReferenceWrapper<Container>)> GetChildFunction;

		Bind(ObjectReferenceWrapper<Container> container, GetChildFunction func)
			: _child(nullptr), _container(container), _func(func) {}

		ObjectReferenceWrapper<Child> operator()() const { return GetChild(); }
		Child& operator*() const { return *GetChild(); }
		ObjectReferenceWrapper<Child> operator->() const { return GetChild(); }

	private:
		//BOOST_STATIC_ASSERT((std::is_base_of<Object, Child>::value));
		//BOOST_STATIC_ASSERT((std::is_base_of<Object, Container>::value));

		mutable ObjectReferenceWrapper<Child> _child;
		ObjectReferenceWrapper<Container> _container;
		GetChildFunction _func;

		ObjectReferenceWrapper<Child> GetChild() const
		{
			if (nullptr == _child || _child->GetContainer() != _container)
				_child = _func(_container);

			return _child;
		}
	};
}

#endif /* _BIND_H */
