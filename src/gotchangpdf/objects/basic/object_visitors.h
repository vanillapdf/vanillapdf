#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
#include "exception.h"
#include "containable.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"
#include "array_object.h"

#include <map>
#include <sstream>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	template <typename T>
	class IsTypeVisitor : public boost::static_visitor < bool >
	{
	public:
		bool operator()(IndirectObjectReferencePtr& obj) const
		{
			auto found = visited.find(*obj);
			if (found != visited.end()) {
				std::stringstream ss;
				ss << "Cyclic reference was found for " << obj->GetReferencedObjectNumber() << " " << obj->GetReferencedGenerationNumber() << " R";
				throw exceptions::Exception(ss.str());
			}

			visited[*obj] = true;

			auto direct = obj->GetReferencedObject();
			return direct.apply_visitor(*this);
		}

		inline bool operator()(const T&) const { return true; }

		template <typename U>
		inline bool operator()(const U&) const { return false; }

	private:
		mutable std::map<IndirectObjectReference, bool> visited;
	};

	template <>
	class IsTypeVisitor<IndirectObjectReferencePtr> : public boost::static_visitor < bool >
	{
	public:
		inline bool operator()(const IndirectObjectReferencePtr&) const { return true; }

		template <typename U>
		inline bool operator()(const U&) const { return false; }
	};

	template <typename T>
	class IsTypeVisitor<ArrayObjectPtr<T>> : public boost::static_visitor<bool>
	{
	public:
		bool operator()(MixedArrayObjectPtr& obj) const
		{
			IsTypeVisitor<T> visitor;
			for (auto& item : *obj) {
				if (!item.apply_visitor(visitor))
					return false;
			}

			return true;
		}

		template <typename U>
		inline bool operator()(const U&) const { return false; }
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
		explicit SetContainerVisitor(DictionaryObjectPtr& container)
			: _container(static_cast<Object*>(container.Content.get())),
			boost::static_visitor<void>() {}

		explicit SetContainerVisitor(MixedArrayObjectPtr& container)
			: _container(static_cast<Object*>(container.Content.get())),
			boost::static_visitor<void>() {}

		template <typename T>
		explicit SetContainerVisitor(ArrayObjectPtr<T>& container)
			: _container(static_cast<Object*>(container.Content.get())),
			boost::static_visitor<void>() {}

		SetContainerVisitor& operator=(const SetContainerVisitor&) = delete;
		SetContainerVisitor(const SetContainerVisitor&) = delete;

		template <typename T>
		inline void operator()(T& obj) const { obj->SetContainer(_container); }

	private:
		ContainerPtr _container;
	};

	template <typename T>
	class ObjectVisitor : public boost::static_visitor<T>
	{
	public:
		T operator()(IndirectObjectReferencePtr& obj) const
		{
			auto found = visited.find(*obj);
			if (found != visited.end()) {
				std::stringstream ss;
				ss << "Cyclic reference was found for " << obj->GetReferencedObjectNumber() << " " << obj->GetReferencedGenerationNumber() << " R";
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

	template <>
	class ObjectVisitor<IndirectObjectReferencePtr> : public boost::static_visitor<IndirectObjectReferencePtr>
	{
	public:
		inline IndirectObjectReferencePtr operator()(IndirectObjectReferencePtr& obj) const { return obj; }

		template <typename U>
		inline IndirectObjectReferencePtr operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
	};

	template <typename T>
	class ObjectVisitor<ArrayObjectPtr<T>> : public boost::static_visitor<ArrayObjectPtr<T>>
	{
	public:
		inline ArrayObjectPtr<T> operator()(MixedArrayObjectPtr& obj) const { return obj->CastToArrayType<T>(); }

		template <typename U>
		inline ArrayObjectPtr<T> operator()(const U&) const { throw exceptions::Exception("Type cast error"); }
	};
}

#endif /* _OBJECT_VISITORS_H */
