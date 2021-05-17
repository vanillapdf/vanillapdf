#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/content_stream.h"
#include "contents/content_stream_instruction_base.h"

namespace vanillapdf {
namespace semantics {

class PageContents : public HighLevelObject<syntax::ObjectPtr>, public IModifyObserver {
public:
	explicit PageContents(syntax::StreamObjectPtr obj);
	explicit PageContents(syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> obj);
	~PageContents();

	contents::BaseInstructionCollectionPtr Instructions(void) const;
	contents::BaseInstructionCollection::IteratorPtr GetInstructionIterator() const;

	virtual void ObserveeChanged(const IModifyObservable*) override;

	bool IsDirty() const;
	void SetDirty(bool dirty);

	bool RecalculateStreamData();

private:
	mutable contents::BaseInstructionCollectionPtr m_instructions;
};

} // semantics
} // vanillapdf

#endif /* _CONTENTS_H */
