#include "TokenDictionary.h"
#include "Exception.h"

#include <cassert>

namespace Pdf
{
	using Pdf::Lexical::Token;
	using namespace std;

	bool TokenDictionary::_initialized = false;

	static const Character nul(Character::WhiteSpace::NUL);
	//rtv::Trie<Character, Token::Type> TokenDictionary::_dictionary(nul);
	//std::map<CharacterSet, Token::Type> TokenDictionary::_dictionary;
	Lexical::Tree TokenDictionary::_dictionary;

	// TODO "non-aggregates cannot be initialized with initializer list"
	// TODO c++03
	/*
	static const vector<const char*> EOL;// = { "\n", " \n", "\r\n" };
	static const vector<const char*> DICTIONARY_BEGIN;// = { "<<" };
	static const vector<const char*> DICTIONARY_END;// = { ">>" };
	static const vector<const char*> STREAM_BEGIN;// = { "stream" };
	static const vector<const char*> STREAM_END;// = { "endstream" };
	static const vector<const char*> INDIRECT_OBJECT_BEGIN;// = { "obj" };
	static const vector<const char*> INDIRECT_OBJECT_END;// = { "endobj" };
	*/

	static vector<const char*> STREAM_BEGIN;// = { "stream" };
	static vector<const char*> STREAM_END;// = { "endstream" };
	static vector<const char*> INDIRECT_OBJECT_BEGIN;// = { "obj" };
	static vector<const char*> INDIRECT_OBJECT_END;// = { "endobj" };
	static vector<const char*> XREF_MARKER;// = { "xref" };
	static vector<const char*> START_XREF_MARKER;// = { "startxref" };
	static vector<const char*> END_OF_LINE_MARKER;// = { "%%EOF" };
	static vector<const char*> TRAILER_MARKER;// = { "TRAILER" };

	Token::Type TokenDictionary::find(const CharacterSet& set)
	{
		Initialize();

		return _dictionary.TokenType(set);
	}

	bool TokenDictionary::IsPartOfToken(const CharacterSet& set)
	{
		Initialize();

		return _dictionary.PathExists(set);
	}

	void TokenDictionary::Initialize()
	{
		if (_initialized)
			return;

		// TODO ifdef c++03
		STREAM_BEGIN.push_back("stream");
		STREAM_END.push_back("endstream");
		INDIRECT_OBJECT_BEGIN.push_back("obj");
		INDIRECT_OBJECT_END.push_back("endobj");
		XREF_MARKER.push_back("xref");
		START_XREF_MARKER.push_back("startxref");
		END_OF_LINE_MARKER.push_back("%%EOF");
		TRAILER_MARKER.push_back("trailer");
		// ENDIF

		Token::Type type;
		auto insert = [&type](const char* ch)
		{
			assert(nullptr != ch);

			CharacterSet set(ch, strlen(ch) + 1);
			_dictionary.Insert(set, type);
		};

		type = Token::Type::STREAM_BEGIN;
		for_each(STREAM_BEGIN.begin(), STREAM_BEGIN.end(), insert);

		type = Token::Type::STREAM_END;
		for_each(STREAM_END.begin(), STREAM_END.end(), insert);

		type = Token::Type::INDIRECT_OBJECT_BEGIN;
		for_each(INDIRECT_OBJECT_BEGIN.begin(), INDIRECT_OBJECT_BEGIN.end(), insert);

		type = Token::Type::INDIRECT_OBJECT_END;
		for_each(INDIRECT_OBJECT_END.begin(), INDIRECT_OBJECT_END.end(), insert);

		type = Token::Type::XREF_MARKER;
		for_each(XREF_MARKER.begin(), XREF_MARKER.end(), insert);

		type = Token::Type::START_XREF;
		for_each(START_XREF_MARKER.begin(), START_XREF_MARKER.end(), insert);

		type = Token::Type::END_OF_FILE;
		for_each(END_OF_LINE_MARKER.begin(), END_OF_LINE_MARKER.end(), insert);

		type = Token::Type::TRAILER;
		for_each(TRAILER_MARKER.begin(), TRAILER_MARKER.end(), insert);

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
