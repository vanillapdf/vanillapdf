#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
//#include "direct_object.h"
#include "exception.h"
#include "indirect_object_reference.h"
//#include "stream_object.h"

#include <map>
#include <sstream>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	class IsNullVisitor : public boost::static_visitor<bool>
	{
	public:

		//template <typename T>
		//inline bool operator()(T obj) const { return std::is_same<T, NullObjectPtr>::value; }

		inline bool operator()(const NullObjectPtr& obj) const { return true; }

		template <typename T>
		inline bool operator()(const T& obj) const { return false; }
	};

	template <typename T>
	class KillIndirectionVisitor : public boost::static_visitor<T>
	{
	public:
		T operator()(IndirectObjectReferencePtr& obj) const
		{
			auto found = visited.find(*obj);
			if (found != visited.end()) {
				std::stringstream ss;
				ss << "Cyclic reference was found for " << obj->GetObjectNumber()->Value() << " " << obj->GetGenerationNumber()->Value() << " R";
				throw exceptions::Exception(ss.str());
			}

			visited[*obj] = true;

			auto direct = obj->GetReferencedObject();
			return direct.apply_visitor(*this);
		}

		inline T operator()(T& obj) const { return obj; }

		template <typename U>
		inline T operator()(const U& obj) const { throw exceptions::Exception("Type cast error"); }

	private:
		mutable std::map<IndirectObjectReference, bool> visited;
	};

	class ObjectBaseVisitor : public boost::static_visitor<Object*>
	{
	public:
		template <typename T>
		inline Object* operator()(T& obj) const { return obj.Content.get(); }
	};

	class ObjectBaseAddRefVisitor : public boost::static_visitor<Object*>
	{
	public:
		template <typename T>
		inline Object* operator()(T& obj) const
		{
			auto result = obj.Content.get();
			result->AddRef();
			return result;
		}
	};

	class SetContainerVisitor : public boost::static_visitor<void>
	{
	public:
		explicit SetContainerVisitor(const ContainerPtr& container)
			: _container(container),
			boost::static_visitor<void>() {}

		template <typename T>
		inline void operator()(T& obj) const { obj->SetContainer(_container); }

	private:
		const ContainerPtr& _container;
	};

	template <typename T>
	class ObjectVisitor : public boost::static_visitor<T>
	{
	public:
		inline T operator()(T& obj) const { return obj; }

		template <typename U>
		inline T operator()(const U& obj) const { throw exceptions::Exception("Type cast error"); }
	};
}

#endif /* _OBJECT_VISITORS_H */
