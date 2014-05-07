#include "NumericObject.h"

#include <cassert>

namespace gotchangpdf
{
	NumericObject::NumericObject(Type type) : Object(type) { assert(type == Object::Type::IntegerObject || type == Object::Type::RealObject); }
	NumericObject::~NumericObject() {}
}
