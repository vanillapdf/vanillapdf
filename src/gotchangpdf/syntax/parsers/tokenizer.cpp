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
		using std::_BADOFF;

		Tokenizer::Tokenizer(CharacterSource & stream)
			: Stream(stream),
			_last_token_offset(_BADOFF) {}

		Tokenizer::Tokenizer(const Tokenizer &other) : Stream(other) {}

		TokenPtr Tokenizer::ReadToken()
		{
			if (_last_token_offset == tellg())
			{
				assert(_BADOFF != _advance_position);
				assert(_BADOFF != _last_token_offset);

				auto result = _last_token;

				seekg(_advance_position);
				assert(good());

				_last_token_offset = _BADOFF;
				_advance_position = _BADOFF;

				return *result;
			}

			BufferPtr chars;

		retry:

			int ch = get();
			int ahead = peek();
			Token::Type result_type = Token::Type::UNKNOWN;

			switch (ch)
			{
			case EOF:
				result_type = Token::Type::END_OF_FILE;
				goto prepared;
			case WhiteSpace::LINE_FEED:
				chars->push_back(WhiteSpace::LINE_FEED);

				result_type = Token::Type::EOL;
				goto prepared;
			case WhiteSpace::SPACE:
				goto retry;
			case WhiteSpace::CARRIAGE_RETURN:
				chars->push_back(WhiteSpace::CARRIAGE_RETURN);
				if (ahead == WhiteSpace::LINE_FEED && ignore()) {
					chars->push_back(WhiteSpace::LINE_FEED);
				}

				result_type = Token::Type::EOL;
				goto prepared;
			case Delimiter::GREATER_THAN_SIGN:
				if (ahead == Delimiter::GREATER_THAN_SIGN && ignore()) {
					chars->push_back(Delimiter::GREATER_THAN_SIGN);
					chars->push_back(Delimiter::GREATER_THAN_SIGN);

					result_type = Token::Type::DICTIONARY_END;
					goto prepared;
				}

				throw GeneralException("Unexpected character at offset: " + std::to_string(tellg()));
			case Delimiter::LESS_THAN_SIGN:
				if (ahead == Delimiter::LESS_THAN_SIGN && ignore()) {
					chars->push_back(Delimiter::LESS_THAN_SIGN);
					chars->push_back(Delimiter::LESS_THAN_SIGN);

					result_type = Token::Type::DICTIONARY_BEGIN;
					goto prepared;
				}

				if (IsNumeric(ahead) || IsAlpha(ahead)) {
					for (;;) {
						auto current_meta = get();
						auto next_meta = peek();

						assert(current_meta != EOF && next_meta != EOF);
						if (current_meta == EOF || next_meta == EOF) {
							break;
						}

						auto current = SafeConvert<unsigned char>(current_meta);
						if (current == Delimiter::GREATER_THAN_SIGN) {
							break;
						}

						chars->push_back(current);
					}

					result_type = Token::Type::HEXADECIMAL_STRING;
					goto prepared;
				}

				throw GeneralException("Unexpected character at offset: " + std::to_string(tellg()));
			case Delimiter::LEFT_SQUARE_BRACKET:
				chars->push_back(Delimiter::LEFT_SQUARE_BRACKET);
				result_type = Token::Type::ARRAY_BEGIN;
				goto prepared;
			case Delimiter::RIGHT_SQUARE_BRACKET:
				chars->push_back(Delimiter::RIGHT_SQUARE_BRACKET);
				result_type = Token::Type::ARRAY_END;
				goto prepared;
			case Delimiter::SOLIDUS:
				while (IsRegular(peek())) {
					auto current = static_cast<char>(get());
					if (current == '#') {
						auto values = read(2);
						auto str = values->ToString();
						auto val = stoi(str, 0, 16);
						auto parsed = SafeConvert<unsigned char, int>(val);
						char converted = reinterpret_cast<char&>(parsed);
						chars->push_back(converted);
						continue;
					}

					chars->push_back(current);
				}

				result_type = Token::Type::NAME_OBJECT;
				goto prepared;
			case Delimiter::LEFT_PARENTHESIS:
				for (;;) {
					int current_meta = get();
					int next_meta = peek();

					assert(current_meta != EOF && next_meta != EOF);
					if (current_meta == EOF || next_meta == EOF) {
						break;
					}

					auto current = SafeConvert<unsigned char>(current_meta);
					auto next = SafeConvert<unsigned char>(next_meta);

					if (current == Delimiter::RIGHT_PARENTHESIS) {
						break;
					}

					if (current != '\\') {
						chars->push_back(current);
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
							if (EOF == numeric_meta) {
								break;
							}

							auto numeric = SafeConvert<unsigned char>(numeric_meta);
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
						auto converted = SafeConvert<unsigned char, int>(value);
						char char_converted = reinterpret_cast<char&>(converted);
						chars->push_back(char_converted);
						continue;
					}

					assert(!"Unrecognized escape sequence");
				}

				result_type = Token::Type::LITERAL_STRING;
				goto prepared;
			default:
				if (ch == 'R') {
					chars->push_back('R');

					result_type = Token::Type::INDIRECT_REFERENCE_MARKER;
					goto prepared;
				}

				auto current = SafeConvert<unsigned char>(ch);
				if (IsNumeric(current) || (current == '+') || (current == '-')) {
					chars->push_back(current);

					while (IsNumeric(peek())) {
						auto numeric = static_cast<char>(get());
						chars->push_back(numeric);
					}

					if (peek() == '.' && ignore()) {
						chars->push_back('.');
						while (IsNumeric(peek())) {
							auto next = static_cast<char>(get());
							chars->push_back(next);
						}

						result_type = Token::Type::REAL_OBJECT;
						goto prepared;
					}

					result_type = Token::Type::INTEGER_OBJECT;
					goto prepared;
				}

				chars->push_back(current);
				for (;;) {
					auto next_meta = peek();
					if (EOF == next_meta) {
						break;
					}

					auto next = SafeConvert<unsigned char>(next_meta);
					if (!IsRegular(next))
						break;

					chars->push_back(next); ignore();
				}

				goto prepared;
			}

		prepared:
			return TokenPtr(result_type, chars);
		}

		/* Peek need cache */
		TokenPtr Tokenizer::PeekToken()
		{
			if (_last_token_offset == tellg())
			{
				assert(_BADOFF != _advance_position);
				assert(_BADOFF != _last_token_offset);

				return *_last_token;
			}

			auto pos = tellg();
			_last_token = ReadToken();
			_advance_position = tellg();
			_last_token_offset = pos;

			seekg(_last_token_offset);
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
