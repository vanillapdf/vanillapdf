#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "object.h"
#include "buffer.h"
#include "containable.h"

#include <stddef.h>

namespace gotchangpdf
{
	class NameObject : public Containable
	{
	public:
		typedef Buffer value_type;

	public:
		NameObject() = default;
		explicit NameObject(const gotchangpdf::lexical::Token& token);
		explicit NameObject(const value_type& name);

		inline const value_type& Value() const { return _value; }

		inline bool operator==(const NameObject& other) const { return Equals(other); }
		inline bool operator!=(const NameObject& other) const { return !Equals(other); }
		inline bool operator<(const NameObject& other) const { return _value < other._value; }

		inline bool Equals(const NameObject& other) const { return _value.Equals(other._value); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::NameObject; }

		void SetName(value_type& name) { _value = name; }

		//private:
	public:
		value_type _value;
	};

	namespace constant
	{
		namespace Name
		{
#			define DECLARE_CONST_NAME(name)	const NameObject name = NameObject(Buffer(#name, sizeof(#name)))

			DECLARE_CONST_NAME(Length);
			DECLARE_CONST_NAME(Type);
			DECLARE_CONST_NAME(ObjStm);
			DECLARE_CONST_NAME(N);
			DECLARE_CONST_NAME(First);
			DECLARE_CONST_NAME(Extends);
			DECLARE_CONST_NAME(Filter);
			DECLARE_CONST_NAME(FlateDecode);
			DECLARE_CONST_NAME(Catalog);
			DECLARE_CONST_NAME(Pages);
			DECLARE_CONST_NAME(Page);
			DECLARE_CONST_NAME(Count);
			DECLARE_CONST_NAME(Kids);
			DECLARE_CONST_NAME(Container);
			DECLARE_CONST_NAME(MediaBox);
			DECLARE_CONST_NAME(Resources);
			DECLARE_CONST_NAME(Parent);
			DECLARE_CONST_NAME(Root);
		}
	}
}

namespace std
{
	template <> struct hash<gotchangpdf::NameObject>
	{
		size_t operator()(const gotchangpdf::NameObject& name) const;
	};
}

#endif /* _NAME_OBJECT_H */
