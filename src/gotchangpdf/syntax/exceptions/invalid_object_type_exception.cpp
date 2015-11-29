#include "precompiled.h"
#include "invalid_object_type_exception.h"

#include <sstream>

namespace gotchangpdf
{
	namespace syntax
	{
		InvalidObjectTypeException::InvalidObjectTypeException() : Exception() {}

		InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj)
			: Exception(std::string("Object ") + Object::TypeName(obj.GetType()) + std::string(" is invalid")) {}

		InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj, Object::Type type)
			: Exception(std::string("Object ") + Object::TypeName(obj.GetType()) + std::string(" is invalid, expected was ") + Object::TypeName(type)) {}
	}
}

