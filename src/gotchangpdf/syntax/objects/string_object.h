#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "containable.h"
#include "buffer.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class StringObject : public Object
		{
		public:
			inline BufferPtr Value() const { return _value; }

		protected:
			StringObject();
			explicit StringObject(BufferPtr value);

			// private
		public:
			BufferPtr _value;
		};

		class HexadecimalStringObject : public StringObject
		{
		public:
			HexadecimalStringObject() = default;
			explicit HexadecimalStringObject(BufferPtr value);

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::HexadecimalString; }

			//private:
		public:
			std::string _hexadecimal;
		};

		class LiteralStringObject : public StringObject
		{
		public:
			LiteralStringObject() = default;
			explicit LiteralStringObject(BufferPtr value);

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::LiteralString; }
		};
	}
}

#endif /* _STRING_OBJECT_H */
