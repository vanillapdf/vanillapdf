#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/objects/string_object.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/character.h"
#include "utils/streams/stream_utils.h"

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

	// Handles backslashes and replaces with actual binary values
	BufferPtr new_value = GetRawValueDecoded();

	if (!m_file.IsEmpty()) {
		if (!m_file.IsActive()) {
			throw FileDisposedException();
		}

		auto locked_file = m_file.GetReference();

		if (!IsEncryptionExempted() && locked_file->IsEncrypted()) {
			new_value = locked_file->DecryptString(new_value, GetRootObjectNumber(), GetRootGenerationNumber());
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

BufferPtr LiteralStringObject::GetRawValueDecoded() const {

	BufferPtr result;

	// I am using stream conversion as the algorithm was written using streams and not buffers.
	// Even though it is not very efficient, I am confident that there will be one brave soul,
	// that will overcome this and do the refactoring with benchmark included, so that we can
	// sleep peacefully knowing the complexity of this algorithm is not quadratically asymptotic.
	auto raw_value_stream = StreamUtils::InputStreamFromBuffer(_raw_value);

	// Keep track of unescaped parenthesis as they have to be balanced
	int nested_count = 0;

	for (;;) {
		assert(nested_count >= 0);

		if (raw_value_stream->Eof()) {
			break;
		}

		auto eof_test = raw_value_stream->Peek();
		if (eof_test == std::char_traits<char>::eof()) {
			break;
		}

		int current_meta = raw_value_stream->Get();
		auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);

		if (current == Delimiter::LEFT_PARENTHESIS) {
			nested_count++;

			// TODO:
			// Wrapping parentheses are currently not included in the raw data

			// Do not include initial parenthesis in the result
			//if (nested_count == 1) {
			//	continue;
			//}
		}

		if (current == Delimiter::RIGHT_PARENTHESIS) {

			// TODO:
			// Wrapping parentheses are currently not included in the raw data

			// Decrement the nested counter on the right parenthesis
			nested_count--;

			// Terminate in case we would underflow the nested count
			if (nested_count < 0) {
				spdlog::warn("Literal string parsing would underflow the parenthesis count: {}", _raw_value->ToString());
				break;
			}
		}

		if (current == '\r') {
			result->push_back('\r');

			auto line_feed = raw_value_stream->Peek();
			if (line_feed == '\n' && raw_value_stream->Ignore()) {
				result->push_back('\n');
			}

			continue;
		}

		if (current != '\\') {
			result->push_back(current);
			continue;
		}

		auto next = raw_value_stream->Peek();
		if (next == std::char_traits<char>::eof()) {
			break;
		}

		// escaped characters
		if (next == 'r' && raw_value_stream->Ignore()) {
			result->push_back('\r');
			continue;
		}

		if (next == 'f' && raw_value_stream->Ignore()) {
			result->push_back('\f');
			continue;
		}

		if (next == 't' && raw_value_stream->Ignore()) {
			result->push_back('\t');
			continue;
		}

		if (next == 'n' && raw_value_stream->Ignore()) {
			result->push_back('\n');
			continue;
		}

		if (next == 'b' && raw_value_stream->Ignore()) {
			result->push_back('\b');
			continue;
		}

		if (next == '(' && raw_value_stream->Ignore()) {
			result->push_back('(');
			continue;
		}

		if (next == ')' && raw_value_stream->Ignore()) {
			result->push_back(')');
			continue;
		}

		if (next == '\\' && raw_value_stream->Ignore()) {
			result->push_back('\\');
			continue;
		}

		// Backslash at the EOL shall be disregarded
		if (next == '\r' && raw_value_stream->Ignore()) {
			if (raw_value_stream->Peek() == '\n') {
				raw_value_stream->Ignore();
			}

			continue;
		}

		// Backslash at the EOL shall be disregarded
		if (next == '\n' && raw_value_stream->Ignore()) {
			continue;
		}

		if (!IsNumeric(next)) {
			continue;
		}

		std::string octal;
		for (int i = 0; i < 3; ++i) {

			auto numeric_meta = raw_value_stream->Peek();
			if (numeric_meta == std::char_traits<char>::eof()) {
				break;
			}

			auto numeric = ValueConvertUtils::SafeConvert<unsigned char>(numeric_meta);
			if (IsNumeric(numeric) && raw_value_stream->Ignore()) {
				octal.push_back(numeric);
				continue;
			}

			// Update 24.10.2024

			// Until now I have not seen a document actually using only two digits specifying the octal value.
			// The specification says \ddd strictly without any other options, however:
			// "whereas both (\053) and (\53) denote strings containing the single character \053, a plus sign (+)"
			// This means that even less characters can be used for octal representation.

			// In case there are less than 3 numbers specifying the octal value we can break the loop
			break;
		}

		// ----------------------------------------------------------------------------------------
		// Benchmark                                              Time             CPU   Iterations
		// ----------------------------------------------------------------------------------------
		// BM_StringGetValue_Literal/stringstream_octal        8462 ns         4604 ns       149333
		// BM_StringGetValue_Literal/stoi_octal                3252 ns         1590 ns       560000

		// The value can actually overflow 255, as the maximum 3-digit octal value is 777.
		// This is 511 in decimal and it's not clear what should be done in such case.

		int value = stoi(octal, 0, 8);
		auto converted = ValueConvertUtils::SafeConvert<unsigned char, int>(value);
		char char_converted = reinterpret_cast<char&>(converted);
		result->push_back(char_converted);
		continue;
	}

	if (nested_count != 0) {
		throw GeneralException("Improperly terminated literal string sequence: " + result->ToString());
	}

	return result;
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

	auto hexadecimal = _raw_value->ToString();

	// Pad missing character with zero
	if (hexadecimal.length() % 2 == 1) {
		hexadecimal.push_back('0');
	}

	// Measure len after the padding has been done
	auto len = (hexadecimal.length() / 2);

	// Based on len we know exact capacity of the output,
	// which can save a few reallocations for longer strings

	// ----------------------------------------------------------------------------------------
	// Benchmark                                              Time             CPU   Iterations
	// ----------------------------------------------------------------------------------------
	// BM_StringGetValue_Hexadecimal/reserve_no            1533 ns          879 ns       746667
	// BM_StringGetValue_Hexadecimal/reserve_yes           1327 ns          808 ns      1102769

	BufferPtr result;
	result->reserve(len);

	for (decltype(len) i = 0; i < len; ++i) {
		int val = stoi(hexadecimal.substr(i * 2, 2), 0, 16);
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
