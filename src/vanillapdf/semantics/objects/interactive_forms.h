#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/signature_flags.h"

#include <map>
#include <mutex>
#include <string_view>
#include <vector>

namespace vanillapdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit InteractiveForm(syntax::DictionaryObjectPtr root);

    // Creates an empty form registered as an indirect object within the
    // document. Attach it through Catalog::SetAcroForm to take effect.
    static InteractiveFormPtr Create(DocumentPtr document);

    // Resolved terminal fields in document order. The field hierarchy's
    // grouping nodes exist only for naming and attribute inheritance and are
    // hidden, the same way PageTree hides its interior nodes behind Page().
    // A radio button group is a single terminal field with one value,
    // regardless of how many widget annotations represent it on the page.
    types::size_type GetFieldCount() const;
    FieldPtr GetField(types::size_type index) const;

    // Finds a terminal field by its fully qualified name - the /T partial
    // names joined with '.' (12.7.3.2), UTF-8 encoded
    bool TryFindField(std::string_view qualified_name, OuputFieldPtr& result) const;

    bool GetFields(OuputFieldCollectionPtr& result) const;
    void SetFields(FieldCollectionPtr value);

    // Appends a field to the root /Fields array, creating the array when the
    // form does not have one yet. The array holds indirect references
    // (Table 218), so the field's dictionary shall be an indirect object -
    // a direct dictionary is an InvalidParameterException.
    void AddField(FieldPtr value);

    bool GetSignatureFlags(OutputSignatureFlagsPtr& result) const;
    void SetSignatureFlags(SignatureFlagsPtr value);

    bool GetNeedAppearances(bool& result) const;
    void SetNeedAppearances(bool value);

    // Document-wide defaults for /DA and /Q (12.7.3.3). Field getters resolve
    // these through the /Parent chain only - when a field reports the entry
    // missing, the caller applies these form-level defaults.
    bool GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const;
    void SetDefaultAppearance(syntax::StringObjectPtr value);

    bool GetQuadding(Field::Quadding& result) const;
    void SetQuadding(Field::Quadding value);

    // Create-if-missing helpers retained for Document::Sign, which builds the
    // signature field on a form it may have just created itself
    FieldCollectionPtr CreateFields();
    SignatureFlagsPtr CreateSignatureFlags();

private:
    // Flat cache of terminal field dictionary pointers in document order.
    // Built in full on first access and cleared by SetFields and AddField, so
    // structural changes are reflected on the next access (same pattern as
    // PageTree's page cache). GetField constructs a Field on demand from the cached
    // dictionary pointer. Mutating the raw /Fields array through the
    // dictionary API bypasses the invalidation.
    mutable std::unique_ptr<std::recursive_mutex> m_cache_lock;
    mutable std::vector<syntax::DictionaryObjectPtr> m_field_cache;
    mutable bool m_cache_built = false;

    void BuildFieldCache() const;
    void BuildFieldCacheInternal(
        syntax::IndirectReferenceObjectPtr node_reference,
        std::map<syntax::IndirectReferenceId, bool>& visited) const;
    void InvalidateFieldCache();
};

} // semantics
} // vanillapdf

#endif /* _INTERACTIVE_FORMS_H */
