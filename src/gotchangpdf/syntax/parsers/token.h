#ifndef _TOKEN_H
#define _TOKEN_H

#include "syntax_fwd.h"
#include "buffer.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Token : public IUnknown
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
				END_OF_FILE_MARKER,
				TRAILER,
				NULL_OBJECT,
				TRUE_VALUE,
				FALSE_VALUE,
				END_OF_FILE
			};

			static const char* GetTypeValueName(Type type);
			static Token::Type Evaluate(BufferPtr chars);

			Token() = default;
			explicit Token(BufferPtr chars);
			explicit Token(Type type);
			Token(Type type, BufferPtr chars);

			Token::Type GetType(void) const { return _type; }
			BufferPtr Value() const { return _value; }

		private:
			Type _type = Type::UNKNOWN;
			BufferPtr _value;
		};
	}
}

#endif /* _TOKEN_H */
