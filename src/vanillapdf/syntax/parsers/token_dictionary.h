#ifndef _LEXICAL_TOKEN_DICTIONARY_H
#define _LEXICAL_TOKEN_DICTIONARY_H

#include "syntax/parsers/token.h"

#include "syntax/parsers/lexical_tree.h"

namespace vanillapdf {
namespace syntax {

class TokenDictionaryBase {
public:
	virtual ~TokenDictionaryBase();
	virtual void Initialize() = 0;
	Token::Type Find(std::string_view set);

protected:
	bool m_initialized = false;
	Tree m_dictionary;
};

class ParserTokenDictionary : public TokenDictionaryBase {
public:
	virtual void Initialize() override;
};

class ReverseParserTokenDictionary : public TokenDictionaryBase {
public:
	virtual void Initialize() override;
};

class ContentStreamTokenDictionary : public ParserTokenDictionary {
public:
	virtual void Initialize() override;
};

class CharacterMapTokenDictionary : public ParserTokenDictionary {
public:
	virtual void Initialize() override;
};

} // syntax
} // vanillapdf

#endif /* _LEXICAL_TOKEN_DICTIONARY_H */
