#ifndef _OBJECT_UTILS_H
#define _OBJECT_UTILS_H

#include "syntax_fwd.h"
#include "deferred.h"
#include "exception.h"
//#include "objects.h"
#include "util.h"

#include "object.h"
#include "integer_object.h"
#include "real_object.h"
#include "indirect_object_reference.h"
#include "mixed_array_object.h"

#include <map>
#include <sstream>

namespace gotchangpdf
{
	namespace syntax
	{

		#pragma region ConversionHelper

		template <
			typename T,
			bool constructible
		>
		class ConversionHelperBase
		{
		public:
			static T Get(const ObjectPtr& obj, bool& result)
			{
				auto ptr = obj.get();
				auto converted = dynamic_cast<typename T::value_type *>(ptr);
				if (nullptr == converted) {
					throw ConversionExceptionFactory<typename T::value_type>::Construct(obj);
				}

				result = true;
				return T(converted);
			}
		};

		template <
			typename T
		>
		class ConversionHelperBase<T, true>
		{
		public:
			static T Get(const ObjectPtr& obj, bool& result)
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

		template <typename T>
		class ConversionHelper : public ConversionHelperBase<T, std::is_constructible<T>::value>
		{
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

		#pragma endregion

		#pragma region DereferenceHelper

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
				if (found != visited.end() && found->second) {
					std::stringstream ss;
					ss << "Cyclic reference was found for " << converted->GetReferencedObjectNumber() << " " << converted->GetReferencedGenerationNumber() << " R";
					throw GeneralException(ss.str());
				}

				visited[*converted] = true;

				auto direct = converted->GetReferencedObject();
				return Get(direct, visited, result);
			}
		};

		#pragma endregion

		#pragma region ObjectTypeFunctor

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

		#pragma endregion

		class ObjectUtils
		{
		public:
			template <
				typename T,
				typename = typename std::enable_if<std::is_base_of<Object, T>::value>::type
			>
				static ObjectPtr GetObjectBase(const Deferred<T>& obj) { return static_cast<Object*>(obj.get()); }

			template <typename T>
			static T ConvertTo(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::Convert(obj); }

			template <typename T>
			static bool IsType(const ObjectPtr& obj) { return ObjectTypeFunctor<T>::IsType(obj); }

			template <
				typename T,
				typename = typename std::enable_if<instantiation_of<Deferred, T>::value ||
				std::is_base_of<Object, typename T::value_type>::value>::type
			>
				static T Clone(const T& obj)
			{
				// Template requirements
				// T - is instantiation of Deferred
				// T::value_type is derived from Object
				// Object implements clone
				return T(obj->Clone());
			}

			static bool ValueEquals(const ObjectPtr& first, const ObjectPtr& second);
		};
	}
}

#endif /* _OBJECT_UTILS_H */
