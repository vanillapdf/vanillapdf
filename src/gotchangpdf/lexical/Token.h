#ifndef _TOKEN_H
#define _TOKEN_H

#include "fwd.h"
#include "Buffer.h"

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
			static Token::Type Evaluate(const Buffer& chars);

			Token();
			explicit Token(const Buffer& chars);
			explicit Token(Type type);
			Token(const char* chars, int len);
			Token(Type type, const Buffer& chars);

			Type type(void) const;

			const Buffer& value() const;

			Token& operator= (const Buffer& value);

			friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, Token& o);
			//friend class Streams::Lexical::ReverseStream;

			friend lexical::Stream& operator>> (lexical::Stream& s, Token& o);
			//friend class Streams::Lexical::Stream;

		private:
			Token::Type _type;
			Buffer _value;
		};
	}
}

#endif /* _TOKEN_H */
