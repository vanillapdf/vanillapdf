#ifndef _LEXICAL_STREAM_H
#define _LEXICAL_STREAM_H

#include "lexical_base_stream.h"
#include "raw_stream.h"
#include "object.h"
#include "constants.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Stream : public BaseStream, public raw::Stream
		{
		public:
			friend lexical::Stream& operator>> (lexical::Stream& s, char& o);

		public:
			explicit Stream(CharacterSource & s);
			Stream(const Stream & other);

			virtual std::shared_ptr<lexical::Token> ReadToken() override;
			virtual std::shared_ptr<lexical::Token> PeekToken() override;
			virtual lexical::Token::Type PeekTokenType() override;

			//using Basic::Stream::seekg;
			//using Lexical::BaseStream::ReadTokenWithType;

			virtual ~Stream();

		private:
			std::shared_ptr<lexical::Token> _last_token;
			streamOffsetValueType _last_token_offset, _advance_position;
		};

		inline lexical::Stream& operator>>(lexical::Stream& s, char& o)
		{
			o = s.get();
			return s;
		}
	}
}

#endif /* _LEXICAL_STREAM_H */
