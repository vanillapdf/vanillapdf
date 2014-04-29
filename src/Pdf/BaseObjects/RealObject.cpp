#include "RealObject.h"
#include "CharacterSet.h"

#include <string>

namespace Pdf
{
	using namespace std;
	RealObject::RealObject() : NumericObject(Object::Type::RealObject), _value(0) {}
	RealObject::RealObject(double value) : NumericObject(Object::Type::RealObject), _value(value) {}

	RealObject::RealObject(const CharacterSet& value) : NumericObject(Object::Type::RealObject)
	{
		string str(value.begin(), value.end());
		_value = stod(str);
	}
}

