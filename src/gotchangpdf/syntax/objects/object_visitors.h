#ifndef _OBJECT_VISITORS_H
#define _OBJECT_VISITORS_H

#include "object.h"
#include "exception.h"
#include "containable.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"
#include "array_object.h"
#include "general_visitors.h"
#include "conversion_visitor.h"

#include <map>
#include <memory>
#include <sstream>

#include <boost/variant/static_visitor.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		template <typename T>
		class IsTypeVisitor : public boost::static_visitor<bool>
		{
		public:
			bool operator()(IndirectObjectReferencePtr& obj) const
			{
				auto found = visited.find(*obj);
				if (found != visited.end()) {
					std::stringstream ss;
					ss << "Cyclic reference was found for " << obj->GetReferencedObjectNumber() << " " << obj->GetReferencedGenerationNumber() << " R";
					throw Exception(ss.str());
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
		class IsTypeVisitor<IndirectObjectReferencePtr> : public boost::static_visitor<bool>
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

		public:
			inline bool operator()(const T&) const { return true; }

			template <typename U>
			inline bool operator()(const U&) const { return false; }
		};

		class ObjectBaseVisitor : public BaseVisitor<Object*>
		{
		};

		class ObjectBaseAddRefVisitor : public BaseAddRefVisitor<Object*>
		{
		};

		class SetContainerVisitor : public boost::static_visitor<void>
		{
		public:
			explicit SetContainerVisitor(const ContainerPtr container)
				: _container(container), boost::static_visitor<void>() {}

			SetContainerVisitor& operator=(const SetContainerVisitor&) = delete;
			SetContainerVisitor(const SetContainerVisitor&) = delete;

			template <typename T>
			inline void operator()(T& obj) const { obj->SetContainer(_container); }

		private:
			const ContainerPtr _container;
		};

		class SetFileVisitor : public boost::static_visitor<void>
		{
		public:
			explicit SetFileVisitor(std::weak_ptr<File> file) : _file(file) {}

			template <typename T>
			inline void operator()(const Deferred<T>& obj) const { obj->SetFile(_file); }

			void operator()(const MixedArrayObjectPtr& obj)
			{
				obj->SetFile(_file);
				for (auto item : *obj) {
					item.apply_visitor(*this);
				}
			}

			void operator()(const DictionaryObjectPtr& obj)
			{
				obj->SetFile(_file);
				for (auto item : *obj) {
					item.first->SetFile(_file);
					item.second.apply_visitor(*this);
				}
			}

			void operator()(const StreamObjectPtr& obj)
			{
				obj->SetFile(_file);

				auto header = obj->GetHeader();
				header->SetFile(_file);
				for (auto item : *header) {
					item.first->SetFile(_file);
					item.second.apply_visitor(*this);
				}
			}

		private:
			std::weak_ptr<File> _file;
		};
	}
}

#endif /* _OBJECT_VISITORS_H */
