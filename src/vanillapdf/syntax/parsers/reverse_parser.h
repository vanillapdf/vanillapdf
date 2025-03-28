#ifndef _REVERSE_PARSER_H
#define _REVERSE_PARSER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/parsers/reverse_tokenizer.h"
#include "syntax/parsers/parser_interface.h"

namespace vanillapdf {
namespace syntax {

class ReverseParser : public ReverseTokenizer, public IReverseParser {
public:
    explicit ReverseParser(IInputStreamPtr stream);
    ReverseParser(const ReverseParser & other);

    types::stream_offset ReadLastXrefOffset() override;

private:
    TokenPtr ReadTokenWithTypeSkip(Token::Type type);
};

} // syntax
} // vanillapdf

#endif /* _REVERSE_PARSER_H */
