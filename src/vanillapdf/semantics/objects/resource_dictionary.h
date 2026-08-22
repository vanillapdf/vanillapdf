#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/font.h"

#include <mutex>
#include <vector>

namespace vanillapdf {
namespace semantics {

class ResourceDictionary : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit ResourceDictionary(syntax::DictionaryObjectPtr obj);

    // Optional
    bool Font(OutputFontMapPtr& result) const;

    // Flat enumeration of the /XObject sub-dictionary resource names.
    // The count is zero when the resource dictionary has no /XObject entry.
    types::size_type GetXObjectCount() const;
    syntax::NameObjectPtr GetXObjectName(types::size_type index) const;

    // Looks up a named entry in the /XObject sub-dictionary. The result is
    // the semantic XObject - check XObjectBase::GetXObjectType and convert
    // to the concrete type, the same way annotations are handled.
    bool TryFindXObject(const syntax::NameObject& name, OutputXObjectPtr& result) const;

    // Registers an XObject under the resource name, creating the /XObject
    // sub-dictionary when missing and overwriting an existing entry. The
    // entry stores an indirect reference, so the XObject's stream has to be
    // registered within the document - see FormXObject::Create.
    void AddXObject(const syntax::NameObject& name, XObjectPtr value);
    bool RemoveXObject(const syntax::NameObject& name);

private:
    // Flat cache of the /XObject resource names in dictionary order.
    // Built in full on first access and cleared by AddXObject/RemoveXObject,
    // so structural changes are reflected on the next access (same pattern
    // as PageTree's page cache). Mutating the raw /XObject sub-dictionary
    // through the dictionary API bypasses the invalidation.
    mutable std::unique_ptr<std::recursive_mutex> m_cache_lock;
    mutable std::vector<syntax::NameObjectPtr> m_xobject_name_cache;
    mutable bool m_cache_built = false;

    void BuildXObjectNameCache() const;
    void InvalidateXObjectNameCache();
};

} // semantics
} // vanillapdf

#endif /* _RESOURCE_DICTIONARY_H */
