#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/objects/string_object.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/character.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

namespace vanillapdf {
namespace syntax {

LiteralStringObject::LiteralStringObject() {
	_value->Subscribe(this);
	_access_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

HexadecimalStringObject::HexadecimalStringObject() {
	_value->Subscribe(this);
	_access_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

LiteralStringObjectPtr LiteralStringObject::CreateFromEncoded(BufferPtr value) {
	LiteralStringObjectPtr result;

	result->SetRawValue(value);
	return result;
}

LiteralStringObjectPtr LiteralStringObject::CreateFromEncoded(const char * value) {
	return CreateFromEncoded(make_deferred_container<Buffer>(value));
}

LiteralStringObjectPtr LiteralStringObject::CreateFromEncoded(const std::string& value) {
	return CreateFromEncoded(make_deferred_container<Buffer>(value));
}

LiteralStringObjectPtr LiteralStringObject::CreateFromDecoded(BufferPtr value) {
	LiteralStringObjectPtr result;

	result->SetValue(value);
	return result;
}

LiteralStringObjectPtr LiteralStringObject::CreateFromDecoded(const char * value) {
	return CreateFromDecoded(make_deferred_container<Buffer>(value));
}

LiteralStringObjectPtr LiteralStringObject::CreateFromDecoded(const std::string& value) {
	return CreateFromDecoded(make_deferred_container<Buffer>(value));
}

void LiteralStringObject::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromEncoded(BufferPtr value) {
	HexadecimalStringObjectPtr result;

	result->SetRawValue(value);
	return result;
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromEncoded(const char * value) {
	return CreateFromEncoded(make_deferred_container<Buffer>(value));
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromEncoded(const std::string& value) {
	return CreateFromEncoded(make_deferred_container<Buffer>(value));
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromDecoded(BufferPtr value) {
	HexadecimalStringObjectPtr result;

	result->SetValue(value);
	return result;
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromDecoded(const char * value) {
	return CreateFromDecoded(make_deferred_container<Buffer>(value));
}

HexadecimalStringObjectPtr HexadecimalStringObject::CreateFromDecoded(const std::string& value) {
	return CreateFromDecoded(make_deferred_container<Buffer>(value));
}

void HexadecimalStringObject::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

StringObjectPtr::StringObjectPtr() : Deferred<StringObjectBase>(LiteralStringObjectPtr()) {
}

HexadecimalStringObject::~HexadecimalStringObject() {
	_value->Unsubscribe(this);
}

LiteralStringObject::~LiteralStringObject() {
	_value->Unsubscribe(this);
}

size_t StringObjectBase::Hash() const {
	auto value = GetValue();
	return value->Hash();
}

HexadecimalStringObject* HexadecimalStringObject::Clone(void) const {
	HexadecimalStringObjectPtr result(pdf_new HexadecimalStringObject(), false);

	auto original_value = GetValue();
	auto original_raw_value = GetRawValue();

	result->SetValue(original_value);
	result->SetRawValue(original_raw_value);

	CloneBaseProperties(result);
	return result.detach();
}

LiteralStringObject* LiteralStringObject::Clone(void) const {
	LiteralStringObjectPtr result(pdf_new LiteralStringObject(), false);

	auto original_value = GetValue();
	auto original_raw_value = GetRawValue();

	result->SetValue(original_value);
	result->SetRawValue(original_raw_value);

	CloneBaseProperties(result);
	return result.detach();
}

bool StringObjectBase::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<StringObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<StringObjectPtr>(other);
	return Equals(*other_obj);
}

BufferPtr LiteralStringObject::GetValue() const {
	if (_value->IsInitialized()) {
		return _value;
	}

	ACCESS_LOCK_GUARD(_access_lock);

	if (_value->IsInitialized()) {
		return _value;
	}

	BufferPtr new_value = _raw_value;

	// TODO: Handle \r, \t, \f, etc.
	// TODO: Handle octal values

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();

		if (!IsEncryptionExempted() && locked_file->IsEncrypted()) {
			new_value = locked_file->DecryptString(_raw_value, GetRootObjectNumber(), GetRootGenerationNumber());
		}
	}

	_value->assign(new_value.begin(), new_value.end());
	_value->SetInitialized();
	return _value;
}

void LiteralStringObject::SetValue(BufferPtr value) {
	ACCESS_LOCK_GUARD(_access_lock);

	_value->assign(value.begin(), value.end());
	_value->SetInitialized();
}

BufferPtr LiteralStringObject::GetRawValue() const {
	return _raw_value;
}

void LiteralStringObject::SetRawValue(BufferPtr value) {
	ACCESS_LOCK_GUARD(_access_lock);

	_raw_value->assign(value.begin(), value.end());
	_raw_value->SetInitialized();
}

BufferPtr HexadecimalStringObject::GetValue() const {
	if (_value->IsInitialized()) {
		return _value;
	}

	ACCESS_LOCK_GUARD(_access_lock);

	if (_value->IsInitialized()) {
		return _value;
	}

	BufferPtr result;
	auto hexadecimal = _raw_value->ToString();
	auto len = (hexadecimal.length() / 2);
	for (decltype(len) i = 0; i < len; ++i) {
		int val = stoi(hexadecimal.substr(i * 2, 2), 0, 16);
		auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
		char converted = reinterpret_cast<char&>(parsed);
		result->push_back(converted);
	}

	// Last byte in unpaired
	if (len * 2 < hexadecimal.length()) {
		std::string pair { hexadecimal[hexadecimal.length() - 1], 0 };
		int val = stoi(pair, 0, 16);
		auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
		char converted = reinterpret_cast<char&>(parsed);
		result->push_back(converted);
	}

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();
		if (!IsEncryptionExempted() && locked_file->IsEncrypted()) {
			result = locked_file->DecryptString(result, GetRootObjectNumber(), GetRootGenerationNumber());
		}
	}

	_value->assign(result.begin(), result.end());
	_value->SetInitialized();
	return _value;
}

void HexadecimalStringObject::SetValue(BufferPtr value) {
	ACCESS_LOCK_GUARD(_access_lock);

	_value->assign(value.begin(), value.end());
	_value->SetInitialized();
}

BufferPtr HexadecimalStringObject::GetRawValue() const {
	return _raw_value;
}

void HexadecimalStringObject::SetRawValue(BufferPtr value) {
	ACCESS_LOCK_GUARD(_access_lock);

	_raw_value->assign(value.begin(), value.end());
	_raw_value->SetInitialized();
}

void HexadecimalStringObject::ToPdfStreamInternal(IOutputStreamPtr output) const {

	// Benchmark comparison

	// stringstream
	// ------------------------------------------------------------------------------------------
	// Benchmark                                                Time             CPU   Iterations
	// ------------------------------------------------------------------------------------------
	// BM_HexadecimalStringObjectToPdf / string_empty          2062 ns         1517 ns       896000
	// BM_HexadecimalStringObjectToPdf / string_unpaired       2625 ns         1904 ns       344615
	// BM_HexadecimalStringObjectToPdf / string_values         4064 ns         3128 ns       194783

	// fmtlib
	// BM_HexadecimalStringObjectToPdf / string_empty          1792 ns         1273 ns       896000
	// BM_HexadecimalStringObjectToPdf / string_unpaired       2094 ns         1569 ns       497778
	// BM_HexadecimalStringObjectToPdf / string_values         2600 ns         1957 ns       407273

	BufferPtr value = GetValue();

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();
		if (!IsEncryptionExempted() && locked_file->IsEncrypted()) {
			value = locked_file->EncryptString(value, GetRootObjectNumber(), GetRootGenerationNumber());
		}
	}

	auto result = fmt::format("<{:02X}>", fmt::join(value, ""));

	output->Write(result);
}

void LiteralStringObject::ToPdfStreamInternal(IOutputStreamPtr output) const {

	// stringstream
	// ---------------------------------------------------------------------------------- -
	// Benchmark                                         Time             CPU   Iterations
	// ---------------------------------------------------------------------------------- -
	// BM_LiteralStringObjectToPdf / string_empty       2076 ns         1256 ns       448000
	// BM_LiteralStringObjectToPdf / string_basic       2904 ns         2250 ns       298667
	// BM_LiteralStringObjectToPdf / string_octal       4855 ns         3578 ns       248889

	// fmtlib
	// ---------------------------------------------------------------------------------- -
	// Benchmark                                         Time             CPU   Iterations
	// ---------------------------------------------------------------------------------- -
	// BM_LiteralStringObjectToPdf / string_empty       2028 ns         1658 ns       735179
	// BM_LiteralStringObjectToPdf / string_basic       2955 ns         2023 ns       448000
	// BM_LiteralStringObjectToPdf / string_octal       3218 ns         2354 ns       497778

	BufferPtr value = GetValue();

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();
		if (!IsEncryptionExempted() && locked_file->IsEncrypted()) {
			value = locked_file->EncryptString(value, GetRootObjectNumber(), GetRootGenerationNumber());
		}
	}

	std::stringstream ss;
	ss << '(';

	auto size = value->size();
	for (decltype(size) i = 0; i < size; ++i) {
		unsigned char current = value[i];

		if (current == '\n') {
			ss << "\\n";
			continue;
		}

		if (current == '\r') {
			ss << "\\r";
			continue;
		}

		if (current == '\t') {
			ss << "\\t";
			continue;
		}

		if (current == '\b') {
			ss << "\\b";
			continue;
		}

		if (current == '\f') {
			ss << "\\f";
			continue;
		}

		if (current == '(') {
			ss << "\\(";
			continue;
		}

		if (current == ')') {
			ss << "\\)";
			continue;
		}

		if (current == '\\') {
			ss << "\\\\";
			continue;
		}

		if (!std::isprint(current)) {
			ss << fmt::format("\\{:03o}", current);
			continue;
		}

		ss << current;
	}

	ss << ')';

	auto result = ss.str();
	output->Write(result);
}

} // syntax
} // vanillapdf
