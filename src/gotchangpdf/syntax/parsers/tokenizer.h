#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "raw_stream.h"
#include "constants.h"
#include "token.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Tokenizer : public Stream
		{
		public:
			explicit Tokenizer(CharacterSource & s);
			Tokenizer(const Tokenizer & other);

			TokenPtr ReadToken(void);
			TokenPtr PeekToken(void);
			TokenPtr ReadTokenWithType(Token::Type type);
			Token::Type PeekTokenType(void);

			virtual ~Tokenizer();

		private:
			TokenPtr _last_token;
			types::stream_offset _last_token_offset, _advance_position;
		};
	}
}

#endif /* _TOKENIZER_H */
