#ifndef _REVERSE_TOKENIZER_H
#define _REVERSE_TOKENIZER_H

#include "syntax/streams/raw_stream.h"

#include "syntax/parsers/token.h"
#include "syntax/parsers/token_dictionary.h"

#include "utils/constants.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ReverseTokenizer : public Stream
		{
		public:
			explicit ReverseTokenizer(CharacterSource & s);

			TokenPtr ReadToken(void);
			TokenPtr PeekToken(void);
			TokenPtr ReadTokenWithType(Token::Type type);
			Token::Type PeekTokenType(void);

		private:
			TokenPtr _last_token;
			types::stream_offset _last_token_offset, _advance_position;
			bool _token_cached = false;
			ReverseParserTokenDictionary _dictionary;
		};
	}
}

#endif /* _REVERSE_TOKENIZER_H */
