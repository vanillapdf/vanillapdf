#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "object.h"
#include "buffer.h"
#include "containerable.h"

#include <stddef.h>

namespace gotchangpdf
{
	template <typename Container>
	class NameObjectBase : public Object, public Container
	{

	};

	class NameObject : public NameObjectBase<ParentContainer<ContainerPtr>>
	{
	public:
		typedef Buffer value_type;

	public:
		struct Hasher
		{
			unsigned long operator()(const NameObject& t) const;
			bool operator()(const NameObject& first, const NameObject& second) const;
		};

		NameObject();
		explicit NameObject(const gotchangpdf::lexical::Token& token);
		explicit NameObject(const value_type& name);

		const value_type& Value() const;

		bool operator==(const NameObject& other) const;
		bool operator!=(const NameObject& other) const;
		bool operator<(const NameObject& other) const;

		bool Equals(const NameObject& other) const;

		virtual Object::Type GetType(void) const override;

		void SetName(value_type& name) { _value = name; }

		//private:
	public:
		value_type _value;
	};

	inline bool NameObject::operator==(const NameObject& other) const
	{
		return Equals(other);
	}

	inline bool NameObject::operator!=(const NameObject& other) const
	{
		return !Equals(other);
	}

	inline bool NameObject::operator<(const NameObject& other) const
	{
		return _value < other._value;
	}

	inline Object::Type NameObject::GetType(void) const
	{
		return Object::Type::NameObject;
	}

	inline bool NameObject::Equals(const NameObject& other) const
	{
		return _value.Equals(other._value);
	}

	inline const NameObject::value_type& NameObject::Value() const { return _value; }

	inline bool NameObject::Hasher::operator()(const NameObject& first, const NameObject& second) const
	{
		return first.Equals(second);
	}

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

#endif /* _NAME_OBJECT_H */
