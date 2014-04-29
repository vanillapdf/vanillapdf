#include "NumericObject.h"

#include <cassert>

namespace Pdf
{
	NumericObject::NumericObject(Type type) : Object(type) { assert(type == Object::Type::IntegerObject || type == Object::Type::RealObject); }
	NumericObject::~NumericObject() {}
}
