#ifndef _BIND_H
#define _BIND_H

#include "fwd.h"
#include "smart_ptr.h"
#include "object.h"
#include "high_level_object.h"

#include "boost/static_assert.hpp"

#include <functional>

namespace gotchangpdf
{
	template <typename Container, typename Child>
	class Bind
	{
	public:
		typedef std::function<SmartPtr<Child>(SmartPtr<Container>)> GetChildFunction;

		Bind(SmartPtr<Container> container, GetChildFunction func)
			: _child(nullptr), _container(container), _func(func) {}

		SmartPtr<Child> operator()() const { return GetChild(); }
		Child& operator*() const { return *GetChild(); }
		SmartPtr<Child> operator->() const { return GetChild(); }

	private:
		BOOST_STATIC_ASSERT((std::is_base_of<Object, Container>::value));

		mutable SmartPtr<Child> _child;
		SmartPtr<Container> _container;
		GetChildFunction _func;

		SmartPtr<Child> GetChild() const
		{
			if (nullptr == _child || _child->GetContainer() != _container)
				_child = _func(_container);

			return _child;
		}
	};
}

#endif /* _BIND_H */
