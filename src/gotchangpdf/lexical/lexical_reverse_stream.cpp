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
		using namespace character;

		ReverseStream::ReverseStream(CharacterSource & stream)
			: raw::ReverseStream(stream), _last_token_offset(_BADOFF) {}

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
			char ch = get();
			char ahead = peek();
			Token::Type result_type = Token::Type::UNKNOWN;

			switch (ch)
			{
			case WhiteSpace::LINE_FEED:
				chars.push_back(ch);
				if (IsSpace(ahead) || Equals(ahead, WhiteSpace::CARRIAGE_RETURN))
					chars.push_back(get());

				result_type = Token::Type::EOL;
				goto prepared;
			case WhiteSpace::SPACE:
				goto retry;
			case Delimiter::GREATER_THAN_SIGN:
				if (Equals(ahead, Delimiter::GREATER_THAN_SIGN))
				{
					chars.push_back(ch);
					chars.push_back(get());

					result_type = Token::Type::DICTIONARY_END;
					goto prepared;
				}
				else
					throw Exception("Unexpected character follows intended dictionary end: " + ahead);
			case Delimiter::LESS_THAN_SIGN:
				if (Equals(ahead, Delimiter::LESS_THAN_SIGN))
				{
					// Little HACK >> twice
					chars.push_back(ch);
					chars.push_back(get());

					result_type = Token::Type::DICTIONARY_BEGIN;
					goto prepared;
				}
				else
				{
					chars.push_back(get());
					while (!Equals(peek(), Delimiter::GREATER_THAN_SIGN))
						chars.push_back(get());

					result_type = Token::Type::HEXADECIMAL_STRING;
					goto eat;
				}
			case Delimiter::LEFT_SQUARE_BRACKET:
				chars.push_back(ch);
				result_type = Token::Type::ARRAY_BEGIN;
				goto prepared;
			case Delimiter::RIGHT_SQUARE_BRACKET:
				chars.push_back(ch);
				result_type = Token::Type::ARRAY_END;
				goto prepared;
			case Delimiter::SOLIDUS:
				while (!(IsWhiteSpace(peek()) || IsDelimiter(peek())))
				{
					/* TODO #sign in name objects */
					chars.push_back(get());
				}

				result_type = Token::Type::NAME_OBJECT;
				goto prepared;
			case Delimiter::LEFT_PARENTHESIS:
				while (!Equals(peek(), Delimiter::RIGHT_PARENTHESIS))
					chars.push_back(get());

				result_type = Token::Type::LITERAL_STRING;
				goto eat;
			default:
				if (Equals(ch, 'R'))
				{
					chars.push_back(ch);

					result_type = Token::Type::INDIRECT_REFERENCE_MARKER;
					goto prepared;
				}

				if (IsNumeric(ch) || Equals(ch, '+') || Equals(ch, '-'))
				{
					chars.push_back(ch);

					while (IsNumeric(peek()))
						chars.push_back(get());

					if (Equals(peek(), '.'))
					{
						chars.push_back(get());
						while (IsNumeric(peek()))
							chars.push_back(get());

						result_type = Token::Type::REAL_OBJECT;
						goto prepared;
					}

					result_type = Token::Type::INTEGER_OBJECT;
					goto prepared;
				}

				chars.push_back(ch);
				while (!(IsWhiteSpace(peek()) || IsDelimiter(peek())))
					chars.push_back(get());


				while (!(IsRegular(peek()) || IsWhiteSpace(peek())))
					chars.push_back(get());

				goto prepared;
			}

		eat:
			get();

		prepared:
			chars.reverse();
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
