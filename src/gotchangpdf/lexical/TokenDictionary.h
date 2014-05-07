#ifndef _LEXICAL_TOKEN_DICTIONARY_H
#define _LEXICAL_TOKEN_DICTIONARY_H

#include "Buffer.h"
#include "Lexical/Token.h"
#include "LexicalTree.h"

#include <map>

namespace gotchangpdf
{
	namespace lexical
	{
		class TokenDictionary
		{
		private:
			TokenDictionary();
			static bool _initialized;

			//static std::map<CharacterSet, Pdf::Lexical::Token::Type> _dictionary;
			static Tree _dictionary;

			static void Initialize();

		public:
			//static std::vector<const char*> lexemes(enum Type key);
			static Token::Type find(const Buffer& set);
		};
	}
}

#endif /* _LEXICAL_TOKEN_DICTIONARY_H */
