#include "precompiled.h"

#include "semantics/objects/content_stream.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operation_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		ContentStream::ContentStream(syntax::StreamObjectPtr obj) : HighLevelObject(obj) {}

		contents::BaseInstructionCollection ContentStream::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			auto body = _obj->GetBody();
			auto strm = body->ToStringStream();
			contents::ContentStreamParser parser(_obj->GetFile(), strm);
			_instructions = parser.ReadContentStreamInstructions();
			return _instructions;
		}
	}
}

