#ifndef _INVALID_TYPE_EXCEPTION_H
#define _INVALID_TYPE_EXCEPTION_H

#include "object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace exceptions
	{
		class InvalidObjectTypeException : public Exception
		{
		public:
			InvalidObjectTypeException();
			explicit InvalidObjectTypeException(const Object& obj);
			InvalidObjectTypeException(const Object& obj, Object::Type type);
		};
	}
}

#endif /* _INVALID_TYPE_EXCEPTION_H */
