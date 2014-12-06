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
		typedef std::function<Child(DictionaryObjectPtr)> GetChildFunction;

		Bind(DictionaryObjectPtr container, GetChildFunction func)
			: _child(), _container(container), _func(func) {}

		Child operator()() const { return GetChild(); }
		Child& operator*() const { return *GetChild(); }
		Child operator->() const { return GetChild(); }

	private:
		BOOST_STATIC_ASSERT((std::is_base_of<Object, typename DictionaryObjectPtr::value_type>::value));

		mutable Child _child;
		DictionaryObjectPtr _container;
		GetChildFunction _func;

		Child& GetChild() const
		{
			if (!_child.HasContents() || _child->GetContainer() != _container)
				_child = _func(_container);

			return _child;
		}
	};
}

#endif /* _BIND_H */
