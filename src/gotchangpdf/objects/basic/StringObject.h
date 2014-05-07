#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "Fwd.h"
#include "Object.h"
#include "Buffer.h"

namespace Pdf
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
		explicit HexadecimalString(const Lexical::Token& token);

	private:
		std::string _hexadecimal;

		friend void ::boost::intrusive_ptr_add_ref(HexadecimalString*);
		friend void ::boost::intrusive_ptr_release(HexadecimalString*);
	};

	class LiteralString : public StringObject
	{
	public:
		explicit LiteralString(const Buffer& value);
		explicit LiteralString(const Lexical::Token& token);

	private:
		friend void ::boost::intrusive_ptr_add_ref(LiteralString*);
		friend void ::boost::intrusive_ptr_release(LiteralString*);
	};
}

#endif /* _STRING_OBJECT_H */
