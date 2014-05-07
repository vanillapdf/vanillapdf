#include "InvalidObjectTypeException.h"

#include <sstream>

namespace Pdf
{
	using namespace std;

	InvalidObjectTypeException::InvalidObjectTypeException() : Exception() {}

	InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj)
		: Exception(string("Object ") + Pdf::Object::TypeName(obj.GetType()) + string(" is invalid")) {}

	InvalidObjectTypeException::InvalidObjectTypeException(const Object& obj, Object::Type type)
		: Exception(string("Object ") + Pdf::Object::TypeName(obj.GetType()) + string(" is invalid, expected was ") + Pdf::Object::TypeName(type)) {}

}

