#include "precompiled.h"
#include "content_stream.h"

#include "spirit_parser.h"

namespace gotchangpdf
{
	namespace semantics
	{
		ContentStream::ContentStream(syntax::StreamObjectPtr obj) : HighLevelObject(obj) {}

		syntax::contents::OperationCollection ContentStream::Operations(void) const
		{
			auto body = _obj->GetBodyDecoded();
			auto strm = body->ToStringStream();
			auto parser = syntax::SpiritParser(_obj->GetFile(), strm);
			return parser.ReadContentStreamOperations();
		}
	}
}

