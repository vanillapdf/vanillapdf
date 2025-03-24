#include "precompiled.h"

#include "syntax/parsers/reverse_parser.h"

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

    auto buffer = offset_token->Value();
    std::reverse(buffer.begin(), buffer.end());
    auto value = std::stoll(buffer);
    return value;
}

TokenPtr ReverseParser::ReadTokenWithTypeSkip(Token::Type type) {
    auto offset = m_stream->GetInputPosition();
    for (;;) {
        auto token = ReadToken();

        if (token->GetType() == type)
            return token;

        if (token->GetType() == Token::Type::REVERSE_EOL)
            continue;

        std::stringstream ss;
        ss << "Could not find token type ";
        ss << static_cast<int>(type);
        ss << " at offset ";
        ss << offset;
        ss << ", instead token type ";
        ss << static_cast<int>(token->GetType());
        ss << " with value ";
        ss << token->Value();
        ss << " was found";

        throw GeneralException(ss.str());
    }
}

} // syntax
} // vanillapdf
