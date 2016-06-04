#include "precompiled.h"
#include "content_stream.h"

#include "parser.h"
#include "content_stream_operation_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		ContentStream::ContentStream(syntax::StreamObjectPtr obj) : HighLevelObject(obj) {}

		syntax::contents::BaseInstructionCollection ContentStream::Instructions(void) const
		{
			if (!_instructions.empty())
				return _instructions;

			auto body = _obj->GetBody();
			auto strm = body->ToStringStream();
			auto parser = syntax::Parser(_obj->GetFile(), strm);
			_instructions = parser.ReadContentStreamInstructions();
			return _instructions;
		}
	}
}

