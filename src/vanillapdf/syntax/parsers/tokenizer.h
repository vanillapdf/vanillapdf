#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "syntax/parsers/token.h"
#include "syntax/parsers/token_dictionary.h"

#include "utils/constants.h"
#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {
namespace syntax {

class Tokenizer {
public:
	explicit Tokenizer(IInputStreamPtr stream);

	TokenPtr ReadToken(void);
	TokenPtr PeekToken(void);
	TokenPtr ReadTokenWithType(Token::Type type);
	Token::Type PeekTokenType(void);

protected:
	std::unique_ptr<TokenDictionaryBase> _dictionary;
	IInputStreamPtr m_stream;

private:
	TokenPtr _last_token;
	types::stream_offset _last_token_offset, _advance_position;
	bool _token_cached = false;

	TokenPtr ReadComment(void);
	TokenPtr ReadHexadecimalString(void);
	TokenPtr ReadLiteralString(void);
	TokenPtr ReadName(void);
};

} // syntax
} // vanillapdf

#endif /* _TOKENIZER_H */
