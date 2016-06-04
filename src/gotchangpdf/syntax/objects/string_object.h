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

			virtual BufferPtr GetValue() const = 0;
			virtual void SetValue(BufferPtr value) = 0;

			virtual StringType GetStringType(void) const noexcept = 0;
			virtual Object::Type GetType(void) const noexcept override { return Object::Type::String; }

			bool Equals(const StringObjectBase& other) const { return GetValue() == other.GetValue();	}
			bool operator==(const StringObjectBase& other) const { return Equals(other); }
		};

		class HexadecimalStringObject : public StringObjectBase
		{
		public:
			HexadecimalStringObject() = default;
			explicit HexadecimalStringObject(BufferPtr value);

			virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Hexadecimal; }

			virtual BufferPtr GetValue() const override;
			virtual void SetValue(BufferPtr value) override { _value = value; }
			virtual std::string ToPdf(void) const override;

			//private:
		public:
			BufferPtr _raw_value;
			mutable BufferPtr _value;
		};

		class LiteralStringObject : public StringObjectBase
		{
		public:
			LiteralStringObject() = default;
			explicit LiteralStringObject(BufferPtr value);

			virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Literal; }
			virtual BufferPtr GetValue() const override;
			virtual void SetValue(BufferPtr value) override { _value = value; }
			virtual std::string ToPdf(void) const override;

		public:
			BufferPtr _raw_value;
			mutable BufferPtr _value;
		};

		class StringObjectPtr : public Deferred<StringObjectBase>
		{
		public:
			using Deferred<StringObjectBase>::Deferred;

		public:
			StringObjectPtr();
		};
	}
}

#endif /* _STRING_OBJECT_H */
