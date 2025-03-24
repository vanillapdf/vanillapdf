#include "precompiled.h"

#include "utils/character.h"

#include "syntax/objects/name_object.h"
#include "utils/streams/output_stream_interface.h"

#include <fmt/core.h>

#include <sstream>

namespace vanillapdf {
namespace syntax {

NameObject::NameObject() {
    _value->Subscribe(this);
}

NameObjectPtr NameObject::CreateFromEncoded(BufferPtr value) {

    auto input_stream = value->ToInputStream();

    BufferPtr chars;
    while (IsRegular(input_stream->Peek())) {
        auto current = static_cast<char>(input_stream->Get());
        if (current == '#') {
            auto values = input_stream->Read(2);
            auto str = values->ToString();
            auto val = std::stoi(str, 0, 16);
            auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
            char converted = reinterpret_cast<char&>(parsed);
            chars->push_back(converted);
            continue;
        }

        chars->push_back(current);
    }

    return CreateFromDecoded(chars);

}

NameObjectPtr NameObject::CreateFromEncoded(const char* value) {
    auto buffer = make_deferred_container<Buffer>(value);
    return CreateFromEncoded(buffer);
}

NameObjectPtr NameObject::CreateFromEncoded(const std::string& value) {
    auto buffer = make_deferred_container<Buffer>(value);
    return CreateFromEncoded(buffer);
}

NameObjectPtr NameObject::CreateFromDecoded(BufferPtr value) {
    NameObjectPtr result;

    result->SetValue(value);
    result->SetInitialized();

    return result;
}

NameObjectPtr NameObject::CreateFromDecoded(const char* value) {
    auto buffer = make_deferred_container<Buffer>(value);
    return CreateFromDecoded(buffer);
}

NameObjectPtr NameObject::CreateFromDecoded(const std::string& value) {
    auto buffer = make_deferred_container<Buffer>(value);
    return CreateFromDecoded(buffer);
}

size_t NameObject::Hash() const {
    return _value->Hash();
}

void NameObject::ObserveeChanged(const IModifyObservable*) {
    OnChanged();
}

BufferPtr NameObject::GetValue() const {
    return _value;
}

void NameObject::SetValue(BufferPtr value) {
    _value->assign(value.begin(), value.end());
    _value->SetInitialized();

    OnChanged();
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

NameObject::~NameObject() {
    _value->Unsubscribe(this);
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
