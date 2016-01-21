#ifndef _REVERSE_TOKENIZER_H
#define _REVERSE_TOKENIZER_H

#include "raw_stream.h"
#include "constants.h"
#include "token.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ReverseTokenizer : public Stream
		{
		public:
			explicit ReverseTokenizer(CharacterSource & s);
			ReverseTokenizer(const ReverseTokenizer & other);

			TokenPtr ReadToken(void);
			TokenPtr PeekToken(void);
			TokenPtr ReadTokenWithType(Token::Type type);
			Token::Type PeekTokenType(void);

		private:
			TokenPtr _last_token;
			types::stream_offset _last_token_offset, _advance_position;
			bool _token_cached = false;
		};
	}
}

#endif /* _REVERSE_TOKENIZER_H */
