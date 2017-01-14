#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/content_stream.h"
#include "contents/content_stream_instruction_base.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Contents : public HighLevelObject<syntax::ObjectPtr>, public IModifyObserver
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);
			~Contents();

			contents::BaseInstructionCollectionPtr Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			contents::InstructionBasePtr GetInstructionAt(types::uinteger at) const;

			virtual void ObserveeChanged(IModifyObservable*) override { m_dirty = true; }
			bool IsDirty() const noexcept { return m_dirty; }
			void SetDirty(bool dirty) noexcept { m_dirty = dirty; }

		private:
			mutable contents::BaseInstructionCollectionPtr m_instructions;

			bool m_dirty = false;
		};
	}
}

#endif /* _CONTENTS_H */
