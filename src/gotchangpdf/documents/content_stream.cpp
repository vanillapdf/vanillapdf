#include "precompiled.h"
#include "content_stream.h"

#include "spirit_parser.h"

namespace gotchangpdf
{
	namespace documents
	{
		ContentStream::ContentStream(StreamObjectPtr obj) : HighLevelObject(obj) {}

		lexical::ContentStreamOperationCollection ContentStream::Operations(void) const
		{
			auto body = _obj->GetBodyDecoded();
			auto strm = body->ToStringStream();
			auto parser = lexical::SpiritParser(_obj->GetFile(), strm);
			return parser.ReadContentStreamOperations();
		}
	}
}

