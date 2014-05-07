#include "Token.h"
#include "Exception.h"
#include "lexical/LexicalReverseStream.h"
#include "lexical/LexicalStream.h"
#include "TokenDictionary.h"

#include <cassert>

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace exceptions;

		#pragma region Constructors

		Token::Token(Token::Type type) : _type(type) {}
		Token::Token() : _type(Token::Type::UNKNOWN), _value() {}
		Token::Token(const Buffer& chars) : _type(Evaluate(chars)), _value(chars) { assert(_value.Size() > 0); }
		Token::Token(const char* chars, int len) : _value(chars, len), _type(Evaluate(_value)) { assert(_value.Size() > 0); }

		Token::Token(Type type, const Buffer& chars) : _type(type), _value(chars)
		{
			if (_type == Token::Type::UNKNOWN)
				_type = Evaluate(_value);
		}

		#pragma endregion

		#pragma region Operators

		Token& Token::operator=(const Buffer& s)
		{
			_value = s;
			_type = Evaluate(_value);
			return *this;
		}

		lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Token& o)
		{
			auto result = s.ReadToken();

			o._type = result->_type;
			o._value = result->_value;

			return s;
		}

		lexical::Stream& operator>>(lexical::Stream& s, Token& o)
		{
			auto result = s.ReadToken();
			o._type = result->_type;
			o._value = result->_value;

			return s;
		}

		#pragma endregion

		Token::Type Token::Evaluate(const Buffer& chars) { return TokenDictionary::find(chars); }

		const Buffer& Token::value() const { return _value; }
		Token::Type Token::type(void) const { return _type; }

		const char* Token::GetTypeValueName(Type type)
		{
			// TODO this should be a single line solution using preprocessor

			switch (type)
			{
			case Type::UNKNOWN:
				return "UNKNOWN";
			case Type::EOL:
				return "EOL";
			case Type::DICTIONARY_BEGIN:
				return "DICTIONARY_BEGIN";
			case Type::DICTIONARY_END:
				return "DICTIONARY_END";
			case Type::ARRAY_BEGIN:
				return "ARRAY_BEGIN";
			case Type::ARRAY_END:
				return "ARRAY_END";
			case Type::NAME_OBJECT:
				return "NAME_OBJECT";
			case Type::HEXADECIMAL_STRING:
				return "HEXADECIMAL_STRING";
			case Type::LITERAL_STRING:
				return "LITERAL_STRING";
			case Type::INTEGER_OBJECT:
				return "INTEGER_OBJECT";
			case Type::REAL_OBJECT:
				return "REAL_OBJECT";
			case Type::INDIRECT_REFERENCE_MARKER:
				return "INDIRECT_REFERENCE_MARKER";
			case Type::STREAM_BEGIN:
				return "STREAM_BEGIN";
			case Type::STREAM_END:
				return "STREAM_END";
			case Type::INDIRECT_OBJECT_BEGIN:
				return "INDIRECT_OBJECT_BEGIN";
			case Type::INDIRECT_OBJECT_END:
				return "INDIRECT_OBJECT_END";
			case Type::XREF_MARKER:
				return "XREF_MARKER";
			case Type::START_XREF:
				return "START_XREF";
			case Type::END_OF_FILE:
				return "END_OF_FILE";
			case Type::TRAILER:
				return "TRAILER";
			default:
				assert(false);
				throw Exception("FIXME: Token type is not mapped");
			}
		}
	}
}
