#ifndef _OBJECT_H
#define _OBJECT_H

//#include <memory>

//#define Derive_Object_CRTP(Type) class Type : public Object_CRTP<Type>

#include "smart_ptr.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace gotchangpdf
{
	class Object;

	typedef SmartPtr<Object> ObjectPtr;

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

		inline void SetContainer(ObjectPtr obj) { _container = obj; }
		inline ObjectPtr GetContainer() const { return _container; }

		//virtual Object* Clone(void) const = 0;

		Object();
		explicit Object(Type type);

		inline void Release() { boost::sp_adl_block::intrusive_ptr_release(this); };

		virtual ~Object() = 0;

	protected:
		ObjectPtr _container = nullptr;
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
