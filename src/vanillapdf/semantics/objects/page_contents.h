#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/content_stream.h"
#include "contents/content_stream_instruction_base.h"

namespace vanillapdf {
namespace semantics {

class PageContents : public HighLevelObject<syntax::ObjectPtr>, public IModifyObserver {
public:
	PageContents() = default;
	explicit PageContents(syntax::StreamObjectPtr obj);
	explicit PageContents(syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> obj);
	~PageContents();

	contents::BaseInstructionCollectionPtr Instructions(void) const;
	types::size_type GetInstructionsSize(void) const;
	contents::InstructionBasePtr GetInstructionAt(types::size_type at) const;

	virtual void ObserveeChanged(const IModifyObservable*) override {
		m_dirty = true;
	}

	bool IsDirty() const noexcept { return m_dirty; }
	void SetDirty(bool dirty) noexcept { m_dirty = dirty; }

private:
	mutable contents::BaseInstructionCollectionPtr m_instructions;

	bool m_dirty = false;
};

} // semantics
} // vanillapdf

#endif /* _CONTENTS_H */
