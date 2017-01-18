#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

namespace gotchangpdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit InteractiveForm(syntax::DictionaryObjectPtr root);

	syntax::ArrayObjectPtr<FieldPtr> Fields(void) const;
};

} // semantics
} // gotchangpdf

#endif /* _INTERACTIVE_FORMS_H */
