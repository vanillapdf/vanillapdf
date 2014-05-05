#include "RealObject.h"
#include "Buffer.h"

#include <string>

namespace Pdf
{
	using namespace std;
	RealObject::RealObject() : NumericObject(Object::Type::RealObject), _value(0) {}
	RealObject::RealObject(double value) : NumericObject(Object::Type::RealObject), _value(value) {}
    RealObject::RealObject(const Buffer& value) : NumericObject(Object::Type::RealObject), _value(stod(value.ToString())) {}
}

