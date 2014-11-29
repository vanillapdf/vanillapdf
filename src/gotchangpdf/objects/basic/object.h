#ifndef _OBJECT_H
#define _OBJECT_H

//#include <memory>

//#define Derive_Object_CRTP(Type) class Type : public Object_CRTP<Type>

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

	//typedef boost::variant <
	//	// first is null object
	//	Deferred<NullObject>,
	//	Deferred<MixedArrayObject>,
	//	Deferred<NameObject>,
	//	Deferred<DictionaryObject>,
	//	Deferred<FunctionObject>,
	//	Deferred<BooleanObject>,
	//	Deferred<IndirectObjectReference>,
	//	Deferred<IntegerObject>,
	//	Deferred<RealObject>,
	//	Deferred<StreamObject>,
	//	Deferred<LiteralString>,
	//	Deferred<HexadecimalString>
	//> DirectObject;

	class Object : public boost::intrusive_ref_counter<Object>
	{
	public:
		enum class Type : unsigned char
		{
			Unknown = 0,
			ArrayObject,
			Boolean,
			DictionaryObject,
			Function,
			IntegerObject,
			NameObject,
			//NameTree,
			NullObject,
			//NumberTree,
			RealObject,
			//Rectangle,
			StreamObject,
			HexadecimalString,
			LiteralString,
			IndirectObjectReference,
			IndirectObject
		};

		static const char* TypeName(Type type);
		virtual inline Type GetType(void) const = 0;

		Object();
		explicit Object(Type type);

		inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); }

		virtual ~Object() = 0;
	};
}

//#include "array_object.h"
//#include "name_object.h"
//#include "dictionary_object.h"
//#include "function_object.h"
//#include "null_object.h"
//#include "boolean_object.h"
//#include "indirect_object_reference.h"
//#include "integer_object.h"
//#include "real_object.h"
//#include "stream_object.h"
//#include "string_object.h"

#endif /* _OBJECT_H */
