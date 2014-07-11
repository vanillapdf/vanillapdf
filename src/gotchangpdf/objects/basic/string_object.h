#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "buffer.h"

namespace gotchangpdf
{
	class StringObject : public Object
	{
	protected:
		Buffer _value;

		StringObject();
		explicit StringObject(const Buffer& value);
	};

	class HexadecimalString : public StringObject
	{
	public:
		explicit HexadecimalString(const Buffer& value);
		explicit HexadecimalString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::HexadecimalString; }

	private:
		std::string _hexadecimal;
	};

	class LiteralString : public StringObject
	{
	public:
		explicit LiteralString(const Buffer& value);
		explicit LiteralString(const lexical::Token& token);

		virtual inline Object::Type GetType(void) const override { return Object::Type::LiteralString; }
	};
}

#endif /* _STRING_OBJECT_H */
