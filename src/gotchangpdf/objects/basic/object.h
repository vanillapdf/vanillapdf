#ifndef _OBJECT_H
#define _OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "unknown_interface.h"

namespace gotchangpdf
{
	class Object : public IUnknown
	{
	public:
		enum class Type : unsigned char
		{
			Array,
			Boolean,
			Dictionary,
			Function,
			Integer,
			Name,
			Null,
			Real,
			Stream,
			HexadecimalString,
			LiteralString,
			IndirectReference
		};

	public:
		Object() = default;
		virtual ~Object() = default;
		explicit Object(Type type);

		static const char* TypeName(Type type);
		virtual inline Type GetType(void) const = 0;
	};
}

#endif /* _OBJECT_H */
