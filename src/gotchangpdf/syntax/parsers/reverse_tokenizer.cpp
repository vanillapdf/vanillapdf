#include "precompiled.h"

#include "reverse_tokenizer.h"
#include "exception.h"
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

		ReverseTokenizer::ReverseTokenizer(CharacterSource & stream)
			: Stream(stream),
			_last_token_offset(_BADOFF),
			_advance_position(_BADOFF)
		{
			_dictionary.Initialize();
		}

		ReverseTokenizer::ReverseTokenizer(const ReverseTokenizer &other)
			: Stream(other), _last_token_offset(other._last_token_offset),
			_advance_position(other._advance_position),
			_dictionary(other._dictionary) {}

		TokenPtr ReverseTokenizer::ReadToken()
		{
			auto current_offset = tellg();
			if (_token_cached && _last_token_offset == current_offset) {
				assert(_BADOFF != _last_token_offset);

				auto result = _last_token;

				seekg(_advance_position);
				_last_token_offset = _BADOFF;
				_advance_position = _BADOFF;
				_token_cached = false;

				return *result;
			}

			BufferPtr chars;

		retry:

			int ch = get();
			int ahead = peek();
			Token::Type result_type = Token::Type::UNKNOWN;

			switch (ch)
			{
			case std::char_traits<char>::eof():
				result_type = Token::Type::END_OF_INPUT;
				goto prepared;
			case WhiteSpace::LINE_FEED:
				chars->push_back(WhiteSpace::LINE_FEED);
				if (ahead == WhiteSpace::CARRIAGE_RETURN && ignore()) {
					chars->push_back(WhiteSpace::CARRIAGE_RETURN);
				}

				result_type = Token::Type::REVERSE_EOL;
				goto prepared;
			case WhiteSpace::SPACE:
			case WhiteSpace::FORM_FEED:
			case WhiteSpace::HORIZONTAL_TAB:
			case WhiteSpace::NUL:
				goto retry;
			case WhiteSpace::CARRIAGE_RETURN:
				chars->push_back(WhiteSpace::CARRIAGE_RETURN);

				result_type = Token::Type::REVERSE_EOL;
				goto prepared;
			default:
				auto current = SafeConvert<unsigned char>(ch);
				chars->push_back(current);

				if (IsNumeric(current)) {
					while (IsNumeric(peek())) {
						auto numeric = static_cast<unsigned char>(get());
						chars->push_back(numeric);
					}

					while ((peek() == '+') || (peek() == '-')) {
						auto next = static_cast<unsigned char>(get());
						chars->push_back(next);
					}

					result_type = Token::Type::REVERSE_INTEGER_OBJECT;
					goto prepared;
				}

				for (;;) {
					auto next_meta = peek();
					if (std::char_traits<char>::eof() == next_meta) {
						break;
					}

					auto next = SafeConvert<unsigned char>(next_meta);
					if (IsWhiteSpace(next))
						break;

					chars->push_back(next); ignore();
				}
			}

			assert(result_type == Token::Type::UNKNOWN);
			result_type = _dictionary.Find(chars);

		prepared:
			return TokenPtr(result_type, chars);
		}

		/* Peek need cache */
		TokenPtr ReverseTokenizer::PeekToken()
		{
			auto current = tellg();
			if (_token_cached && _last_token_offset == current) {
				assert(_BADOFF != _advance_position);
				assert(_BADOFF != _last_token_offset);

				return *_last_token;
			}

			_last_token = ReadToken();
			_advance_position = tellg();
			_last_token_offset = current;
			_token_cached = true;

			if (_BADOFF == _advance_position && _BADOFF == _last_token_offset) {
				assert(_last_token->GetType() == Token::Type::END_OF_INPUT);
			}

			if (_BADOFF == _advance_position && eof()) {
				clear();
			}

			seekg(_last_token_offset);
			assert(_last_token_offset == tellg());

			return *_last_token;
		}

		TokenPtr ReverseTokenizer::ReadTokenWithType(Token::Type type)
		{
			auto current_type = PeekTokenType();
			if (current_type != type)
				throw GeneralException("Unexpected token type");

			return ReadToken();
		}

		Token::Type ReverseTokenizer::PeekTokenType(void)
		{
			auto current = PeekToken();
			return current->GetType();
		}
	}
}
