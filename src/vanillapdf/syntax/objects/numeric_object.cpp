#include "precompiled.h"
#include "numeric_object.h"

#include <fmt/core.h>

namespace vanillapdf {
namespace syntax {

NumericObjectBackend::NumericObjectBackend()
    : m_type(Type::SignedInteger), m_int(0) {
}

NumericObjectBackend::NumericObjectBackend(int32_t value)
    : m_type(Type::SignedInteger), m_int(value) {
}

NumericObjectBackend::NumericObjectBackend(uint32_t value)
    : m_type(Type::UnsignedInteger), m_uint(value) {
}

NumericObjectBackend::NumericObjectBackend(types::big_int value)
    : m_type(Type::SignedInteger), m_int(value) {
}
NumericObjectBackend::NumericObjectBackend(types::real value)
    : m_type(Type::Real), m_real(value) {
}

NumericObjectBackend::NumericObjectBackend(types::real value, uint32_t precision)
    : m_type(Type::Real), m_real(value), m_precision(precision) {
}

NumericObjectBackend& NumericObjectBackend::operator= (types::big_int value) {
    m_type = Type::SignedInteger;
    m_int = value;
    return *this;
}

NumericObjectBackend& NumericObjectBackend::operator= (types::real value) {
    m_type = Type::Real;
    m_real = value;
    return *this;
}

void NumericObjectBackend::SetRealPrecision(uint32_t precision) {
    m_precision = precision;
}

NumericObjectBackend* NumericObjectBackend::Clone(void) const {
    return pdf_new NumericObjectBackend(*this);
}

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

void NumericObjectBackend::ToggleBit(int pos, bool value) {
    if (m_type == Type::SignedInteger) {
        decltype(m_int) newbit = 1;

        if (value) {
            m_int |= newbit << pos;
        } else {
            m_int &= ~(newbit << pos);
        }

        OnChanged();
        return;
    }

    if (m_type == Type::UnsignedInteger) {
        decltype(m_uint) newbit = 1;

        if (value) {
            m_uint |= newbit << pos;
        } else {
            m_uint &= ~(newbit << pos);
        }

        OnChanged();
        return;
    }

    if (m_type == Type::Real) {
        throw GeneralException("Cannot toggle bits on floating point numbers");
    }

    assert(false && "Unknown numeric type");
    throw GeneralException("Unknown numeric type");
}

void NumericObjectBackend::SetBit(int pos) {
    ToggleBit(pos, true);
}

void NumericObjectBackend::ClearBit(int pos) {
    ToggleBit(pos, false);
}

bool NumericObjectBackend::IsBitSet(int pos) const {
    if (m_type == Type::SignedInteger) {
        decltype(m_int) newbit = 1;
        return ((m_int >> pos) & newbit) != 0;
    }

    if (m_type == Type::UnsignedInteger) {
        decltype(m_uint) newbit = 1;
        return ((m_uint >> pos) & newbit) != 0;
    }

    if (m_type == Type::Real) {
        throw GeneralException("Cannot read bits on floating point numbers");
    }

    assert(false && "Unknown numeric type");
    throw GeneralException("Unknown numeric type");
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

    // Benchmarking the difference os stringstream vs fmtlib

    // Run on(16 X 3792 MHz CPU s)
    // CPU Caches :
    //  L1 Data 32 KiB(x8)
    //  L1 Instruction 32 KiB(x8)
    //  L2 Unified 256 KiB(x8)
    //  L3 Unified 16384 KiB(x1)

    //fmtlib
    //BM_RealObjectToPdf / 752 / 0       1298 ns          977 ns      1120000
    //BM_RealObjectToPdf / 752 / 2       1243 ns          816 ns       746667

    //stringstream
    //BM_RealObjectToPdf / 752 / 0       2749 ns         1660 ns       404645
    //BM_RealObjectToPdf / 752 / 2       2793 ns         1967 ns       373333

    if (m_precision > 0) {
        return fmt::format("{:.{p}f}", m_real, fmt::arg("p", m_precision));
    }

    return fmt::to_string(m_real);
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

size_t NumericObjectBackend::Hash() const {
    if (m_type == Type::Real) {
        std::hash<decltype(m_real)> hasher;
        return hasher(m_real);
    }

    if (m_type == Type::SignedInteger) {
        std::hash<decltype(m_int)> hasher;
        return hasher(m_int);
    }

    if (m_type == Type::UnsignedInteger) {
        std::hash<decltype(m_uint)> hasher;
        return hasher(m_uint);
    }

    assert(false && "Unknown numeric type");
    throw GeneralException("Unknown numeric type");
}

size_t NumericObject::Hash() const {
    return m_value->Hash();
}

NumericObject::~NumericObject(){
    m_value->Unsubscribe(this);
}

} // syntax
} // vanillapdf
