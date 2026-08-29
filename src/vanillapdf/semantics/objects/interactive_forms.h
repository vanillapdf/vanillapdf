#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/field_tree.h"
#include "semantics/objects/signature_flags.h"

#include <mutex>

namespace vanillapdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit InteractiveForm(syntax::DictionaryObjectPtr root);

    // Creates an empty form registered as an indirect object within the
    // document. Attach it through Catalog::SetAcroForm to take effect. The
    // document is the public form, mirrored by the C API; the file overload
    // is for the library's own use, where Document holds its file directly.
    static InteractiveFormPtr Create(DocumentPtr document);
    static InteractiveFormPtr Create(syntax::FilePtr file);

    // The field hierarchy over the /Fields entry - the flat terminal
    // enumeration, the structural walk and every field mutation live there.
    // Reading never creates the entry: a form without /Fields has no
    // hierarchy, and one is attached with SetFieldTree. One instance per
    // form, so that the cache it owns is shared by everyone holding this
    // form; the instance installed by SetFieldTree is the one handed out.
    bool GetFieldTree(OutputFieldTreePtr& result) const;
    void SetFieldTree(FieldTreePtr value);

    bool GetFields(OuputFieldCollectionPtr& result) const;

    bool GetSignatureFlags(OutputSignatureFlagsPtr& result) const;
    void SetSignatureFlags(SignatureFlagsPtr value);

    bool GetNeedAppearances(bool& result) const;
    void SetNeedAppearances(bool value);

    // Document-wide defaults for /DA and /Q (12.7.3.3). Field getters
    // resolve these through the /Parent chain only - when a field reports
    // the entry missing, the caller applies these form-level defaults.
    bool GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const;
    void SetDefaultAppearance(syntax::StringObjectPtr value);

    bool GetQuadding(Field::Quadding& result) const;
    void SetQuadding(Field::Quadding value);

    // The /DA and /Q a field is rendered with: the field's own entry, then
    // its ancestors through /Parent, then this form's document default
    // (12.7.3.3). The field getters stop at the hierarchy; the form owns
    // the last step because it owns the entry. /DA has no further default
    // and may be absent everywhere; /Q defaults to left justification
    // (Table 222), so it always resolves.
    bool ResolveDefaultAppearance(const FieldPtr& field, syntax::OutputStringObjectPtr& result) const;
    Field::Quadding ResolveQuadding(const FieldPtr& field) const;

    // Create-if-missing helper retained for Document::Sign, which sets the
    // flags on a form it may have just created itself. Fields have no such
    // path: Document::Sign attaches a FieldTree like any other caller, so
    // the tree's cache is never bypassed from inside the library.
    SignatureFlagsPtr CreateSignatureFlags();

private:
    // One lock for every operation on the form. The dictionary guards its
    // individual entries itself, but the form's check-then-act sequences -
    // the create-if-missing helpers, the remove-then-insert setters and the
    // resolve-once field tree - are only atomic when the whole sequence
    // runs under this lock. Distinct InteractiveForm wrappers over the same
    // dictionary each carry their own lock (canonicalization is issue #524).
    mutable std::unique_ptr<std::recursive_mutex> m_access_lock;

    // The tree instance is resolved once from /Fields and then handed out
    // unchanged, or installed by SetFieldTree. The setter can replace it,
    // so the lock-free fast path of CachedValue does not apply.
    mutable OutputFieldTreePtr m_field_tree;
};

} // semantics
} // vanillapdf

#endif /* _INTERACTIVE_FORMS_H */
