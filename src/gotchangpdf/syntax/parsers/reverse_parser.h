#ifndef _REVERSE_PARSER_H
#define _REVERSE_PARSER_H

#include "syntax_fwd.h"
#include "reverse_tokenizer.h"
#include "parser_interface.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ReverseParser : public ReverseTokenizer, public IReverseParser
		{
		public:
			ReverseParser(CharacterSource & stream);
			ReverseParser(const ReverseParser & other);

			types::stream_offset ReadLastXrefOffset();

		private:
			TokenPtr ReadTokenWithTypeSkip(Token::Type type);
		};
	}
}

#endif /* _REVERSE_PARSER_H */
