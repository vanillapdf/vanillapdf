#include "RealObject.h"
#include "Token.h"

#include <string>

namespace Pdf
{
	RealObject::RealObject() : NumericObject(Object::Type::RealObject), _value(0) {}
	RealObject::RealObject(double value) : NumericObject(Object::Type::RealObject), _value(value) {}
	RealObject::RealObject(const Lexical::Token& token) : NumericObject(Object::Type::RealObject), _value(stod(token.value().ToString())) {}
}

