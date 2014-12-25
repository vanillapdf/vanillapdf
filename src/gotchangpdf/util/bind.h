#ifndef _BIND_H
#define _BIND_H
/*
#include "fwd.h"
#include "object.h"
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
		typedef std::function<ContainerPtr(Child)> GetContainerFunction;

	public:
		Bind(DictionaryObjectPtr container, GetChildFunction child_func, GetContainerFunction container_func)
			: _container(container), _get_child(child_func), _get_container(container_func) {}

		Child operator()() const { return GetChild(); }
		Child& operator*() const { return *GetChild(); }
		Child operator->() const { return GetChild(); }

		void Release()
		{
			_child.reset();
			_container.reset();

			active = false;
		}

	private:
		Child& GetChild() const;

		DictionaryObjectPtr _container;
		GetChildFunction _get_child;
		GetContainerFunction _get_container;
		mutable Child _child;

		bool active = true;
	};

	template <typename Child>
	Child& Bind<Child>::GetChild() const
	{
		if (!active)

		IsNullVisitor is_null;
		auto child_container = _get_container(_child);
		if (!child_container.apply_visitor(is_null)) {
			ObjectVisitor<DictionaryObjectPtr> visitor;
			auto dictionary = child_container.apply_visitor(visitor);
			if (*dictionary != *_container)
				_child = _get_child(_container);
		}

		return _child;
	}

	template <typename Child>
	ContainerPtr GetBasicObjectContainer(const Child& child)
	{
		BOOST_STATIC_ASSERT((std::is_base_of<Containable, typename Child::value_type>::value));
		return child->GetContainer();
	}

	template <typename Child>
	ContainerPtr GetHighLevelObjectContainer(const Child& child)
	{
		auto obj = child->GetObject();
		return obj->GetContainer();
	}

#define BIND_BASIC_OBJECT(container, child_type, child_function) \
	Bind<child_type>(container, child_function, &GetBasicObjectContainer<child_type>)

#define BIND_HIGH_OBJECT(container, child_type, child_function) \
	Bind<child_type>(container, child_function, &GetHighLevelObjectContainer<child_type>)
}
*/

#endif /* _BIND_H */
