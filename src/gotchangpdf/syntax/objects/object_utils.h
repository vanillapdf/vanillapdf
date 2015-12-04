#ifndef _OBJECT_UTILS_H
#define _OBJECT_UTILS_H

#include "deferred.h"
#include "object.h"
#include "exception.h"
#include "indirect_object_reference.h"
#include "array_object.h"

#include <map>
#include <sstream>

namespace gotchangpdf
{
	namespace syntax
	{
		class ObjectUtils
		{
		public:
			template <typename T, typename = std::enable_if_t<std::is_base_of<Object, T>::value>>
			static ObjectPtr GetObjectBase(const Deferred<T>& obj) { return static_cast<Object*>(obj.Content.get()); }

			template <typename T>
			static T ConvertTo(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::Convert(obj); }

			template <typename T>
			static bool IsType(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::IsType(obj); }

		private:
			// This whole masquerade is because template functions cannot be partially specialized
			template <typename T>
			class ObjectTypeFunctor
			{
			public:
				static bool IsType(const ObjectPtr& obj)
				{
					std::map<IndirectObjectReference, bool> visited;
					bool passed = false;
					auto result = GetInternal<T>(obj, visited, passed);
					return passed;
				}

				static T Convert(const ObjectPtr& obj)
				{
					std::map<IndirectObjectReference, bool> visited;
					bool passed = false;
					auto result = GetInternal<T>(obj, visited, passed);
					if (!passed)
						throw Exception("Could not convert object to indirect destination type");

					return result;
				}

				template <typename T>
				static T GetInternal(const ObjectPtr& obj, std::map<IndirectObjectReference, bool>& visited, bool& result)
				{
					auto ptr = obj.Content.get();
					bool is_ref = (ptr->GetType() == Object::Type::IndirectReference);
					if (!is_ref) {
						auto converted = dynamic_cast<typename T::value_type *>(ptr);
						if (nullptr == converted) {
							return T();
						}

						result = true;
						return T(converted);
					}

					auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
					if (nullptr == converted)
						throw Exception("Could not convert object to indirect reference type");

					auto reference = IndirectObjectReferencePtr(converted);

					auto found = visited.find(*converted);
					if (found != visited.end()) {
						std::stringstream ss;
						ss << "Cyclic reference was found for " << converted->GetReferencedObjectNumber() << " " << converted->GetReferencedGenerationNumber() << " R";
						throw Exception(ss.str());
					}

					visited[*converted] = true;

					auto direct = converted->GetReferencedObject();
					return GetInternal<T>(direct, visited, result);
				}
			};			

			template <>
			class ObjectTypeFunctor<IndirectObjectReferencePtr>
			{
			public:
				static bool IsType(const ObjectPtr& obj)
				{
					auto ptr = obj.Content.get();
					auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
					return (nullptr != converted);
				}

				static IndirectObjectReferencePtr Convert(const ObjectPtr& obj)
				{
					auto ptr = obj.Content.get();
					auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
					if (nullptr == converted)
						throw Exception("Could not convert object to destination type");

					return IndirectObjectReferencePtr(converted);
				}
			};

			template <typename T>
			class ObjectTypeFunctor<ArrayObjectPtr<T>>
			{
			public:
				static bool IsType(const ObjectPtr& obj)
				{
					bool is_array = ObjectTypeFunctor<MixedArrayObjectPtr>::IsType(obj);
					if (!is_array)
						return false;

					auto converted = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
					for (auto& item : *converted) {
						if (!ObjectTypeFunctor<T>::IsType(item))
							return false;
					}

					return true;
				}

				static ArrayObjectPtr<T> Convert(const ObjectPtr& obj)
				{
					auto mixed = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
					return mixed->CastToArrayType<T>();
				}
			};
		};
	}
}

#endif /* _OBJECT_UTILS_H */
