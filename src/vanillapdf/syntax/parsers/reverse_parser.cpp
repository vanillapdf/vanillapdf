#include "precompiled.h"

#include "syntax/parsers/reverse_parser.h"
#include "syntax/parsers/parser_utils.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/misc_utils.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

ReverseParser::ReverseParser(IInputStreamPtr stream)
    : ReverseTokenizer(stream) {
}

types::stream_offset ReverseParser::ReadLastXrefOffset() {
    ReadTokenWithTypeSkip(Token::Type::REVERSE_END_OF_FILE_MARKER);
    auto offset_token = ReadTokenWithTypeSkip(Token::Type::REVERSE_INTEGER_OBJECT);
    ReadTokenWithTypeSkip(Token::Type::REVERSE_START_XREF);

    auto buffer = offset_token->ValueCopy();
    std::reverse(buffer.begin(), buffer.end());

    return MiscUtils::FromChars<types::stream_offset>(buffer);
}

TokenPtr ReverseParser::ReadTokenWithTypeSkip(Token::Type type) {
    auto offset = m_stream->GetInputPosition();
    for (;;) {
        auto token = ReadToken();

        if (token->GetType() == type) {
            return token;
        }

        if (token->GetType() == Token::Type::REVERSE_EOL) {
            continue;
        }

        LOG_ERROR_AND_THROW(syntax::ParseException,
            "Could not find token type {} at offset {}, instead token type {} with value {} was found",
            static_cast<int>(type),
            offset,
            static_cast<int>(token->GetType()),
            token->ValueView());
    }
}

} // syntax
} // vanillapdf
