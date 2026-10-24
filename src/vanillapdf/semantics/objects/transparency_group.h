#ifndef _TRANSPARENCY_GROUP_H
#define _TRANSPARENCY_GROUP_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/dictionary_object.h"

namespace vanillapdf {
namespace semantics {

// Group attributes dictionary (/Group) of a page or a form XObject,
// see Table 147. It describes how the group's contents are composited
// in the transparent imaging model (11.6.6). The group colour space
// (/CS) is not modelled until a colour space type exists - it stays
// reachable through GetObject.
class TransparencyGroup : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    // An empty group with the mandatory /S /Transparency subtype
    TransparencyGroup();
    explicit TransparencyGroup(syntax::DictionaryObjectPtr obj);

    // Whether the group is isolated (/I) - composited against a fully
    // transparent backdrop rather than the group's own backdrop
    bool GetIsolated(bool& result) const;
    void SetIsolated(bool value);

    // Whether the group is a knockout group (/K) - each element composited
    // with the group's initial backdrop, overriding earlier elements
    bool GetKnockout(bool& result) const;
    void SetKnockout(bool value);
};

} // semantics
} // vanillapdf

#endif /* _TRANSPARENCY_GROUP_H */
