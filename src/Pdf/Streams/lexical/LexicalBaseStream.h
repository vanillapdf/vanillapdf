#ifndef _LEXICAL_BASE_STREAM_H
#define _LEXICAL_BASE_STREAM_H

#include "BaseObjects/CharacterSet.h"
#include "Lexical/Token.h"

#include <memory>

namespace Pdf
{
	namespace Streams
	{		
		namespace Lexical
		{
			class BaseStream
			{
			public:
				virtual std::shared_ptr<Pdf::Lexical::Token> ReadToken() = 0;
				virtual std::shared_ptr<Pdf::Lexical::Token> PeekToken() = 0;
				virtual Pdf::Lexical::Token::Type PeekTokenType();
				virtual std::shared_ptr<Pdf::Lexical::Token> ReadTokenWithType(Pdf::Lexical::Token::Type type);
				virtual ~BaseStream() = 0;
			};
		}
	}
}

#endif /* _LEXICAL_BASE_STREAM_H */
