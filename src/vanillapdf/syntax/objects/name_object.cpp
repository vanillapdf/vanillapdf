#include "precompiled.h"

#include "utils/character.h"
#include "utils/misc_utils.h"

#include "syntax/objects/name_object.h"
#include "utils/streams/output_stream_interface.h"

#include <fmt/core.h>

#include <sstream>

namespace vanillapdf {
namespace syntax {

NameObjectPtr NameObject::CreateFromEncoded(BufferPtr value) {
    auto str = value->ToStringView();
    return CreateFromEncoded(str);
}

NameObjectPtr NameObject::CreateFromEncoded(std::string_view value) {

    /*
    * Refactoring of ToInputStream and stoi is now string_view without any additional allocations.
    * Substrings to manage hexadecimal notation is not std::from_chars receiving pointers.
    * Using std::string instead of Buffer, further improving with small string optimization.
    * 
    * Before
    * ---------------------------------------------------------------------------------------------
    * Benchmark                                                   Time             CPU   Iterations
    * ---------------------------------------------------------------------------------------------
    * BM_CreateFromEncodedString_Name/string_empty             1166 ns         1172 ns       640000
    * BM_CreateFromEncodedString_Name/string_basic             2498 ns         2455 ns       280000
    * BM_CreateFromEncodedString_Name/string_hexadecimal       3231 ns         3223 ns       213333
    * 
    * After
    * ---------------------------------------------------------------------------------------------
    * Benchmark                                                   Time             CPU   Iterations
    * ---------------------------------------------------------------------------------------------
    * BM_CreateFromEncodedString_Name/string_empty              567 ns          572 ns      1120000
    * BM_CreateFromEncodedString_Name/string_basic              718 ns          715 ns       896000
    * BM_CreateFromEncodedString_Name/string_hexadecimal        652 ns          656 ns      1120000
    */

    std::string chars;

    auto buffer_size = value.size();
    for (decltype(buffer_size) i = 0; i < buffer_size; ++i) {
        auto current = value[i];

        if (!IsRegular(current)) {
            LOG_ERROR_AND_THROW_GENERAL("Unexpected character found in NameObject: {}", value);
        }

        if (current == '#') {
            if (i + 3 > buffer_size) {
                LOG_ERROR_AND_THROW_GENERAL("Could not parse hexadecimal character in NameObject: {}", value);
            }

            auto current_ptr = value.data() + i;
            auto hex_value = MiscUtils::FromChars<unsigned char>(current_ptr + 1, current_ptr + 3, 16);
            chars.push_back(hex_value);

            // If we would like to be strict, this flow should be part of the loop
            // and we should mark some kind of stage, that we have a hex char.
            // This is too complicated for this case, so let's do unfamous advancement of i.
            i = i + 2;

            continue;
        }

        chars.push_back(current);
    }

    return CreateFromDecoded(chars);

}

NameObjectPtr NameObject::CreateFromDecoded(BufferPtr value) {
    auto str = value->ToStringView();
    return CreateFromDecoded(str);
}

NameObjectPtr NameObject::CreateFromDecoded(std::string_view value) {
    NameObjectPtr result;

    result->SetValue(value);
    result->SetInitialized();

    return result;
}

size_t NameObject::Hash() const {
    return _value->Hash();
}

BufferPtr NameObject::GetValue() const {
    return _value;
}

void NameObject::SetValue(BufferPtr value) {
    auto str = value->ToStringView();
    SetValue(str);
}

void NameObject::SetValue(std::string_view value) {
    _value->assign(value.begin(), value.end());
    _value->SetInitialized();

    IncrementVersion();
}

bool NameObject::Equals(const NameObject& other) const {
    return _value->Equals(other._value);
}

bool NameObject::Equals(ObjectPtr other) const {
    if (!ObjectUtils::IsType<NameObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<NameObjectPtr>(other);
    return Equals(*other_obj);
}

Object::Type NameObject::GetObjectType(void) const noexcept {
    return Object::Type::Name;
}

void NameObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    output->Write("/");
    output->Write(ToString());
}

NameObject* NameObject::Clone(void) const {
    NameObjectPtr result(pdf_new NameObject(), false);
    result->SetValue(_value->Clone());

    CloneBaseProperties(result);
    return result.detach();
}

std::string NameObject::GetHexadecimalNotation(char ch) const {

    // stringstream
    // --------------------------------------------------------------------------------
    // Benchmark                                      Time             CPU   Iterations
    // --------------------------------------------------------------------------------
    // BM_NameObjectToPdf / string_empty             2370 ns         1664 ns       497778
    // BM_NameObjectToPdf / string_basic             4610 ns         3174 ns       172308
    // BM_NameObjectToPdf / string_hexadecimal      10948 ns         7394 ns       112000

    // fmtlib
    // --------------------------------------------------------------------------------
    // Benchmark                                      Time             CPU   Iterations
    // --------------------------------------------------------------------------------
    // BM_NameObjectToPdf / string_empty             2342 ns         1562 ns       560000
    // BM_NameObjectToPdf / string_basic             4518 ns         3537 ns       172308
    // BM_NameObjectToPdf / string_hexadecimal       5765 ns         5259 ns       154483

    return fmt::format("#{:02X}", ch);
}

std::string NameObject::ToString(void) const {
    std::stringstream ss;
    auto size = _value->size();
    for (decltype(size) i = 0; i < size; ++i) {
        auto current = _value[i];

        if ('#' == current) {
            ss << GetHexadecimalNotation(current);
            continue;
        }

        if (!IsRegular(current)) {
            ss << GetHexadecimalNotation(current);
            continue;
        }

        if (current < '!' || current > '~') {
            /* Regular characters that are outside the range
            EXCLAMATION MARK(21h) (!) to TILDE (7Eh) (~)
            should be written using the hexadecimal notation */

            ss << GetHexadecimalNotation(current);
            continue;
        }

        ss << current;
    }

    return ss.str();
}

} // syntax

bool operator==(const syntax::NameObject& left, const syntax::NameObject& right) {
    return (left.GetValue() == right.GetValue());
}

bool operator!=(const syntax::NameObject& left, const syntax::NameObject& right) {
    return (left.GetValue() != right.GetValue());
}

bool operator<(const syntax::NameObject& left, const syntax::NameObject& right) {
    return (left.GetValue() < right.GetValue());
}

bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
    return (*left == *right);
}

bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
    return (*left != *right);
}

bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
    return (*left < *right);
}

} // vanillapdf

namespace std {
size_t hash<vanillapdf::syntax::NameObject>::operator()(const vanillapdf::syntax::NameObject& name) const {
    return name.Hash();
}

} // std
