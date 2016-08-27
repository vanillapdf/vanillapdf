#ifndef _CONTENT_STREAM_H
#define _CONTENT_STREAM_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "content_stream_instruction_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ContentStream : public HighLevelObject<syntax::StreamObjectPtr>
		{
		public:
			explicit ContentStream(syntax::StreamObjectPtr obj);
			syntax::contents::BaseInstructionCollection Instructions(void) const;

		private:
			mutable syntax::contents::BaseInstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENT_STREAM_H */
