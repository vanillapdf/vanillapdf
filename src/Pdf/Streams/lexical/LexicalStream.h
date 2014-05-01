#ifndef _LEXICAL_STREAM_H
#define _LEXICAL_STREAM_H

#include "streams/lexical/LexicalBaseStream.h"
#include "streams/basic/BasicStream.h"
#include "BaseObjects/Object.h"
#include "Constants.h"

namespace Pdf
{
	namespace Streams
	{
		namespace Lexical
		{
			class Stream : public BaseStream, public Basic::Stream
			{
			public:
				explicit Stream(std::istream& s);

				//friend Stream& Pdf::operator>> (Stream& s, IntegerObject& o);
				//friend Stream& operator>> (Stream& s, Token& o);
				//friend Stream& operator>> (Stream& s, Header& o);
				//friend Stream& operator>> (Stream& s, CrossReferenceTable& o);
				//friend Stream& operator>> (Stream& s, DictionaryObject& o);
				//friend Stream& operator>> (Stream& s, ArrayObject& o);

				virtual std::shared_ptr<Pdf::Lexical::Token> ReadToken() override;
				virtual std::shared_ptr<Pdf::Lexical::Token> PeekToken() override;
				virtual Pdf::Lexical::Token::Type PeekTokenType() override;

				Pdf::Streams::Lexical::Stream::Stream(const Pdf::Streams::Lexical::Stream &);

				//using Basic::Stream::seekg;
				//using Lexical::BaseStream::ReadTokenWithType;

				virtual ~Stream();

			private:
				std::shared_ptr<Pdf::Lexical::Token> _last_token;
				streamOffsetValueType _last_token_offset, _advance_position;
			};
		}
	}
}

#endif /* _LEXICAL_STREAM_H */
