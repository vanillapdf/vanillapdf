#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/signature_flags.h"

namespace vanillapdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit InteractiveForm(syntax::DictionaryObjectPtr root);

    // Creates an empty form registered as an indirect object within the
    // document. Attach it through Catalog::SetAcroForm to take effect.
    static InteractiveFormPtr Create(DocumentPtr document);

    bool GetFields(OuputFieldCollectionPtr& result) const;
    void SetFields(FieldCollectionPtr value);

    bool GetSignatureFlags(OutputSignatureFlagsPtr& result) const;
    void SetSignatureFlags(SignatureFlagsPtr value);

    bool GetNeedAppearances(bool& result) const;
    void SetNeedAppearances(bool value);

    // Create-if-missing helpers retained for Document::Sign, which builds the
    // signature field on a form it may have just created itself
    FieldCollectionPtr CreateFields();
    SignatureFlagsPtr CreateSignatureFlags();
};

} // semantics
} // vanillapdf

#endif /* _INTERACTIVE_FORMS_H */
