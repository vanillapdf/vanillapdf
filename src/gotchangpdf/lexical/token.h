#ifndef _TOKEN_H
#define _TOKEN_H

#include "fwd.h"
#include "buffer.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Token
		{
		public:
			enum class Type : unsigned char
			{
				UNKNOWN = 0,
				EOL,
				DICTIONARY_BEGIN,
				DICTIONARY_END,
				ARRAY_BEGIN,
				ARRAY_END,
				NAME_OBJECT,
				HEXADECIMAL_STRING,
				LITERAL_STRING,
				INTEGER_OBJECT,
				REAL_OBJECT,
				INDIRECT_REFERENCE_MARKER,
				STREAM_BEGIN,
				STREAM_END,
				INDIRECT_OBJECT_BEGIN,
				INDIRECT_OBJECT_END,
				XREF_MARKER,
				START_XREF,
				END_OF_FILE,
				TRAILER
			};

			static const char* GetTypeValueName(Type type);
			static Token::Type Evaluate(BufferPtr chars);

			Token();
			Token(const Token& other);
			Token(Token && other);
			explicit Token(BufferPtr chars);
			explicit Token(Type type);
			Token(Type type, BufferPtr chars);

			Token::Type GetType(void) const;

			BufferPtr Value() const;

			Token& operator= (const Token& value);

			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Token& o);
			//friend class Streams::Lexical::ReverseStream;

			friend lexical::Stream& operator>> (lexical::Stream& s, Token& o);
			//friend class Streams::Lexical::Stream;

		private:
			Token::Type _type;
			BufferPtr _value;
		};

		inline BufferPtr Token::Value() const { return _value; }
		inline Token::Type Token::GetType(void) const { return _type; }
	}
}

#endif /* _TOKEN_H */
