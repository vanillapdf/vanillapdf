#ifndef _INVALID_TYPE_EXCEPTION_H
#define _INVALID_TYPE_EXCEPTION_H

#include "BaseObjects/Object.h"
#include "Exceptions/Exception.h"

namespace Pdf
{
	class InvalidObjectTypeException : public Exception
	{
	public:
		InvalidObjectTypeException();
		explicit InvalidObjectTypeException(const Object& obj);
		InvalidObjectTypeException(const Object& obj, Object::Type type);
	};
}

#endif /* _INVALID_TYPE_EXCEPTION_H */
