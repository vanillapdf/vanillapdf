#ifndef _TOKEN_H
#define _TOKEN_H

#include "BaseObjects/CharacterSet.h"
#include "streams/lexical/LexicalStreamFwd.h"
#include "streams/lexical/LexicalReverseStreamFwd.h"

namespace Pdf
{
	namespace Lexical
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

			Token();
			explicit Token(const CharacterSet& chars);
			explicit Token(Type type);
			Token(const char* chars, int len);
			Token(Type type, const CharacterSet& chars);

			Type type(void) const;

			const CharacterSet& value() const;

			Token& operator= (const CharacterSet& value);

			friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, Token& o);
			//friend class Streams::Lexical::ReverseStream;

			friend Streams::Lexical::Stream& operator>> (Streams::Lexical::Stream& s, Token& o);
			//friend class Streams::Lexical::Stream;

		protected:
			Token::Type _type;
			CharacterSet _value;

		private:
			Token::Type evaluate(const CharacterSet& chars) const;
		};
	}
}

#endif /* _TOKEN_H */
