#ifndef _LEXICAL_TOKEN_DICTIONARY_H
#define _LEXICAL_TOKEN_DICTIONARY_H

#include "Trie.h"
#include "CharacterSet.h"
#include "Lexical/Token.h"
#include "LexicalTree.h"

#include <map>

namespace Pdf
{
	class TokenDictionary
	{
	private:
		TokenDictionary();
		static bool _initialized;
		
		//static rtv::Trie<Character, Pdf::Lexical::Token::Type> _dictionary;
		//static std::map<CharacterSet, Pdf::Lexical::Token::Type> _dictionary;
		static Lexical::Tree _dictionary;

		static void Initialize();

	public:
		//static std::vector<const char*> lexemes(enum Type key);
		static Lexical::Token::Type find(const CharacterSet& set);
		static bool IsPartOfToken(const CharacterSet& set);
	};
}

#endif /* _LEXICAL_TOKEN_DICTIONARY_H */
