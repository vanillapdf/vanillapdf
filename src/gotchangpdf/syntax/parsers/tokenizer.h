#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "syntax/streams/raw_stream.h"

#include "syntax/parsers/token.h"
#include "syntax/parsers/token_dictionary.h"

#include "utils/constants.h"

namespace gotchangpdf {
namespace syntax {

class Tokenizer : public Stream {
public:
	explicit Tokenizer(CharacterSource & s);

	TokenPtr ReadToken(void);
	TokenPtr PeekToken(void);
	TokenPtr ReadTokenWithType(Token::Type type);
	Token::Type PeekTokenType(void);

protected:
	std::unique_ptr<TokenDictionaryBase> _dictionary;

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
} // gotchangpdf

#endif /* _TOKENIZER_H */
