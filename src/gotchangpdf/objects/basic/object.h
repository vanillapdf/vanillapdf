#ifndef _OBJECT_H
#define _OBJECT_H

#include "fwd.h"
#include "deferred.h"
#include "smart_ptr.h"
#include "direct_object.h"

#include <boost/variant/variant.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
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

	class Object : public boost::intrusive_ref_counter<Object>
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
			//NameTree,
			Null,
			//NumberTree,
			Real,
			//Rectangle,
			Stream,
			HexadecimalString,
			LiteralString,
			IndirectReference,
			Indirect
		};

	public:
		Object();
		explicit Object(Type type);

		static const char* TypeName(Type type);
		virtual inline Type GetType(void) const = 0;

		inline void Release() const { boost::sp_adl_block::intrusive_ptr_release(this); }

		virtual ~Object() = 0;
	};
}

#endif /* _OBJECT_H */
