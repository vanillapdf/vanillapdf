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
		LiteralStringObject::LiteralStringObject(BufferPtr value) : _raw_value(value) {}
		HexadecimalStringObject::HexadecimalStringObject(BufferPtr value) : _raw_value(value) {}
		StringObjectPtr::StringObjectPtr() : Deferred<StringObjectBase>(LiteralStringObjectPtr()) {}

		BufferPtr LiteralStringObject::Value() const
		{
			if (!_value.empty())
				return _value;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			if (locked_file->IsEncrypted())
				_value = locked_file->DecryptString(_raw_value, _obj_number, _gen_number);
			else
				_value = _raw_value;

			return _value;
		}

		BufferPtr HexadecimalStringObject::Value() const
		{
			if (!_value.empty())
				return _value;

			BufferPtr result;
			auto hexadecimal = _raw_value->ToString();
			unsigned int len = (hexadecimal.length() / 2);
			for (unsigned int i = 0; i < len; ++i) {
				int val = stoi(hexadecimal.substr(i * 2, 2), 0, 16);
				auto parsed = SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				result->push_back(converted);
			}

			// Last byte in unpaired
			if (len * 2 < hexadecimal.length()) {
				std::string pair{ hexadecimal[hexadecimal.length() - 1], 0 };
				int val = stoi(pair, 0, 16);
				auto parsed = SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				result->push_back(converted);
			}

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			if (locked_file->IsEncrypted()) {
				result = locked_file->DecryptString(result, _obj_number, _gen_number);
			}

			_value = result;
			return _value;
		}

		std::string HexadecimalStringObject::ToPdf() const
		{
			std::stringstream ss;
			ss << '<';

			auto size = _value->size();
			for (decltype(size) i = 0; i < size; ++i) {
				auto current = _value[i];
				int converted = static_cast<int>(current);
				ss << std::hex << converted;
			}

			ss << '>';
			return ss.str();
		}

		std::string LiteralStringObject::ToPdf() const
		{
			std::stringstream ss;
			ss << '(';

			auto size = _value->size();
			for (decltype(size) i = 0; i < size; ++i) {
				unsigned char current = _value[i];

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

			ss << ')';
			return ss.str();
		}
	}
}