#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "token.h"
#include "object.h"
#include "containable.h"
#include "util.h"

namespace gotchangpdf
{
	class IntegerObject : public Containable, public Object
	{
	public:
		typedef types::integer value_type;

		IntegerObject() = default;
		explicit IntegerObject(value_type value);
		explicit IntegerObject(const lexical::Token& value);

		inline value_type Value(void) const _NOEXCEPT { return _value; }

		template <typename T>
		inline T SafeConvert(void) const
		{
			if (!IsInRange<value_type, T>(_value))
				throw exceptions::Exception("Integer value is out of range");

			return static_cast<T>(_value);
		}

		operator value_type() const { return _value; }

		IntegerObject& operator= (value_type value);
		IntegerObject& operator= (BufferPtr value);
		IntegerObject& operator= (const lexical::Token& value);

		virtual Object::Type GetType(void) const _NOEXCEPT override{ return Object::Type::Integer; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

		friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, IntegerObject& o);
		friend lexical::Stream& operator>> (lexical::Stream& s, IntegerObject& o);

		inline bool Equals(const IntegerObject& other) const _NOEXCEPT { return _value == other._value; }

		inline bool operator==(const IntegerObject& other) const { return Equals(other); }
		inline bool operator!=(const IntegerObject& other) const { return !Equals(other); }
		inline bool operator<(const IntegerObject& other) const { return _value < other._value; }

	//private:
	public:
		value_type _value = 0;

	private:
		ContainerPtr _container;
	};
}

#endif /* _INTEGER_OBJECT_H */
