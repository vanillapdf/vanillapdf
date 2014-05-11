#ifndef _OBJECT_H
#define _OBJECT_H

//#include <memory>

//#define Derive_Object_CRTP(Type) class Type : public Object_CRTP<Type>

#include "intrusive.h"
#include "interface.h"

namespace gotchangpdf
{
	class Object : public virtual IObject
	{
	public:
		Type GetType(void) const;

		//virtual Object* Clone(void) const = 0;

		static const char* TypeName(Type type);

		Object();
		explicit Object(Type type);

		void Release();

		virtual ~Object() = 0;

	protected:
		Type _type;

		mutable long _intrusive_ref_count;

		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
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
