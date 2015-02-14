#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
#include "exception.h"
#include "containable.h"
#include "indirect_object_reference.h"

#include <map>
#include <sstream>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	class IsNullVisitor : public boost::static_visitor<bool>
	{
	public:
		inline bool operator()(const NullObjectPtr&) const { return true; }

		template <typename T>
		inline bool operator()(const T&) const { return false; }
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
		inline T operator()(const U&) const { throw exceptions::Exception("Type cast error"); }

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

		SetContainerVisitor& operator=(const SetContainerVisitor&) = delete;
		SetContainerVisitor(const SetContainerVisitor&) = delete;

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
		inline T operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
	};

	template <typename T>
	class ObjectVisitor<ArrayObjectPtr<T>> : public boost::static_visitor<ArrayObjectPtr<T>>
	{
	public:
		inline ArrayObjectPtr<T> operator()(ArrayObjectPtr<T>& obj) const { return obj; }

		inline ArrayObjectPtr<T> operator()(MixedArrayObjectPtr& obj) const { return obj->CastToArrayType<T>(); }

		template <typename U>
		inline ArrayObjectPtr<T> operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
	};
}

#endif /* _OBJECT_VISITORS_H */
