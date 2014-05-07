#include "lexical_reverse_stream.h"
#include "Character.h"
#include "exception.h"
#include "integer_object.h"
#include "constants.h"

#include <cassert>
#include <functional>

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace std;
		using namespace exceptions;

		ReverseStream::ReverseStream(istream& stream) : basic::ReverseStream(stream) {}

		shared_ptr<Token> ReverseStream::ReadToken()
		{
			if (_last_token_offset == tellg())
			{
				assert(nullptr != _last_token);
				assert(_BADOFF != _advance_position);
				assert(_BADOFF != _last_token_offset);

				auto result = _last_token;

				seekg(_advance_position);
				assert(good());

				_last_token = nullptr;
				_last_token_offset = _BADOFF;
				_advance_position = _BADOFF;

				return result;
			}

			Buffer chars;

		retry:
			Character ch = Get();
			Character ahead = Peek();
			Token::Type result_type = Token::Type::UNKNOWN;

			switch (ch)
			{
			case Character::WhiteSpace::LINE_FEED:
				chars.Append(ch);
				if (ahead == Character::WhiteSpace::SPACE || ahead == Character::WhiteSpace::CARRIAGE_RETURN)
					chars.Append(Get());

				result_type = Token::Type::EOL;
				goto prepared;
			case Character::WhiteSpace::SPACE:
				goto retry;
			case Character::Delimiter::GREATER_THAN_SIGN:
				if (ahead == Character::Delimiter::GREATER_THAN_SIGN)
				{
					chars.Append(ch);
					chars.Append(Get());

					result_type = Token::Type::DICTIONARY_END;
					goto prepared;
				}
				else
					throw Exception("Unexpected character follows intended dictionary end: " + ahead);
			case Character::Delimiter::LESS_THAN_SIGN:
				if (ahead == Character::Delimiter::LESS_THAN_SIGN)
				{
					// Little HACK >> twice
					chars.Append(ch);
					chars.Append(Get());

					result_type = Token::Type::DICTIONARY_BEGIN;
					goto prepared;
				}
				else
				{
					chars.Append(Get());
					while (Peek() != Character::Delimiter::GREATER_THAN_SIGN)
						chars.Append(Get());

					result_type = Token::Type::HEXADECIMAL_STRING;
					goto eat;
				}
			case Character::Delimiter::LEFT_SQUARE_BRACKET:
				chars.Append(ch);
				result_type = Token::Type::ARRAY_BEGIN;
				goto prepared;
			case Character::Delimiter::RIGHT_SQUARE_BRACKET:
				chars.Append(ch);
				result_type = Token::Type::ARRAY_END;
				goto prepared;
			case Character::Delimiter::SOLIDUS:
				while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
				{
					/* TODO #sign in name objects */
					chars.Append(Get());
				}

				result_type = Token::Type::NAME_OBJECT;
				goto prepared;
			case Character::Delimiter::LEFT_PARENTHESIS:
				while (Peek() != Character::Delimiter::RIGHT_PARENTHESIS)
					chars.Append(Get());

				result_type = Token::Type::LITERAL_STRING;
				goto eat;
			default:
				if (ch == 'R')
				{
					chars.Append(ch);

					result_type = Token::Type::INDIRECT_REFERENCE_MARKER;
					goto prepared;
				}

				if (ch.isNumeric() || ch == '+' || ch == '-')
				{
					chars.Append(ch);

					while (Peek().isNumeric())
						chars.Append(Get());

					if (Peek() == '.')
					{
						chars.Append(Get());
						while (Peek().isNumeric())
							chars.Append(Get());

						result_type = Token::Type::REAL_OBJECT;
						goto prepared;
					}

					result_type = Token::Type::INTEGER_OBJECT;
					goto prepared;
				}

				chars.Append(ch);
				while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
					chars.Append(Get());


				while (!(Peek().isRegular() || Peek().isWhiteSpace()))
					chars.Append(Get());

				goto prepared;
			}

		eat:
			Get();

		prepared:
			chars.Reverse();
			return shared_ptr<Token>(new Token(result_type, chars));
		}

		ReverseStream::~ReverseStream() {}

		shared_ptr<Token> ReverseStream::PeekToken()
		{
			if (_last_token_offset == tellg())
			{
				assert(nullptr != _last_token);
				assert(_BADOFF != _advance_position);
				assert(_BADOFF != _last_token_offset);

				return _last_token;
			}

			auto pos = tellg();
			_last_token = ReadToken();
			_advance_position = tellg();
			_last_token_offset = pos;

			seekg(_last_token_offset);
			assert(good());

			return _last_token;
		}
	}
}
