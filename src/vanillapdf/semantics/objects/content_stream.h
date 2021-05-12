#ifndef _CONTENT_STREAM_H
#define _CONTENT_STREAM_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "contents/content_stream_instruction_base.h"

namespace vanillapdf {
namespace semantics {

class ContentStream : public HighLevelObject<syntax::StreamObjectPtr> {
public:
	explicit ContentStream(syntax::StreamObjectPtr obj);
	contents::BaseInstructionCollectionPtr Instructions(void) const;

private:
	mutable contents::BaseInstructionCollectionPtr _instructions;
};

} // semantics
} // vanillapdf

#endif /* _CONTENT_STREAM_H */
