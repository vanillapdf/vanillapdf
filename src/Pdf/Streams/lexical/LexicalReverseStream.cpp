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

			unique_ptr<Token> ReverseStream::ReadToken()
			{
			retry:

				CharacterSet chars;
				Character ch = Get();
				Character ahead = Peek();

				switch (ch)
				{
				case Character::WhiteSpace::LINE_FEED:
					chars.PushBack(ch);
					return unique_ptr<Token>(new Token(Token::Type::EOL, chars));
				case Character::WhiteSpace::SPACE:
					if (Peek() == Character::WhiteSpace::LINE_FEED)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());
					}
					else
						goto retry;

					return unique_ptr<Token>(new Token(Token::Type::EOL, chars));
				case Character::WhiteSpace::CARRIAGE_RETURN:
					if (ahead == Character::WhiteSpace::LINE_FEED)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());
					}
					else
						throw Exception("Unexpected character follows intended EOL after carriage return: " + ahead);

					return unique_ptr<Token>(new Token(Token::Type::EOL, chars));
				case Character::Delimiter::GREATER_THAN_SIGN:
					if (ahead == Character::Delimiter::GREATER_THAN_SIGN)
					{
						chars.PushBack(ch);
						chars.PushBack(Get());

						return unique_ptr<Token>(new Token(Token::Type::DICTIONARY_END, chars));
					}
					else
						throw Exception("Unexpected character follows intended dictionary end: " + ahead);
				case Character::Delimiter::LESS_THAN_SIGN:
					if (ahead == Character::Delimiter::LESS_THAN_SIGN)
					{
						// Little HACK >> twice
						chars.PushBack(ch);
						chars.PushBack(Get());
						return unique_ptr<Token>(new Token(Token::Type::DICTIONARY_BEGIN, chars));
					}
					else
					{
						chars.PushBack(Get());
						while (Peek() != Character::Delimiter::GREATER_THAN_SIGN)
							chars.PushBack(Get());

						return unique_ptr<Token>(new Token(Token::Type::HEXADECIMAL_STRING, chars));
					}
				case Character::Delimiter::LEFT_SQUARE_BRACKET:
					chars.PushBack(ch);
					return unique_ptr<Token>(new Token(Token::Type::ARRAY_BEGIN, chars));
				case Character::Delimiter::RIGHT_SQUARE_BRACKET:
					chars.PushBack(ch);
					return unique_ptr<Token>(new Token(Token::Type::ARRAY_END, chars));
				case Character::Delimiter::SOLIDUS:
					while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
					{
						/* TODO #sign in name objects */
						chars.PushBack(Get());
					}

					return unique_ptr<Token>(new Token(Token::Type::NAME_OBJECT, chars));
				case Character::Delimiter::LEFT_PARENTHESIS:
					while (Peek() != Character::Delimiter::RIGHT_PARENTHESIS)
						chars.PushBack(Get());

					return unique_ptr<Token>(new Token(Token::Type::LITERAL_STRING, chars));
				default:
					if (ch == 'R')
					{
						chars.PushBack(ch);
						return unique_ptr<Token>(new Token(Token::Type::INDIRECT_REFERENCE_MARKER, chars));
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

							return unique_ptr<Token>(new Token(Token::Type::REAL_OBJECT, chars));
						}

						return unique_ptr<Token>(new Token(Token::Type::INTEGER_OBJECT, chars));
					}

					chars.PushBack(ch);
					while (!(Peek().isWhiteSpace() || Peek().isDelimiter()))
						chars.PushBack(Get());
				}

				return unique_ptr<Token>(new Token(chars));
			}

			ReverseStream::~ReverseStream() {}

			unique_ptr<Token> ReverseStream::PeekToken()
			{
				auto result = ReadToken();
				auto size = result->value().Size();
				for (int i = 0; i < size; ++i)
					Unget();

				return result;
			}
		}
	}
}
