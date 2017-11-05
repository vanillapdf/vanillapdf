#ifndef _REVERSE_TOKENIZER_H
#define _REVERSE_TOKENIZER_H

#include "syntax/parsers/token.h"
#include "syntax/parsers/token_dictionary.h"

#include "utils/constants.h"
#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {
namespace syntax {

class ReverseTokenizer {
public:
	explicit ReverseTokenizer(IInputStreamPtr stream);

	TokenPtr ReadToken(void);
	TokenPtr PeekToken(void);
	TokenPtr ReadTokenWithType(Token::Type type);
	Token::Type PeekTokenType(void);

protected:
	IInputStreamPtr m_stream;

private:
	TokenPtr _last_token;
	types::stream_offset _last_token_offset, _advance_position;
	bool _token_cached = false;
	ReverseParserTokenDictionary _dictionary;

	TokenPtr ReadUnknown(int ch);
};

} // syntax
} // vanillapdf

#endif /* _REVERSE_TOKENIZER_H */
