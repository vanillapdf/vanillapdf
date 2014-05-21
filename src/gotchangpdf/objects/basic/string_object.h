#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "Buffer.h"

namespace gotchangpdf
{
	class StringObject : public Object
	{
	protected:
		Buffer _value;

		explicit StringObject(Type type);
		StringObject(Type type, const Buffer& value);
	};

	class HexadecimalString : public StringObject
	{
	public:
		explicit HexadecimalString(const Buffer& value);
		explicit HexadecimalString(const lexical::Token& token);

	private:
		std::string _hexadecimal;
	};

	class LiteralString : public StringObject
	{
	public:
		explicit LiteralString(const Buffer& value);
		explicit LiteralString(const lexical::Token& token);
	};
}

#endif /* _STRING_OBJECT_H */
