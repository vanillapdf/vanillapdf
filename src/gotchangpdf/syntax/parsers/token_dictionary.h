#ifndef _LEXICAL_TOKEN_DICTIONARY_H
#define _LEXICAL_TOKEN_DICTIONARY_H

#include "token.h"
#include "lexical_tree.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class TokenDictionaryBase
		{
		public:
			virtual ~TokenDictionaryBase();
			virtual void Initialize() = 0;
			Token::Type Find(BufferPtr set);

		protected:
			bool m_initialized = false;
			Tree m_dictionary;
		};

		class ParserTokenDictionary : public TokenDictionaryBase
		{
		public:
			virtual void Initialize() override;
		};

		class ReverseParserTokenDictionary : public TokenDictionaryBase
		{
		public:
			virtual void Initialize() override;
		};

		class ContentStreamTokenDictionary : public ParserTokenDictionary
		{
		public:
			virtual void Initialize() override;
		};
	}
}

#endif /* _LEXICAL_TOKEN_DICTIONARY_H */
