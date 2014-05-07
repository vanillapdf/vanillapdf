#include "real_object.h"
#include "token.h"

#include <string>

namespace gotchangpdf
{
	RealObject::RealObject() : NumericObject(Object::Type::RealObject), _value(0) {}
	RealObject::RealObject(double value) : NumericObject(Object::Type::RealObject), _value(value) {}
	RealObject::RealObject(const lexical::Token& token) : NumericObject(Object::Type::RealObject), _value(stod(token.value().ToString())) {}
}

