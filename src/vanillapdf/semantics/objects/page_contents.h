#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/content_stream.h"
#include "contents/content_stream_instruction_base.h"

namespace vanillapdf {
namespace semantics {

class PageContents : public HighLevelObject<syntax::ObjectPtr> {
public:
    explicit PageContents(syntax::StreamObjectPtr obj);
    explicit PageContents(syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> obj);
    ~PageContents() = default;

    // Creates empty page contents backed by a new indirect content stream.
    // PageObject::SetContents stores an indirect reference, so the stream has
    // to own an xref entry - this registers one within the document's file.
    static PageContentsPtr Create(DocumentPtr document);

    contents::BaseInstructionCollectionPtr Instructions(void) const;

    bool IsDirty() const;
    void SetDirty(bool dirty);

    bool RecalculateStreamData();

private:
    mutable contents::BaseInstructionCollectionPtr m_instructions;
};

} // semantics
} // vanillapdf

#endif /* _CONTENTS_H */
