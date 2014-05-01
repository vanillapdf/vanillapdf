#ifndef _LEXICAL_REVERSE_STREAM_H
#define _LEXICAL_REVERSE_STREAM_H

#include "streams/lexical/LexicalBaseStream.h"
#include "streams/Basic/BasicReverseStream.h"
#include "Constants.h"

#include <vector>

namespace Pdf
{
	namespace Streams
	{
		namespace Lexical
		{
			class ReverseStream : public Lexical::BaseStream, public Basic::ReverseStream
			{
			public:
				explicit ReverseStream(std::istream& stream);

				virtual std::shared_ptr<Pdf::Lexical::Token> ReadToken() override;
				virtual std::shared_ptr<Pdf::Lexical::Token> PeekToken() override;

				virtual ~ReverseStream();

			private:
				std::shared_ptr<Pdf::Lexical::Token> _last_token;
				streamOffsetValueType _last_token_offset, _advance_position;
			};
		}
	}
}

#endif /* _LEXICAL_REVERSE_STREAM_H */
