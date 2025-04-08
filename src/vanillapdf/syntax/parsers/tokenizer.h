#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "syntax/parsers/token.h"
#include "syntax/parsers/token_dictionary.h"

#include "utils/constants.h"
#include "utils/unknown_interface.h"
#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {
namespace syntax {

class Tokenizer : public IUnknown {
private:
    struct CacheItem {
        TokenPtr token;
        types::stream_offset advance_position;
    };

public:
    explicit Tokenizer(IInputStreamPtr stream);

    bool IsItemCached(types::stream_offset offset);
    const CacheItem& GetCachedItem(types::stream_offset offset);

    TokenPtr ReadToken(void);
    TokenPtr PeekToken(void);
    TokenPtr ReadTokenWithType(Token::Type type);
    Token::Type PeekTokenType(void);

protected:
    std::unique_ptr<TokenDictionaryBase> _dictionary;
    IInputStreamPtr m_stream;

private:
    std::map<types::stream_offset, CacheItem> _cache;

    TokenPtr ReadComment(void);
    TokenPtr ReadHexadecimalString(void);
    TokenPtr ReadLiteralString(void);
    TokenPtr ReadName(void);
    TokenPtr ReadUnknown(int ch);
};

} // syntax
} // vanillapdf

#endif /* _TOKENIZER_H */
