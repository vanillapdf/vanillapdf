#ifndef _LEXICAL_REVERSE_STREAM_H
#define _LEXICAL_REVERSE_STREAM_H

#include "lexical_base_stream.h"
#include "basic_reverse_stream.h"
#include "constants.h"

#include <vector>

namespace gotchangpdf
{
	namespace lexical
	{
		class ReverseStream : public lexical::BaseStream, public basic::ReverseStream
		{
		public:
			explicit ReverseStream(std::istream& stream);

			virtual std::shared_ptr<gotchangpdf::lexical::Token> ReadToken() override;
			virtual std::shared_ptr<gotchangpdf::lexical::Token> PeekToken() override;

			virtual ~ReverseStream();

		private:
			std::shared_ptr<gotchangpdf::lexical::Token> _last_token;
			streamOffsetValueType _last_token_offset, _advance_position;
		};
	}
}

#endif /* _LEXICAL_REVERSE_STREAM_H */
