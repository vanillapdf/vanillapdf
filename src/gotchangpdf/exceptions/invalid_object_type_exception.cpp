#include "precompiled.h"
#include "invalid_object_type_exception.h"

#include <sstream>

namespace gotchangpdf
{
	namespace exceptions
	{
		using namespace std;

		InvalidObjectTypeException::InvalidObjectTypeException() : Exception() {}

		InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj)
			: Exception(string("Object ") + gotchangpdf::Object::TypeName(obj.GetType()) + string(" is invalid")) {}

		InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj, Object::Type type)
			: Exception(string("Object ") + gotchangpdf::Object::TypeName(obj.GetType()) + string(" is invalid, expected was ") + gotchangpdf::Object::TypeName(type)) {}
	}
}

