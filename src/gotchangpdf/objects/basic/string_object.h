#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "containable.h"
#include "buffer.h"

namespace gotchangpdf
{
	class StringObject : public Containable, public Object
	{
	public:
		BufferPtr _value;

		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

	protected:
		StringObject();
		explicit StringObject(BufferPtr value);

	private:
		ContainerPtr _container;
	};

	class HexadecimalString : public StringObject
	{
	public:
		HexadecimalString() = default;
		explicit HexadecimalString(BufferPtr value);
		explicit HexadecimalString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::HexadecimalString; }

	//private:
	public:
		std::string _hexadecimal;
	};

	class LiteralString : public StringObject
	{
	public:
		LiteralString() = default;
		explicit LiteralString(BufferPtr value);
		explicit LiteralString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::LiteralString; }
	};
}

#endif /* _STRING_OBJECT_H */
