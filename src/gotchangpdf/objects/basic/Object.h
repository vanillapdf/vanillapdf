#ifndef _OBJECT_H
#define _OBJECT_H

//#include <memory>

//#define Derive_Object_CRTP(Type) class Type : public Object_CRTP<Type>

#include "i_object.h"
#include "intrusive.h"

namespace gotchangpdf
{
	class Object : public IObject
	{
	public:
		virtual Type GetType(void) const override;

		//virtual Object* Clone(void) const = 0;

		Object();
		explicit Object(Type type);

		virtual void Release() override;

		virtual ~Object() = 0;

	protected:
		Type _type;

		mutable long _intrusive_ref_count;
		friend void ::boost::intrusive_ptr_add_ref(Object*);
		friend void ::boost::intrusive_ptr_release(Object*);
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
