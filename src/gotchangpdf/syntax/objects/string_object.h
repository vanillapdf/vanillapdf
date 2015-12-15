#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "syntax_fwd.h"
#include "containable.h"
#include "buffer.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class StringObjectBase : public ContainableObject
		{
		public:
			enum class StringType
			{
				Literal = 0,
				Hexadecimal
			};

			virtual BufferPtr Value() const = 0;

			virtual StringType GetStringType(void) const _NOEXCEPT = 0;
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::String; }

			bool Equals(const StringObjectBase& other) const { return Value() == other.Value();	}
			inline bool operator==(const StringObjectBase& other) const { return Equals(other); }
		};

		class HexadecimalStringObject : public StringObjectBase
		{
		public:
			HexadecimalStringObject() = default;
			explicit HexadecimalStringObject(BufferPtr value);

			virtual inline StringObjectBase::StringType GetStringType(void) const _NOEXCEPT override { return StringObjectBase::StringType::Hexadecimal; }

			virtual inline BufferPtr Value() const override { return _value; }
			virtual std::string ToString(void) const override { return _value->ToString(); }

			//private:
		public:
			BufferPtr _value;
		};

		class LiteralStringObject : public StringObjectBase
		{
		public:
			LiteralStringObject() = default;
			explicit LiteralStringObject(BufferPtr value);

			virtual inline StringObjectBase::StringType GetStringType(void) const _NOEXCEPT override { return StringObjectBase::StringType::Literal; }
			virtual inline BufferPtr Value() const override { return _value; }
			virtual std::string ToString(void) const override { return _value->ToString(); }

		public:
			BufferPtr _value;
		};
	}
}

#endif /* _STRING_OBJECT_H */
