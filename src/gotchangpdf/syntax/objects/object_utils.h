#ifndef _OBJECT_UTILS_H
#define _OBJECT_UTILS_H

#include "syntax_fwd.h"
#include "deferred.h"
#include "exception.h"
#include "objects.h"
#include "util.h"

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
			static ObjectPtr GetObjectBase(const Deferred<T>& obj) { return static_cast<Object*>(obj.get()); }

			template <typename T>
			static T ConvertTo(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::Convert(obj); }

			template <typename T>
			static bool IsType(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::IsType(obj); }

			template <
				typename T,
				typename = std::enable_if_t<instantiation_of<Deferred, T>::value || std::is_base_of<Object, T::value_type>::value>>
			static T Clone(const T& obj)
			{
				// Template requirements
				// T - is instantiation of Deferred
				// T::value_type is derived from Object
				// Object implements clone
				return obj->Clone();
			}

			static bool ValueEquals(const ObjectPtr& first, const ObjectPtr& second)
			{
				if (first->GetType() != second->GetType())
					return false;

				auto type = first->GetType();
				switch (type) {
				case Object::Type::Array:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<MixedArrayObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					auto first_size = first_converted->Size();
					auto second_size = second_converted->Size();
					if (first_size != second_size)
						return false;

					for (unsigned int i = 0; i < first_size; ++i)
						if (!ValueEquals(first_converted->At(i), second_converted->At(i)))
							return false;

					return true;
				}
				case Object::Type::Boolean:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<BooleanObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return (first_converted->GetValue() == second_converted->GetValue());
				}
				case Object::Type::Dictionary:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<DictionaryObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

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
					auto first_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<IndirectObjectReference*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return (*first_converted == *second_converted);
				}
				case Object::Type::Integer:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<IntegerObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return (first_converted->GetValue() == second_converted->GetValue());
				}
				case Object::Type::Name:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<NameObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return first_converted->Equals(*second_converted);
				}
				case Object::Type::Null:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<NullObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return (*first_converted == *second_converted);
				}
				case Object::Type::Real:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<RealObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return (first_converted->GetValue() == second_converted->GetValue());
				}
				case Object::Type::Stream:
				{
					auto first_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<StreamObject*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

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
					auto first_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(first.get());
					auto second_converted = ConvertUtils<Object*>::ConvertTo<StringObjectBase*>(second.get());

					assert(nullptr != first_converted && nullptr != second_converted);
					if (nullptr == first_converted || nullptr == second_converted)
						return false;

					return first_converted->Equals(*second_converted);
				}
				default:
					assert(!"Unknown object type");
				}

				return false;
			}

		private:
			// This whole masquerade is because template functions cannot be partially specialized

			template <typename T>
			class ConversionHelper
			{
			public:
				static T Get(const ObjectPtr& obj, bool& result)
				{
					return GetInternal<std::is_constructible<T>::value>(obj, result);
				}

			private:
				template <bool Constructible>
				static T GetInternal(const ObjectPtr& obj, bool& result)
				{
					auto ptr = obj.get();
					auto converted = dynamic_cast<typename T::value_type *>(ptr);
					if (nullptr == converted) {
						throw ConversionExceptionFactory<typename T::value_type>::Construct(obj);
					}

					result = true;
					return T(converted);
				}

				template <>
				static T GetInternal<true>(const ObjectPtr& obj, bool& result)
				{
					auto ptr = obj.get();
					auto converted = dynamic_cast<typename T::value_type *>(ptr);
					if (nullptr == converted) {
						result = false;
						return T();
					}

					result = true;
					return T(converted);
				}
			};

			template <>
			class ConversionHelper<IntegerObjectPtr>
			{
			public:
				static IntegerObjectPtr Get(const ObjectPtr& obj, bool& result)
				{
					auto ptr = obj.get();
					auto int_converted = dynamic_cast<IntegerObject *>(ptr);
					auto real_converted = dynamic_cast<RealObject *>(ptr);

					if (int_converted) {
						result = true;
						return IntegerObjectPtr(int_converted);
					}

					if (real_converted) {
						result = true;
						return IntegerObjectPtr(*real_converted);
					}

					result = false;
					return IntegerObjectPtr();
				}
			};

			template <>
			class ConversionHelper<RealObjectPtr>
			{
			public:
				static RealObjectPtr Get(const ObjectPtr& obj, bool& result)
				{
					auto ptr = obj.get();
					auto int_converted = dynamic_cast<IntegerObject *>(ptr);
					auto real_converted = dynamic_cast<RealObject *>(ptr);

					if (int_converted) {
						result = true;
						return RealObjectPtr(*int_converted);
					}

					if (real_converted) {
						result = true;
						return RealObjectPtr(real_converted);
					}

					result = false;
					return RealObjectPtr();
				}
			};

			template <typename T>
			class DereferenceHelper
			{
			public:
				static T Get(const ObjectPtr& obj, std::map<IndirectObjectReference, bool>& visited, bool& result)
				{
					auto ptr = obj.get();
					bool is_ref = (ptr->GetType() == Object::Type::IndirectReference);
					if (!is_ref) {
						return ConversionHelper<T>::Get(obj, result);
					}

					auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
					if (nullptr == converted)
						throw ConversionExceptionFactory<IndirectObjectReference>::Construct(obj);

					auto found = visited.find(*converted);
					if (found != visited.end()) {
						std::stringstream ss;
						ss << "Cyclic reference was found for " << converted->GetReferencedObjectNumber() << " " << converted->GetReferencedGenerationNumber() << " R";
						throw GeneralException(ss.str());
					}

					visited[*converted] = true;

					auto direct = converted->GetReferencedObject();
					return Get(direct, visited, result);
				}
			};

			template <typename T>
			class ObjectTypeFunctor
			{
			public:
				static bool IsType(const ObjectPtr& obj)
				{
					std::map<IndirectObjectReference, bool> visited;
					bool passed = false;
					auto result = DereferenceHelper<T>::Get(obj, visited, passed);
					return passed;
				}

				static T Convert(const ObjectPtr& obj)
				{
					std::map<IndirectObjectReference, bool> visited;
					bool passed = false;
					auto result = DereferenceHelper<T>::Get(obj, visited, passed);
					if (!passed)
						throw ConversionExceptionFactory<T>::Construct(obj);

					return result;
				}
			};

			template <>
			class ObjectTypeFunctor<IndirectObjectReferencePtr>
			{
			public:
				static bool IsType(const ObjectPtr& obj)
				{
					auto ptr = obj.get();
					auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
					return (nullptr != converted);
				}

				static IndirectObjectReferencePtr Convert(const ObjectPtr& obj)
				{
					auto ptr = obj.get();
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
					bool found = false;
					std::map<IndirectObjectReference, bool> visited;
					DereferenceHelper<ArrayObjectPtr<T>>::Get(obj, visited, found);
					if (found)
						return true;

					bool is_array = ObjectTypeFunctor<MixedArrayObjectPtr>::IsType(obj);
					if (!is_array)
						return false;

					auto mixed = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
					for (auto& item : *mixed) {
						if (!ObjectTypeFunctor<T>::IsType(item))
							return false;
					}

					return true;
				}

				static ArrayObjectPtr<T> Convert(const ObjectPtr& obj)
				{
					bool found = false;
					std::map<IndirectObjectReference, bool> visited;
					auto converted = DereferenceHelper<ArrayObjectPtr<T>>::Get(obj, visited, found);
					if (found)
						return converted;

					auto mixed = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
					return ArrayObjectPtr<T>(mixed);
				}
			};
		};
	}
}

#endif /* _OBJECT_UTILS_H */
