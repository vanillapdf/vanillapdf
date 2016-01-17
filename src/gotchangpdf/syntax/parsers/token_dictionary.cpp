#include "precompiled.h"
#include "token_dictionary.h"
#include "exception.h"

#include <cassert>
#include <algorithm>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		bool TokenDictionary::_initialized = false;

		//std::map<CharacterSet, Token::Type> TokenDictionary::_dictionary;
		Tree TokenDictionary::_dictionary;

		static const char STREAM_BEGIN[] = "stream";
		static const char STREAM_END[] = "endstream";
		static const char INDIRECT_OBJECT_BEGIN[] = "obj";
		static const char INDIRECT_OBJECT_END[] = "endobj";
		static const char XREF_MARKER[] = "xref";
		static const char START_XREF[] = "startxref";
		static const char END_OF_FILE[] = "%%EOF";
		static const char TRAILER[] = "trailer";
		static const char NULL_OBJECT[] = "null";
		static const char TRUE_VALUE[] = "true";
		static const char FALSE_VALUE[] = "false";

		Token::Type TokenDictionary::Find(BufferPtr set)
		{
			Initialize();
			return _dictionary.TokenType(set);
		}

#define INSERT_TO_DICTIONARY(name) _dictionary.Insert(BufferPtr(name, sizeof(name)), Token::Type::name)

		void TokenDictionary::Initialize()
		{
			if (_initialized)
				return;

			INSERT_TO_DICTIONARY(STREAM_BEGIN);
			INSERT_TO_DICTIONARY(STREAM_END);
			INSERT_TO_DICTIONARY(INDIRECT_OBJECT_BEGIN);
			INSERT_TO_DICTIONARY(INDIRECT_OBJECT_END);
			INSERT_TO_DICTIONARY(XREF_MARKER);
			INSERT_TO_DICTIONARY(START_XREF);
			INSERT_TO_DICTIONARY(END_OF_FILE);
			INSERT_TO_DICTIONARY(TRAILER);
			INSERT_TO_DICTIONARY(NULL_OBJECT);
			INSERT_TO_DICTIONARY(TRUE_VALUE);
			INSERT_TO_DICTIONARY(FALSE_VALUE);

			_initialized = true;
		}

		/*
		vector<const char*> lexemes(Token::Type key)
		{
		switch (key)
		{
		case Token::Type::EOL:
		return EOL;
		case Token::Type::DICTIONARY_BEGIN:
		return DICTIONARY_BEGIN;
		case Token::Type::DICTIONARY_END:
		return DICTIONARY_END;
		case Token::Type::STREAM_BEGIN:
		return STREAM_BEGIN;
		case Token::Type::STREAM_END:
		return STREAM_END;
		case Token::Type::UNKNOWN:
		default:
		throw Exception("Unknown token type");
		}
		}
		*/
	}
}
