#include "precompiled.h"
#include "string_object.h"
#include "character.h"
#include "file.h"

#include <cassert>
#include <cctype>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		LiteralStringObject::LiteralStringObject() { _value->Subscribe(this); }
		HexadecimalStringObject::HexadecimalStringObject() { _value->Subscribe(this); }
		LiteralStringObject::LiteralStringObject(BufferPtr value) : _raw_value(value) { _value->Subscribe(this); }
		HexadecimalStringObject::HexadecimalStringObject(BufferPtr value) : _raw_value(value) { _value->Subscribe(this); }
		StringObjectPtr::StringObjectPtr() : Deferred<StringObjectBase>(LiteralStringObjectPtr()) {}

		BufferPtr LiteralStringObject::GetValue() const
		{
			if (!_value->empty())
				return _value;

			auto locked_file = m_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			BufferPtr new_value;
			if (!m_encryption_exempted && locked_file->IsEncrypted())
				new_value = locked_file->DecryptString(_raw_value, m_obj_number, m_gen_number);
			else
				new_value = _raw_value;

			_value->assign(new_value.begin(), new_value.end());
			_value->SetInitialized();
			return _value;
		}

		BufferPtr HexadecimalStringObject::GetValue() const
		{
			if (!_value->empty())
				return _value;

			BufferPtr result;
			auto hexadecimal = _raw_value->ToString();
			unsigned int len = (hexadecimal.length() / 2);
			for (unsigned int i = 0; i < len; ++i) {
				int val = stoi(hexadecimal.substr(i * 2, 2), 0, 16);
				auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				result->push_back(converted);
			}

			// Last byte in unpaired
			if (len * 2 < hexadecimal.length()) {
				std::string pair{ hexadecimal[hexadecimal.length() - 1], 0 };
				int val = stoi(pair, 0, 16);
				auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				result->push_back(converted);
			}

			auto locked_file = m_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			if (!m_encryption_exempted && locked_file->IsEncrypted()) {
				result = locked_file->DecryptString(result, m_obj_number, m_gen_number);
			}

			_value->assign(result.begin(), result.end());
			_value->SetInitialized();
			return _value;
		}

		std::string HexadecimalStringObject::ToPdf() const
		{
			std::stringstream ss;

			BufferPtr value = GetValue();
			auto size = value->size();
			for (decltype(size) i = 0; i < size; ++i) {
				auto current = value[i];

				// Only 2 byte hex representation
				int converted = static_cast<int>(current & 0xFF);
				ss << std::hex << std::setfill('0') <<std::setw(2) << converted;
			}

			auto locked_file = m_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			std::string str = ss.str();
			Buffer result = Buffer(str.begin(), str.end());
			if (locked_file->IsEncrypted()) {
				result = locked_file->EncryptString(result, m_obj_number, m_gen_number);
			}

			result.insert(result.begin(), '<');
			result.insert(result.end(), '>');
			return result.ToString();
		}

		std::string LiteralStringObject::ToPdf() const
		{
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

			auto locked_file = m_file.lock();
			if (!locked_file)
				throw FileDisposedException();

			std::string str = ss.str();
			Buffer result = Buffer(str.begin(), str.end());
			if (locked_file->IsEncrypted()) {
				result = locked_file->EncryptString(result, m_obj_number, m_gen_number);
			}

			result.insert(result.begin(), '(');
			result.insert(result.end(), ')');
			return result.ToString();
		}
	}
}