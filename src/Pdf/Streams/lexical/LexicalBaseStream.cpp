#include "LexicalBaseStream.h"
#include "Exceptions/Exception.h"

namespace Pdf
{
	namespace Streams
	{
		namespace Lexical
		{
			using namespace std;
			using Pdf::Lexical::Token;

			shared_ptr<Token> BaseStream::ReadTokenWithType(Token::Type type)
			{
				auto token = ReadToken();
				if (token->type() != type)
					throw Exception(string("Token type does not correspond to required type: ") + Pdf::Lexical::Token::GetTypeValueName(type));

				return token;
			}

			BaseStream::~BaseStream() {}

			Pdf::Lexical::Token::Type BaseStream::PeekTokenType()
			{
				// TODO optimize
				auto token = PeekToken();
				return token->type();
			}
		}
	}
}
