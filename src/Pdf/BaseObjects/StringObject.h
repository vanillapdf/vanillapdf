#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "BaseObjects/Object.h"
#include "CharacterSet.h"

namespace Pdf
{
	namespace Lexical
	{
		class Token;
	}

	class StringObject : public Object
	{
	protected:
		CharacterSet _value;

		explicit StringObject(Type type);
		StringObject(Type type, const CharacterSet& value);
	};

	class HexadecimalString : public StringObject
	{
	public:
		explicit HexadecimalString(const CharacterSet& value);
		explicit HexadecimalString(const Lexical::Token& token);

	private:
		std::string _hexadecimal;

		friend void ::boost::intrusive_ptr_add_ref(HexadecimalString*);
		friend void ::boost::intrusive_ptr_release(HexadecimalString*);
	};

	class LiteralString : public StringObject
	{
	public:
		explicit LiteralString(const CharacterSet& value);
		explicit LiteralString(const Lexical::Token& token);

	private:
		friend void ::boost::intrusive_ptr_add_ref(LiteralString*);
		friend void ::boost::intrusive_ptr_release(LiteralString*);
	};
}

#endif /* _STRING_OBJECT_H */
