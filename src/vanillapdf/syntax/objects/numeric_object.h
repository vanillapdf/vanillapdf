#ifndef _NUMERIC_OBJECT_H
#define _NUMERIC_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include <string>

namespace vanillapdf {
namespace syntax {

class NumericObjectBackend : public virtual IUnknown, public IWeakReferenceable<NumericObjectBackend>, public IModifyObservable {
public:
	NumericObjectBackend();
	explicit NumericObjectBackend(int32_t value);
	explicit NumericObjectBackend(uint32_t value);
	explicit NumericObjectBackend(types::big_int value);
	explicit NumericObjectBackend(types::real value);
	explicit NumericObjectBackend(types::real value, uint32_t precision);
	NumericObjectBackend& operator= (types::big_int value);
	NumericObjectBackend& operator= (types::real value);

	types::big_int GetIntegerValue(void) const;
	types::big_uint GetUnsignedIntegerValue(void) const;
	types::real GetRealValue(void) const;

	void SetIntegerValue(types::big_int value);
	void SetUnsignedIntegerValue(types::big_uint value);
	void SetRealValue(types::real value);
	void SetRealPrecision(uint32_t precision);

	size_t Hash() const;
	NumericObjectBackend* Clone(void) const;

	std::string ToString(void) const;

private:
	std::string IntegerString(void) const;
	std::string UnsignedIntegerString(void) const;
	std::string RealString(void) const;

private:
	enum class Type {
		SignedInteger,
		UnsignedInteger,
		Real
	};

private:
	Type m_type = Type::SignedInteger;
	uint32_t m_precision = 0;

	union {
		types::big_int m_int;
		types::big_uint m_uint;
		types::real m_real;
	};
};

class NumericObject : public ContainableObject {
public:
	NumericObjectBackendPtr GetNumericBackend(void) const {
		return m_value;
	}

	virtual size_t Hash() const override;

protected:
	NumericObjectBackendPtr m_value;
};

} // syntax
} // vanillapdf

#endif /* _NUMERIC_OBJECT_H */
