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

			char ch = get();
			char ahead = peek();
			Token::Type result_type = Token::Type::UNKNOWN;

			switch (ch)
			{
			case WhiteSpace::LINE_FEED:
				chars->push_back(ch);

				result_type = Token::Type::EOL;
				goto prepared;
			case WhiteSpace::SPACE:
				goto retry;
			case WhiteSpace::CARRIAGE_RETURN:
				chars->push_back(ch);
				if (ahead == WhiteSpace::LINE_FEED)
					chars->push_back(get());

				result_type = Token::Type::EOL;
				goto prepared;
			case Delimiter::GREATER_THAN_SIGN:
				if (ahead == Delimiter::GREATER_THAN_SIGN)
				{
					chars->push_back(ch);
					chars->push_back(get());

					result_type = Token::Type::DICTIONARY_END;
					goto prepared;
				}
				else
					throw GeneralException("Unexpected character follows intended dictionary end: " + ahead);
			case Delimiter::LESS_THAN_SIGN:
				if (ahead == Delimiter::LESS_THAN_SIGN)
				{
					// Little HACK >> twice
					chars->push_back(ch);
					chars->push_back(get());

					result_type = Token::Type::DICTIONARY_BEGIN;
					goto prepared;
				}
				else
				{
					chars->push_back(get());
					while (peek() != Delimiter::GREATER_THAN_SIGN)
						chars->push_back(get());

					result_type = Token::Type::HEXADECIMAL_STRING;
					goto eat;
				}
			case Delimiter::LEFT_SQUARE_BRACKET:
				chars->push_back(ch);
				result_type = Token::Type::ARRAY_BEGIN;
				goto prepared;
			case Delimiter::RIGHT_SQUARE_BRACKET:
				chars->push_back(ch);
				result_type = Token::Type::ARRAY_END;
				goto prepared;
			case Delimiter::SOLIDUS:
				while (!(IsWhiteSpace(peek()) || IsDelimiter(peek())))
				{
					if (peek() == '#')
					{
						char sign = get();
						assert(sign == '#');

						// TODO check hex flags, clear if persists
						unsigned int value;
						*this >> std::hex >> value;

						/*
						char sign = get();
						char hinib = get_hex();
						char lonib = get_hex();

						char val = (hinib << 4) + lonib;
						*/
						chars->push_back(value & 0xFF);
					}
					else
					{
						chars->push_back(get());
					}
				}

				result_type = Token::Type::NAME_OBJECT;
				goto prepared;
			case Delimiter::LEFT_PARENTHESIS:
				for (;;) {
					char current = get();
					char next = peek();

					if (current == Delimiter::RIGHT_PARENTHESIS) {
						break;
					}

					if (current != '\\') {
						chars->push_back(current);
						continue;
					}

					// escaped characters
					if (next == 'r') {
						char consume = get();
						chars->push_back('\r');
						continue;
					}

					if (next == 'f') {
						char consume = get();
						chars->push_back('\f');
						continue;
					}

					if (next == 't') {
						char consume = get();
						chars->push_back('\t');
						continue;
					}

					if (next == 'n') {
						char consume = get();
						chars->push_back('\n');
						continue;
					}

					if (next == 'b') {
						char consume = get();
						chars->push_back('\b');
						continue;
					}

					if (next == '(') {
						char consume = get();
						chars->push_back('(');
						continue;
					}

					if (next == ')') {
						char consume = get();
						chars->push_back(')');
						continue;
					}

					if (next == '\\') {
						char consume = get();
						chars->push_back('\\');
						continue;
					}

					// Backslash at the EOL shall be disregarded
					if (next == '\r') {
						char consume = get();
						if (peek() == '\n') {
							char consume_another = get();
						}

						continue;
					}

					// Backslash at the EOL shall be disregarded
					if (next == '\n') {
						char consume = get();
						continue;
					}

					if (IsNumeric(next)) {
						std::stringstream octal;
						for (int i = 0; i < 3; ++i) {
							if (IsNumeric(peek())) {
								char current = get();
								octal << current;
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
				if (ch == 'R')
				{
					chars->push_back(ch);

					result_type = Token::Type::INDIRECT_REFERENCE_MARKER;
					goto prepared;
				}

				if (IsNumeric(ch) || (ch == '+') || (ch == '-'))
				{
					chars->push_back(ch);

					while (IsNumeric(peek()))
						chars->push_back(get());

					if (peek() == '.') {
						chars->push_back(get());
						while (IsNumeric(peek()))
							chars->push_back(get());

						result_type = Token::Type::REAL_OBJECT;
						goto prepared;
					}

					result_type = Token::Type::INTEGER_OBJECT;
					goto prepared;
				}

				chars->push_back(ch);
				while (!(IsWhiteSpace(peek()) || IsDelimiter(peek())))
					chars->push_back(get());

				goto prepared;
			}

		eat:
			get();

		prepared:
			//auto settings = lexical::BaseStream::LexicalSettingsGet();
			//if (std::find(settings->skip.begin(), settings->skip.end(), result_type) != settings->skip.end())
			//	// TODO handle recursive call using goto probably
			//	return ReadToken();

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
			assert(good());

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
