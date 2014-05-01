#include "streams/lexical/LexicalStream.h"
#include "Exceptions/Exception.h"
#include "Constants.h"
#include "TokenDictionary.h"

#include <cassert>
#include <algorithm>

namespace Pdf
{
	namespace Streams
	{
		namespace Lexical
		{
			using namespace std;
			using Pdf::Lexical::Token;

			Stream::Stream(std::istream& stream) : Basic::Stream(stream), _last_token(nullptr), _last_token_offset(_BADOFF) {}

			Stream::Stream(const Pdf::Streams::Lexical::Stream &other) : Basic::Stream(other) {}

			/* We need to move to some serious lexer */

			shared_ptr<Token> Stream::ReadToken()
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

			retry:

				CharacterSet chars;
				Character ch = Get();
				Character ahead = Peek();
				Token::Type result_type = Token::Type::UNKNOWN;

				switch (ch)
				{
				case Character::WhiteSpace::LINE_FEED:
					chars.PushBack(ch);

					result_type = Token::Type::EOL;
					goto prepared;
				case Character::WhiteSpace::SPACE:
					if (Peek() == Character::WhiteSpace::LINE_FEED)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());
					}
					else
						goto retry;

					result_type = Token::Type::EOL;
					goto prepared;
				case Character::WhiteSpace::CARRIAGE_RETURN:
					if (ahead == Character::WhiteSpace::LINE_FEED)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());
					}
					else
						throw Exception("Unexpected character follows intended EOL after carriage return: " + ahead);

					result_type = Token::Type::EOL;
					goto prepared;
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

					goto prepared;
				}

			eat:
				Get();

			prepared:
				return shared_ptr<Token>(new Token(result_type, chars));
			}

			/* Peek need cache */
			shared_ptr<Token> Stream::PeekToken()
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

			Stream::~Stream() {}

			Pdf::Lexical::Token::Type Stream::PeekTokenType()
			{
				// TODO optimize
				auto token = PeekToken();
				return token->type();
			}
		}
	}
}
