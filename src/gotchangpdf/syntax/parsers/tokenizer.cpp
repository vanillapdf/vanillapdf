#include "precompiled.h"

#include "tokenizer.h"
#include "exception.h"
#include "constants.h"
#include "token_dictionary.h"
#include "character.h"
#include "util.h"

#include <cassert>
#include <algorithm>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		Tokenizer::Tokenizer(CharacterSource & stream)
			: Stream(stream),
			_last_token_offset(constant::BAD_OFFSET),
			_advance_position(constant::BAD_OFFSET)
		{
		}

		TokenPtr Tokenizer::ReadToken()
		{
			// When reading past EOF, failbit is set to true
			// We are aware, that this function might try to read past EOF
			// instead of checking for EOF before every get
			// we just clear the flags after we finish
			SCOPE_GUARD_CAPTURE_REFERENCES(if (eof() && fail()) clear(rdstate() & ~failbit));

			auto current_offset = GetPosition();
			if (_token_cached && _last_token_offset == current_offset) {
				auto result = _last_token;

				SetPosition(_advance_position);

				_last_token_offset = constant::BAD_OFFSET;
				_advance_position = constant::BAD_OFFSET;
				_token_cached = false;

				return *result;
			}

			BufferPtr chars;

		retry:

			int ch = get();
			int ahead = peek();

			switch (ch)
			{
			case WhiteSpace::SPACE:
			case WhiteSpace::FORM_FEED:
			case WhiteSpace::HORIZONTAL_TAB:
			case WhiteSpace::NUL:
				goto retry;
			case std::char_traits<char>::eof():
				return TokenPtr(Token::Type::END_OF_INPUT);
			case Delimiter::PERCENT_SIGN:
				for (;;) {
					auto current_meta = get();
					auto next_meta = peek();

					if (current_meta == std::char_traits<char>::eof() || next_meta == std::char_traits<char>::eof()) {
						break;
					}

					if (current_meta == '\r') {
						if (next_meta == '\n') {
							ignore();
						}

						break;
					}

					if (current_meta == '\n') {
						break;
					}
				}

				// We are currently silently ignoring all comments
				goto retry;
			case WhiteSpace::LINE_FEED:
				chars->push_back(WhiteSpace::LINE_FEED);

				return TokenPtr(Token::Type::EOL, chars);
			case WhiteSpace::CARRIAGE_RETURN:
				chars->push_back(WhiteSpace::CARRIAGE_RETURN);
				if (ahead == WhiteSpace::LINE_FEED && ignore()) {
					chars->push_back(WhiteSpace::LINE_FEED);
				}

				return TokenPtr(Token::Type::EOL, chars);
			case Delimiter::GREATER_THAN_SIGN:
				if (ahead == Delimiter::GREATER_THAN_SIGN && ignore()) {
					chars->push_back(Delimiter::GREATER_THAN_SIGN);
					chars->push_back(Delimiter::GREATER_THAN_SIGN);

					return TokenPtr(Token::Type::DICTIONARY_END, chars);
				}

				throw GeneralException("Unexpected character at offset: " + std::to_string(tellg()));
			case Delimiter::LESS_THAN_SIGN:
				if (ahead == Delimiter::LESS_THAN_SIGN && ignore()) {
					chars->push_back(Delimiter::LESS_THAN_SIGN);
					chars->push_back(Delimiter::LESS_THAN_SIGN);

					return TokenPtr(Token::Type::DICTIONARY_BEGIN, chars);
				}

				// empty hexadecimal string
				if (ahead == Delimiter::GREATER_THAN_SIGN && ignore()) {
					return TokenPtr(Token::Type::HEXADECIMAL_STRING, chars);
				}

				for (;;) {
					auto current_meta = get();
					auto next_meta = peek();

					assert(current_meta != std::char_traits<char>::eof() && next_meta != std::char_traits<char>::eof());
					if (current_meta == std::char_traits<char>::eof() || next_meta == std::char_traits<char>::eof()) {
						break;
					}

					auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);
					if (current == Delimiter::GREATER_THAN_SIGN) {
						break;
					}

					if (IsNumeric(current) || IsAlpha(current)) {
						chars->push_back(current);
						continue;
					}

					throw GeneralException("Unexpected character in hexadecimal string " + std::to_string(current));
				}

				return TokenPtr(Token::Type::HEXADECIMAL_STRING, chars);
			case Delimiter::LEFT_SQUARE_BRACKET:
				chars->push_back(Delimiter::LEFT_SQUARE_BRACKET);
				return TokenPtr(Token::Type::ARRAY_BEGIN, chars);
			case Delimiter::RIGHT_SQUARE_BRACKET:
				chars->push_back(Delimiter::RIGHT_SQUARE_BRACKET);
				return TokenPtr(Token::Type::ARRAY_END, chars);
			case Delimiter::SOLIDUS:
				while (IsRegular(peek())) {
					auto current = static_cast<char>(get());
					if (current == '#') {
						auto values = read(2);
						auto str = values->ToString();
						auto val = stoi(str, 0, 16);
						auto parsed = ValueConvertUtils::SafeConvert<unsigned char, int>(val);
						char converted = reinterpret_cast<char&>(parsed);
						chars->push_back(converted);
						continue;
					}

					chars->push_back(current);
				}

				return TokenPtr(Token::Type::NAME_OBJECT, chars);
			case Delimiter::LEFT_PARENTHESIS:
			{
				int nested_count = 0;
				for (;;) {
					int current_meta = get();
					int next = peek();

					if (current_meta == std::char_traits<char>::eof()) {
						throw GeneralException("Improperly terminated literal string sequence: " + chars->ToString());
					}

					auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);

					if (current == Delimiter::LEFT_PARENTHESIS) {
						nested_count++;
					}

					if (current == Delimiter::RIGHT_PARENTHESIS) {
						if (0 == nested_count) {
							break;
						}

						nested_count--;
						assert(nested_count >= 0);
						continue;
					}

					if (current == '\r') {
						if (next == '\n') {
							ignore();
						}

						chars->push_back(WhiteSpace::LINE_FEED);
						continue;
					}

					if (current != '\\') {
						chars->push_back(current);
						continue;
					}

					if (next == std::char_traits<char>::eof()) {
						continue;
					}

					// escaped characters
					if (next == 'r' && ignore()) {
						chars->push_back('\r');
						continue;
					}

					if (next == 'f' && ignore()) {
						chars->push_back('\f');
						continue;
					}

					if (next == 't' && ignore()) {
						chars->push_back('\t');
						continue;
					}

					if (next == 'n' && ignore()) {
						chars->push_back('\n');
						continue;
					}

					if (next == 'b' && ignore()) {
						chars->push_back('\b');
						continue;
					}

					if (next == '(' && ignore()) {
						chars->push_back('(');
						continue;
					}

					if (next == ')' && ignore()) {
						chars->push_back(')');
						continue;
					}

					if (next == '\\' && ignore()) {
						chars->push_back('\\');
						continue;
					}

					// Backslash at the EOL shall be disregarded
					if (next == '\r' && ignore()) {
						if (peek() == '\n') {
							ignore();
						}

						continue;
					}

					// Backslash at the EOL shall be disregarded
					if (next == '\n' && ignore()) {
						continue;
					}

					if (IsNumeric(next)) {
						std::stringstream octal;
						for (int i = 0; i < 3; ++i) {
							auto numeric_meta = peek();
							if (std::char_traits<char>::eof() == numeric_meta) {
								break;
							}

							auto numeric = ValueConvertUtils::SafeConvert<unsigned char>(numeric_meta);
							if (IsNumeric(numeric) && ignore()) {
								octal << numeric;
							}
							else {
								assert(!"Found invalid value in octal representation inside literal string");
								throw GeneralException("Expected octal value inside literal string: " + octal.str());
							}
						}

						int value = 0;
						octal >> std::oct >> value;
						auto converted = ValueConvertUtils::SafeConvert<unsigned char, int>(value);
						char char_converted = reinterpret_cast<char&>(converted);
						chars->push_back(char_converted);
						continue;
					}

					continue;
				}

				return TokenPtr(Token::Type::LITERAL_STRING, chars);
			}
			default:
				if (ch == 'R') {
					chars->push_back('R');

					return TokenPtr(Token::Type::INDIRECT_REFERENCE_MARKER, chars);
				}

				auto current = ValueConvertUtils::SafeConvert<unsigned char>(ch);
				bool has_dot = (current == '.');
				if (IsNumeric(current) || (current == '+') || (current == '-') || has_dot) {
					chars->push_back(current);
					while (IsNumeric(peek())) {
						auto numeric = static_cast<char>(get());
						chars->push_back(numeric);
					}

					if (has_dot) {
						return TokenPtr(Token::Type::REAL_OBJECT, chars);
					}

					if (peek() == '.' && ignore()) {
						chars->push_back('.');
						while (IsNumeric(peek())) {
							auto next = static_cast<char>(get());
							chars->push_back(next);
						}

						return TokenPtr(Token::Type::REAL_OBJECT, chars);
					}

					return TokenPtr(Token::Type::INTEGER_OBJECT, chars);
				}

				chars->push_back(current);
				for (;;) {
					auto next_meta = peek();
					if (std::char_traits<char>::eof() == next_meta) {
						break;
					}

					auto next = ValueConvertUtils::SafeConvert<unsigned char>(next_meta);
					if (!IsRegular(next))
						break;

					chars->push_back(next); ignore();
				}
			}

			auto result_type = _dictionary->Find(chars);
			return TokenPtr(result_type, chars);
		}

		/* Peek need cache */
		TokenPtr Tokenizer::PeekToken()
		{
			auto current = GetPosition();
			if (_token_cached && _last_token_offset == current) {
				return *_last_token;
			}

			_last_token = ReadToken();
			_advance_position = GetPosition();
			_last_token_offset = current;
			_token_cached = true;

			if (constant::BAD_OFFSET == _advance_position && constant::BAD_OFFSET == _last_token_offset) {
				assert(_last_token->GetType() == Token::Type::END_OF_INPUT);
			}

			SetPosition(_last_token_offset);
			return *_last_token;
		}

		TokenPtr Tokenizer::ReadTokenWithType(Token::Type type)
		{
			auto current_type = PeekTokenType();
			if (current_type != type)
				throw GeneralException("Unexpected token type");

			return ReadToken();
		}

		Token::Type Tokenizer::PeekTokenType(void)
		{
			auto current = PeekToken();
			return current->GetType();
		}
	}
}
