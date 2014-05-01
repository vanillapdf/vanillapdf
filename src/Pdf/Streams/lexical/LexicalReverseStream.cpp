#include "streams/lexical/LexicalReverseStream.h"
#include "BaseObjects/Character.h"
#include "Exceptions/Exception.h"
#include "BaseObjects/IntegerObject.h"
#include "Constants.h"

#include <cassert>
#include <functional>

namespace Pdf
{
	namespace Streams
	{
		namespace Lexical
		{
			using namespace std;

			using Pdf::Lexical::Token;

			ReverseStream::ReverseStream(istream& stream) : Basic::ReverseStream(stream) {}

			shared_ptr<Token> ReverseStream::ReadToken()
			{
				if (_last_token_offset == tellg())
				{
					assert(nullptr != _last_token);
					assert(_BADOFF != _advance_position);
					assert(_BADOFF != _last_token_offset);

					auto result = _last_token;

					seekg(_advance_position);
					assert(!eof());
					assert(!fail());

					_last_token = nullptr;
					_last_token_offset = _BADOFF;
					_advance_position = _BADOFF;

					return result;
				}

				CharacterSet chars;

			retry:
				Character ch = Get();
				Character ahead = Peek();
				Token::Type result_type = Token::Type::UNKNOWN;

				switch (ch)
				{
				case Character::WhiteSpace::LINE_FEED:
					chars.PushBack(ch);
					if (ahead == Character::WhiteSpace::SPACE || ahead == Character::WhiteSpace::CARRIAGE_RETURN)
						chars.PushBack(Get());

					result_type = Token::Type::EOL;
					goto prepared;
				case Character::WhiteSpace::SPACE:
					goto retry;
				case Character::Delimiter::GREATER_THAN_SIGN:
					if (ahead == Character::Delimiter::GREATER_THAN_SIGN)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());

						result_type = Token::Type::DICTIONARY_END;
						goto prepared;
					}
					else
						throw Exception("Unexpected character follows intended dictionary end: " + ahead);
				case Character::Delimiter::LESS_THAN_SIGN:
					if (ahead == Character::Delimiter::LESS_THAN_SIGN)
					{
						// Little HACK >> twice
						chars.PushBack(ch);
						chars.PushBack(Get());

						result_type = Token::Type::DICTIONARY_BEGIN;
						goto prepared;
					}
					else
					{
						chars.PushBack(Get());
						while (Peek() != Character::Delimiter::GREATER_THAN_SIGN)
							chars.PushBack(Get());

						result_type = Token::Type::HEXADECIMAL_STRING;
						goto eat;
					}
				case Character::Delimiter::LEFT_SQUARE_BRACKET:
					chars.PushBack(ch);
					result_type = Token::Type::ARRAY_BEGIN;
					goto prepared;
				case Character::Delimiter::RIGHT_SQUARE_BRACKET:
					chars.PushBack(ch);
					result_type = Token::Type::ARRAY_END;
					goto prepared;
				case Character::Delimiter::SOLIDUS:
					while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
					{
						/* TODO #sign in name objects */
						chars.PushBack(Get());
					}

					result_type = Token::Type::NAME_OBJECT;
					goto prepared;
				case Character::Delimiter::LEFT_PARENTHESIS:
					while (Peek() != Character::Delimiter::RIGHT_PARENTHESIS)
						chars.PushBack(Get());

					result_type = Token::Type::LITERAL_STRING;
					goto eat;
				default:
					if (ch == 'R')
					{
						chars.PushBack(ch);

						result_type = Token::Type::INDIRECT_REFERENCE_MARKER;
						goto prepared;
					}

					if (ch.isNumeric() || ch == '+' || ch == '-')
					{
						chars.PushBack(ch);

						while (Peek().isNumeric())
							chars.PushBack(Get());

						if (Peek() == '.')
						{
							chars.PushBack(Get());
							while (Peek().isNumeric())
								chars.PushBack(Get());

							result_type = Token::Type::REAL_OBJECT;
							goto prepared;
						}

						result_type = Token::Type::INTEGER_OBJECT;
						goto prepared;
					}

					chars.PushBack(ch);
					while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
						chars.PushBack(Get());


					while (!(Peek().isRegular() || Peek().isWhiteSpace()))
						chars.PushBack(Get());

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
				assert(!eof());
				assert(!fail());

				return _last_token;
			}
		}
	}
}
