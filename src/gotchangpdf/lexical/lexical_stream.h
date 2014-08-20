#ifndef _LEXICAL_STREAM_H
#define _LEXICAL_STREAM_H

#include "lexical_base_stream.h"
#include "basic_stream.h"
#include "object.h"
#include "constants.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Stream : public BaseStream, public basic::Stream
		{
		public:
			explicit Stream(std::istream& s);

			//friend Stream& Pdf::operator>> (Stream& s, IntegerObject& o);
			//friend Stream& operator>> (Stream& s, Token& o);
			//friend Stream& operator>> (Stream& s, Header& o);
			//friend Stream& operator>> (Stream& s, CrossReferenceTable& o);
			//friend Stream& operator>> (Stream& s, DictionaryObject& o);
			//friend Stream& operator>> (Stream& s, MixedArrayObject& o);

			virtual std::shared_ptr<gotchangpdf::lexical::Token> ReadToken() override;
			virtual std::shared_ptr<gotchangpdf::lexical::Token> PeekToken() override;
			virtual gotchangpdf::lexical::Token::Type PeekTokenType() override;

			Stream(const Stream &);

			//using Basic::Stream::seekg;
			//using Lexical::BaseStream::ReadTokenWithType;

			virtual ~Stream();

		private:
			std::shared_ptr<gotchangpdf::lexical::Token> _last_token;
			streamOffsetValueType _last_token_offset, _advance_position;
		};
	}
}

#endif /* _LEXICAL_STREAM_H */
