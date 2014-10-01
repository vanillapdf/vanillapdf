#include "real_object.h"
#include "token.h"

#include <string>

namespace gotchangpdf
{
	RealObject::RealObject() {}
	RealObject::RealObject(double value) : _value(value) {}
	RealObject::RealObject(const lexical::Token& token) : _value(stod(token.Value().ToString())) {}
}

