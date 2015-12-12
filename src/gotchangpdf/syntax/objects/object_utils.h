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

			static bool ValueEquals(const ObjectPtr& first, const ObjectPtr& second)
			{
				if (first->GetType() != second->GetType())
					return false;

				auto type = first->GetType();
				switch (type) {
				case Object::Type::Array:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(second.Content.get());

					auto first_size = first_converted->Size();
					auto second_size = second_converted->Size();
					if (first_size != second_size)
						return false;

					for (int i = 0; i < first_size; ++i)
						if (!ValueEquals(first_converted->At(i), second_converted->At(i)))
							return false;

					return true;
				}
				case Object::Type::Boolean:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Dictionary:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(second.Content.get());

					auto first_vals = first_converted->Values();
					auto second_vals = second_converted->Values();
					auto first_vals_size = first_vals.size();
					auto second_vals_size = second_vals.size();
					if (first_vals_size != second_vals_size)
						return false;

					for (unsigned int i = 0; i < second_vals_size; ++i)
						if (!ValueEquals(first_vals[i], second_vals[i]))
							return false;

					return true;
				}
				case Object::Type::IndirectReference:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Integer:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Name:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Null:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Real:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				case Object::Type::Stream:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(second.Content.get());

					auto first_header = first_converted->GetHeader();
					auto second_header = second_converted->GetHeader();
					if (!ValueEquals(first_header, second_header))
						return false;

					auto first_body = first_converted->GetBody();
					auto second_body = second_converted->GetBody();
					if (first_body != second_body)
						return false;

					return true;
				}
				case Object::Type::String:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(first.Content.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(second.Content.get());
					return (*first_converted == *second_converted);
				}
				default:
					assert(!"Unknown object type");
				}

				return false;
			}

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
						throw ConversionExceptionFactory<T>::Construct(obj);

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
						throw ConversionExceptionFactory<IndirectObjectReference>::Construct(obj);

					auto reference = IndirectObjectReferencePtr(converted);

					auto found = visited.find(*converted);
					if (found != visited.end()) {
						std::stringstream ss;
						ss << "Cyclic reference was found for " << converted->GetReferencedObjectNumber() << " " << converted->GetReferencedGenerationNumber() << " R";
						throw GeneralException(ss.str());
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
						throw ConversionExceptionFactory<IndirectObjectReference>::Construct(obj);

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
