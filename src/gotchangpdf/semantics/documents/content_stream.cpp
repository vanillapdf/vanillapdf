#include "precompiled.h"
#include "content_stream.h"

#include "spirit_parser.h"

namespace gotchangpdf
{
	namespace semantics
	{
		ContentStream::ContentStream(syntax::StreamObjectPtr obj) : HighLevelObject(obj) {}

		syntax::contents::GenericOperationCollection ContentStream::Operations(void) const
		{
			if (!_operations.empty())
				return _operations;

			auto body = _obj->GetBodyDecoded();
			auto strm = body->ToStringStream();
			auto parser = syntax::SpiritParser(_obj->GetFile(), strm);
			_operations = parser.ReadContentStreamOperations();
			return _operations;
		}
	}
}

