#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "buffer.h"
#include "containerable.h"

namespace gotchangpdf
{
	class StringObject : public Object
	{
	public:
		Buffer _value;

	protected:
		StringObject();
		explicit StringObject(const Buffer& value);
	};

	class HexadecimalString : public StringObject, public ParentContainer<ContainerPtr>
	{
	public:
		HexadecimalString() = default;
		explicit HexadecimalString(const Buffer& value);
		explicit HexadecimalString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::HexadecimalString; }

	//private:
	public:
		std::string _hexadecimal;
	};

	class LiteralString : public StringObject, public ParentContainer<ContainerPtr>
	{
	public:
		LiteralString() = default;
		explicit LiteralString(const Buffer& value);
		explicit LiteralString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::LiteralString; }
	};
}

#endif /* _STRING_OBJECT_H */
