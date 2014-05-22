#ifndef _OBJECT_H
#define _OBJECT_H

//#include <memory>

//#define Derive_Object_CRTP(Type) class Type : public Object_CRTP<Type>

#include "intrusive.h"
#include "object_reference_wrapper.h"

namespace gotchangpdf
{
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
		Type GetType(void) const;

		inline void SetParent(ObjectReferenceWrapper<Object> obj) { _parent = obj; }
		inline ObjectReferenceWrapper<Object> GetParent() const { return _parent; }

		//virtual Object* Clone(void) const = 0;

		Object();
		explicit Object(Type type);

		void Release();

		virtual ~Object() = 0;

	protected:
		Type _type;
		ObjectReferenceWrapper<Object> _parent = nullptr;
	};
	/*
	template <typename T>
	class Object_CRTP : public Object
	{
	public:
		Object_CRTP() : Object() {};
		explicit Object_CRTP(Type type) : Object(type) {};

		virtual Object* Clone(void) const  override { return new T(static_cast<T const&>(*this)); };
		virtual ~Object_CRTP() {};
	};
	*/
}

#endif /* _OBJECT_H */
