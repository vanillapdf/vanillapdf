#include "precompiled.h"
#include "reverse_parser.h"
#include "exception.h"
#include "token.h"

namespace gotchangpdf
{
	namespace syntax
	{
		ReverseParser::ReverseParser(CharacterSource & stream)
			: ReverseTokenizer(stream) {}

		types::stream_offset ReverseParser::ReadLastXrefOffset()
		{
			ReadTokenWithTypeSkip(Token::Type::REVERSE_END_OF_FILE_MARKER);
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::REVERSE_INTEGER_OBJECT);
			ReadTokenWithTypeSkip(Token::Type::REVERSE_START_XREF);

			auto buffer = offset_token->Value();
			std::reverse(buffer.begin(), buffer.end());
			auto value = std::stoll(buffer->ToString());
			return value;
		}

		TokenPtr ReverseParser::ReadTokenWithTypeSkip(Token::Type type)
		{
			auto offset = tellg();
			for (;;) {
				auto token = ReadToken();

				if (token->GetType() == type)
					return token;

				if (token->GetType() == Token::Type::REVERSE_EOL)
					continue;

				std::stringstream ss;
				ss << "Could not find token type " << static_cast<int>(type) << " at offset " << offset;
				throw GeneralException(ss.str());
			}
		}
	}
}
