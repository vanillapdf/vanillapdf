#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics_fwd.h"
#include "content_stream.h"
#include "content_stream_instruction_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);

			syntax::contents::BaseInstructionCollection Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			syntax::contents::InstructionBasePtr GetInstructionAt(types::uinteger at) const;

			inline types::uinteger GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
			mutable syntax::contents::BaseInstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENTS_H */
