#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics_fwd.h"
#include "content_stream.h"
#include "content_stream_instruction_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Contents : public HighLevelObject<syntax::ObjectPtr>
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);

			syntax::contents::BaseInstructionCollection Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			syntax::contents::InstructionBasePtr GetInstructionAt(types::uinteger at) const;

		private:
			mutable syntax::contents::BaseInstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENTS_H */
