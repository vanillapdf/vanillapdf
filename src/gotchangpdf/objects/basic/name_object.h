#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "object.h"
#include "buffer.h"
#include "containable.h"

#include <cstddef>

namespace gotchangpdf
{
	class NameObject : public Containable, public Object
	{
	public:
		typedef BufferPtr value_type;
		friend std::ostream& operator<<(std::ostream& os, const NameObject& name);

	public:
		NameObject() = default;
		explicit NameObject(const lexical::Token& token);
		explicit NameObject(value_type name) : _value(name) {}

		inline value_type Value() const _NOEXCEPT { return _value; }

		inline bool operator==(const NameObject& other) const { return Equals(other); }
		inline bool operator!=(const NameObject& other) const { return !Equals(other); }
		inline bool operator<(const NameObject& other) const { return *_value < *other._value; }

		inline bool Equals(const NameObject& other) const { return _value->Equals(*other._value); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::Name; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

		void SetName(value_type name) { _value = name; }

		//private:
	public:
		value_type _value;

	private:
		ContainerPtr _container;
	};

	namespace constant
	{
		namespace Name
		{
#			define DECLARE_CONST_NAME(name)	const NameObjectPtr name = NameObject(Buffer(#name, sizeof(#name)))

			DECLARE_CONST_NAME(BitsPerComponent);
			DECLARE_CONST_NAME(Colors);
			DECLARE_CONST_NAME(Columns);
			DECLARE_CONST_NAME(DecodeParms);
			DECLARE_CONST_NAME(EarlyChange);
			DECLARE_CONST_NAME(Length);
			DECLARE_CONST_NAME(Type);
			DECLARE_CONST_NAME(ObjStm);
			DECLARE_CONST_NAME(N);
			DECLARE_CONST_NAME(First);
			DECLARE_CONST_NAME(Extends);
			DECLARE_CONST_NAME(Filter);
			DECLARE_CONST_NAME(FlateDecode);
			DECLARE_CONST_NAME(Index);
			DECLARE_CONST_NAME(Catalog);
			DECLARE_CONST_NAME(Pages);
			DECLARE_CONST_NAME(Page);
			DECLARE_CONST_NAME(Count);
			DECLARE_CONST_NAME(Kids);
			DECLARE_CONST_NAME(Container);
			DECLARE_CONST_NAME(MediaBox);
			DECLARE_CONST_NAME(Resources);
			DECLARE_CONST_NAME(Parent);
			DECLARE_CONST_NAME(Predictor);
			DECLARE_CONST_NAME(Prev);
			DECLARE_CONST_NAME(Root);
			DECLARE_CONST_NAME(Size);
			DECLARE_CONST_NAME(W);
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
