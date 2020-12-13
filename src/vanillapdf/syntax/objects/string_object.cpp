#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/objects/string_object.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/character.h"

#include <cctype>
#include <cassert>
#include <iomanip>
#include <sstream>

namespace vanillapdf {
namespace syntax {

LiteralStringObject::LiteralStringObject() {
	_value->Subscribe(this);
}

HexadecimalStringObject::HexadecimalStringObject() {
	_value->Subscribe(this);
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
	result->SetValue(GetValue());

	CloneBaseProperties(result);
	return result.detach();
}

LiteralStringObject* LiteralStringObject::Clone(void) const {
	LiteralStringObjectPtr result(pdf_new LiteralStringObject(), false);
	result->SetValue(GetValue());

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

	BufferPtr new_value = _raw_value;
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
	_value->assign(value.begin(), value.end());
	_value->SetInitialized();
}

BufferPtr LiteralStringObject::GetRawValue() const {
	return _raw_value;
}

void LiteralStringObject::SetRawValue(BufferPtr value) {
	_raw_value->assign(value.begin(), value.end());
	_raw_value->SetInitialized();
}

BufferPtr HexadecimalStringObject::GetValue() const {
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
	_value->assign(value.begin(), value.end());
	_value->SetInitialized();
}

BufferPtr HexadecimalStringObject::GetRawValue() const {
	return _raw_value;
}

void HexadecimalStringObject::SetRawValue(BufferPtr value) {
	_raw_value->assign(value.begin(), value.end());
	_raw_value->SetInitialized();
}

void HexadecimalStringObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	std::stringstream ss;

	BufferPtr value = GetValue();
	auto size = value->size();
	for (decltype(size) i = 0; i < size; ++i) {
		auto current = value[i];

		// Only 2 byte hex representation
		int converted = static_cast<int>(current & 0xFF);
		ss << std::hex << std::setfill('0') << std::setw(2) << converted;
	}

	std::string str = ss.str();
	Buffer result = Buffer(str.begin(), str.end());

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();
		if (locked_file->IsEncrypted()) {
			result = locked_file->EncryptString(result, GetObjectNumber(), GetGenerationNumber());
		}
	}

	result.insert(result.begin(), '<');
	result.insert(result.end(), '>');

	output->Write(result);
}

void LiteralStringObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	std::stringstream ss;

	BufferPtr value = GetValue();
	auto size = value->size();
	for (decltype(size) i = 0; i < size; ++i) {
		unsigned char current = value[i];

		if (current == '\n') {
			ss << '\\' << 'n';
			continue;
		}

		if (current == '\r') {
			ss << '\\' << 'r';
			continue;
		}

		if (current == '\t') {
			ss << '\\' << 't';
			continue;
		}

		if (current == '\b') {
			ss << '\\' << 'b';
			continue;
		}

		if (current == '\f') {
			ss << '\\' << 'f';
			continue;
		}

		if (current == '(') {
			ss << '\\' << '(';
			continue;
		}

		if (current == ')') {
			ss << '\\' << ')';
			continue;
		}

		if (current == '\\') {
			ss << '\\' << '\\';
			continue;
		}

		if (!std::isprint(current)) {
			int converted = static_cast<int>(current);
			ss << '\\' << std::setfill('0') << std::setw(3) << std::oct << converted;
			continue;
		}

		ss << current;
	}

	std::string str = ss.str();
	Buffer result = Buffer(str.begin(), str.end());

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();
		if (locked_file->IsEncrypted()) {
			result = locked_file->EncryptString(result, GetObjectNumber(), GetGenerationNumber());
		}
	}

	result.insert(result.begin(), '(');
	result.insert(result.end(), ')');

	output->Write(result);
}

} // syntax
} // vanillapdf
