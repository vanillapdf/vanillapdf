#include "precompiled.h"
#include "numeric_object.h"

#include <sstream>
#include <iomanip>

namespace gotchangpdf {
namespace syntax {

void NumericObjectBackend::SetIntegerValue(types::big_int value) {
	if (Type::SignedInteger == m_type && m_int == value) {
		return;
	}

	m_type = Type::SignedInteger;
	m_int = value;
	OnChanged();
}

void NumericObjectBackend::SetUnsignedIntegerValue(types::big_uint value) {
	if (Type::UnsignedInteger == m_type && m_uint == value) {
		return;
	}

	m_type = Type::UnsignedInteger;
	m_uint = value;
	OnChanged();
}
void NumericObjectBackend::SetRealValue(types::real value) {
	if (Type::Real == m_type && m_real == value) {
		return;
	}

	m_type = Type::Real;
	m_real = value;
	OnChanged();
}

types::big_int NumericObjectBackend::GetIntegerValue(void) const {
	if (m_type == Type::SignedInteger) {
		return m_int;
	}

	if (m_type == Type::UnsignedInteger) {
		return ValueConvertUtils::SafeConvert<types::big_int>(m_uint);
	}

	if (m_type == Type::Real) {
		return ValueConvertUtils::SafeConvert<types::big_int>(m_real);
	}

	assert(false && "Unknown numeric type");
	throw GeneralException("Unknown numeric type");
}

types::big_uint NumericObjectBackend::GetUnsignedIntegerValue(void) const {
	if (m_type == Type::UnsignedInteger) {
		return m_uint;
	}

	if (m_type == Type::SignedInteger) {
		return ValueConvertUtils::SafeConvert<types::big_uint>(m_int);
	}

	if (m_type == Type::Real) {
		auto integer = ValueConvertUtils::SafeConvert<types::big_int>(m_real);
		return ValueConvertUtils::SafeConvert<types::big_uint>(integer);
	}

	assert(false && "Unknown numeric type");
	throw GeneralException("Unknown numeric type");
}

types::real NumericObjectBackend::GetRealValue(void) const {
	if (m_type == Type::Real) {
		return m_real;
	}

	if (m_type == Type::SignedInteger) {
		return ValueConvertUtils::SafeConvert<types::real>(m_int);
	}

	if (m_type == Type::UnsignedInteger) {
		auto integer = ValueConvertUtils::SafeConvert<types::big_int>(m_uint);
		return ValueConvertUtils::SafeConvert<types::real>(integer);
	}

	assert(false && "Unknown numeric type");
	throw GeneralException("Unknown numeric type");
}

std::string NumericObjectBackend::IntegerString(void) const {
	return std::to_string(m_int);
}

std::string NumericObjectBackend::UnsignedIntegerString(void) const {
	return std::to_string(m_uint);
}

std::string NumericObjectBackend::RealString(void) const {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(m_precision) << m_real;
	return ss.str();
}

std::string NumericObjectBackend::ToString(void) const {
	if (Type::SignedInteger == m_type) {
		return IntegerString();
	}

	if (Type::UnsignedInteger == m_type) {
		return UnsignedIntegerString();
	}

	if (Type::Real == m_type) {
		return RealString();
	}

	assert(false && "Unknown numeric type");
	throw GeneralException("Unknown numeric type");
}

} // syntax
} // gotchangpdf
