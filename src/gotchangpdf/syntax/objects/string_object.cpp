#include "precompiled.h"
#include "string_object.h"
#include "character.h"

#include <cassert>
#include <cctype>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		LiteralStringObject::LiteralStringObject(BufferPtr value) : _value(value) {}

		HexadecimalStringObject::HexadecimalStringObject(BufferPtr value)
		{
			auto hexadecimal = value->ToString();

			unsigned int len = (hexadecimal.length() / 2);
			for (unsigned int i = 0; i < len; ++i) {
				int val = stoi(hexadecimal.substr(i * 2, 2), 0, 16);
				auto parsed = SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				_value->push_back(converted);
			}

			// Last byte in unpaired
			if (len * 2 < hexadecimal.length()) {
				std::string pair { hexadecimal[hexadecimal.length() - 1], 0 };
				int val = stoi(pair, 0, 16);
				auto parsed = SafeConvert<unsigned char, int>(val);
				char converted = reinterpret_cast<char&>(parsed);
				_value->push_back(converted);
			}
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
				auto current = _value[i];

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