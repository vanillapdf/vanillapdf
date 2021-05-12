#include "precompiled.h"

#include "semantics/objects/content_stream.h"

#include "contents/content_stream_parser.h"
#include "contents/content_stream_operation_base.h"

namespace vanillapdf {
namespace semantics {

ContentStream::ContentStream(syntax::StreamObjectPtr obj) : HighLevelObject(obj) {}

contents::BaseInstructionCollectionPtr ContentStream::Instructions(void) const {
	if (_instructions->IsInitialized()) {
		return _instructions;
	}

	auto body = _obj->GetBody();
	auto input_stream = body->ToInputStream();

	contents::ContentStreamParser parser(_obj->GetFile(), input_stream);

	_instructions = parser.ReadContentStreamInstructions();
	_instructions->SetInitialized();

	return _instructions;
}

} // semantics
} // vanillapdf
