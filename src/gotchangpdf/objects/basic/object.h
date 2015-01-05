#ifndef _OBJECT_H
#define _OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "unknown_interface.h"

namespace gotchangpdf
{
	/*
	typedef boost::variant <
		Deferred<NullObject>,
		Deferred<LiteralString>,
		Deferred<HexadecimalString>
	> StringObjectPtr;

	typedef boost::variant <
		Deferred<NullObject>,
		Deferred<IntegerObject>,
		Deferred<RealObject>
	> NumericObjectPtr;
	*/
	class Object : public IUnknown
	{
	public:
		enum class Type : unsigned char
		{
			Unknown = 0,
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
		/*
	private:
		types::integer _obj_number = 0;
		types::ushort _gen_number = 0;
		types::stream_offset _offset = std::_BADOFF;
		*/
	};
}

#endif /* _OBJECT_H */
