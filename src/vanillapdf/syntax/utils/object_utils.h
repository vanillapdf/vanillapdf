#ifndef _OBJECT_UTILS_H
#define _OBJECT_UTILS_H

#include "utils/deferred.h"
#include "utils/exceptions.h"
#include "utils/conversion_utils.h"

#include "syntax/objects/object.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/real_object.h"
#include "syntax/objects/indirect_object_reference.h"
#include "syntax/objects/mixed_array_object.h"
#include "syntax/objects/array_object.h"

#include <map>
#include <sstream>

namespace vanillapdf {
namespace syntax {

#pragma region ConversionHelper

template <
	typename T,
	bool constructible
>
class ConversionHelperBase {
public:
	static T Get(Object* ptr, bool& result) {
		auto converted = dynamic_cast<typename T::deferred_ptr_type *>(ptr);
		if (nullptr == converted) {
			throw ConversionExceptionFactory<typename T::deferred_ptr_type>::Construct(ptr);
		}

		result = true;
		return T(converted);
	}
};

template <
	typename T
>
class ConversionHelperBase<T, true> {
public:
	static T Get(Object* ptr, bool& result) {
		auto converted = dynamic_cast<typename T::deferred_ptr_type *>(ptr);
		if (nullptr == converted) {
			result = false;
			return T();
		}

		result = true;
		return T(converted);
	}
};

template <typename T>
class ConversionHelper : public ConversionHelperBase<T, std::is_constructible<T>::value> {
};

template <>
class ConversionHelper<IntegerObjectPtr> {
public:
	static IntegerObjectPtr Get(Object* ptr, bool& result) {
		auto int_converted = dynamic_cast<IntegerObject *>(ptr);
		auto real_converted = dynamic_cast<RealObject *>(ptr);

		if (int_converted) {
			result = true;
			return IntegerObjectPtr(int_converted);
		}

		if (real_converted) {
			result = true;
			return make_deferred<IntegerObject>(*real_converted);
		}

		result = false;
		return IntegerObjectPtr();
	}
};

template <>
class ConversionHelper<RealObjectPtr> {
public:
	static RealObjectPtr Get(Object* ptr, bool& result) {
		auto int_converted = dynamic_cast<IntegerObject *>(ptr);
		auto real_converted = dynamic_cast<RealObject *>(ptr);

		if (int_converted) {
			result = true;
			return make_deferred<RealObject>(*int_converted);
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
class DereferenceHelper {
public:
	static T Get(Object* ptr, std::map<IndirectObjectReference, bool>& visited, bool& result) {
		bool is_ref = (ptr->GetType() == Object::Type::IndirectReference);
		if (!is_ref) {
			return ConversionHelper<T>::Get(ptr, result);
		}

		auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
		if (nullptr == converted) {
			throw ConversionExceptionFactory<IndirectObjectReference>::Construct(ptr);
		}

		auto found = visited.find(*converted);
		if (found != visited.end() && found->second) {
			std::stringstream ss;
			ss << "Cyclic reference was found for " << converted->GetReferencedObjectNumber() << " " << converted->GetReferencedGenerationNumber() << " R";
			throw GeneralException(ss.str());
		}

		visited[*converted] = true;

		auto direct = converted->GetReferencedObject();
		auto direct_ptr = direct.get();
		return Get(direct_ptr, visited, result);
	}
};

#pragma endregion

#pragma region ObjectTypeFunctor

template <typename T>
class ObjectTypeFunctor {
public:
	static bool IsType(Object* obj) {
		bool passed = false;
		bool is_ref = (obj->GetType() == Object::Type::IndirectReference);
		if (is_ref) {
			std::map<IndirectObjectReference, bool> visited;
			auto result = DereferenceHelper<T>::Get(obj, visited, passed);
			return passed;
		}

		auto result = ConversionHelper<T>::Get(obj, passed);
		return passed;
	}

	static T Convert(Object* obj) {
		bool passed = false;
		bool is_ref = (obj->GetType() == Object::Type::IndirectReference);
		if (is_ref) {
			std::map<IndirectObjectReference, bool> visited;
			auto result = DereferenceHelper<T>::Get(obj, visited, passed);

			if (!passed) {
				throw ConversionExceptionFactory<T>::Construct(obj);
			}

			return result;
		}

		auto result = ConversionHelper<T>::Get(obj, passed);
		if (!passed) {
			throw ConversionExceptionFactory<T>::Construct(obj);
		}

		return result;
	}
};

template <>
class ObjectTypeFunctor<IndirectObjectReferencePtr> {
public:
	static bool IsType(Object* ptr) {
		auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
		return (nullptr != converted);
	}

	static IndirectObjectReferencePtr Convert(Object* ptr) {
		auto converted = dynamic_cast<IndirectObjectReference*>(ptr);
		if (nullptr == converted) {
			throw ConversionExceptionFactory<IndirectObjectReference>::Construct(ptr);
		}

		return IndirectObjectReferencePtr(converted);
	}
};

template <typename T>
class ObjectTypeFunctor<ArrayObjectPtr<T>> {
public:
	static bool IsType(Object* obj) {
		bool is_ref = (obj->GetType() == Object::Type::IndirectReference);
		if (is_ref) {
			bool found = false;
			std::map<IndirectObjectReference, bool> visited;
			auto converted = DereferenceHelper<ArrayObjectPtr<T>>::Get(obj, visited, found);
			if (found) {
				return true;
			}
		}

		bool is_array = ObjectTypeFunctor<MixedArrayObjectPtr>::IsType(obj);
		if (!is_array) {
			return false;
		}

		auto mixed = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
		for (auto& item : mixed) {
			if (!ObjectTypeFunctor<T>::IsType(item.get())) {
				return false;
			}
		}

		return true;
	}

	static ArrayObjectPtr<T> Convert(Object* obj) {
		bool is_ref = (obj->GetType() == Object::Type::IndirectReference);
		if (is_ref) {
			bool found = false;
			std::map<IndirectObjectReference, bool> visited;
			auto converted = DereferenceHelper<ArrayObjectPtr<T>>::Get(obj, visited, found);
			if (found) {
				return converted;
			}
		}

		auto mixed = ObjectTypeFunctor<MixedArrayObjectPtr>::Convert(obj);
		return ArrayObjectPtr<T>(pdf_new ArrayObject<T>(mixed));
	}
};

#pragma endregion

class ObjectUtils {
public:
	template <
		typename T,
		typename = typename std::enable_if<std::is_base_of<Object, T>::value>::type
	>
		static ObjectPtr GetObjectBase(Deferred<T> obj) {
		auto object_ptr = obj.get();
		return static_cast<Object*>(object_ptr);
	}

	template <typename T>
	static T ConvertTo(ObjectPtr obj) {
		auto object_ptr = obj.get();
		return ObjectTypeFunctor<T>::Convert(object_ptr);
	}

	template <typename T>
	static bool IsType(ObjectPtr obj) {
		auto object_ptr = obj.get();
		return ObjectTypeFunctor<T>::IsType(object_ptr);
	}

	template <
		typename T,
		typename = typename std::enable_if<instantiation_of<Deferred, T>::value ||
		std::is_base_of<Object, typename T::deferred_ptr_type>::value>::type
	>
		static T Clone(const typename T::deferred_ptr_type& obj) {
		// Template requirements
		// T - is instantiation of Deferred
		// T::value_type is derived from Object
		// Object implements clone
		return T(obj.Clone());
	}
};

} // syntax
} // vanillapdf

#endif /* _OBJECT_UTILS_H */
