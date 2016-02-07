#ifndef _CONTENT_STREAM_H
#define _CONTENT_STREAM_H

#include "semantics_fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ContentStream : public HighLevelObject<syntax::StreamObjectPtr>
		{
		public:
			explicit ContentStream(syntax::StreamObjectPtr obj);
			std::vector<syntax::contents::InstructionBasePtr> Instructions(void) const;

		private:
			mutable std::vector<syntax::contents::InstructionBasePtr> _instructions;
		};
	}
}

#endif /* _CONTENT_STREAM_H */
