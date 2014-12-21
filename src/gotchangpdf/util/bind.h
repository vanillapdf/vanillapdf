#ifndef _BIND_H
#define _BIND_H

#include "fwd.h"
#include "object.h"
#include "operators.h"
#include "dictionary_object.h"

#include <functional>
#include <boost/static_assert.hpp>

namespace gotchangpdf
{
	template <typename Child>
	class Bind
	{
	public:
		BOOST_STATIC_ASSERT((std::is_base_of<Object, typename DictionaryObjectPtr::value_type>::value));
		typedef std::function<Child(DictionaryObjectPtr)> GetChildFunction;

	public:
		Bind(DictionaryObjectPtr container, GetChildFunction func)
			: _child(), _container(container), _func(func) {}

		Child operator()() const { return GetChild(); }
		Child& operator*() const { return *GetChild(); }
		Child operator->() const { return GetChild(); }

	private:
		Child& GetChild() const;

		DictionaryObjectPtr _container;
		GetChildFunction _func;
		mutable Child _child;
	};

	template <typename Child>
	Child& Bind<Child>::GetChild() const
	{
		if (_child->GetContainer() != _container)
			_child = _func(_container);

		return _child;
	}
}

#endif /* _BIND_H */
