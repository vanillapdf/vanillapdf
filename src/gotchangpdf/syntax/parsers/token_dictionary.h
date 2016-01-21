#ifndef _LEXICAL_TOKEN_DICTIONARY_H
#define _LEXICAL_TOKEN_DICTIONARY_H

#include "token.h"
#include "lexical_tree.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class TokenDictionary
		{
		public:
			void Initialize();
			Token::Type Find(BufferPtr set);

		private:
			bool _initialized = false;
			Tree _dictionary;
		};
	}
}

#endif /* _LEXICAL_TOKEN_DICTIONARY_H */
