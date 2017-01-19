#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"

namespace gotchangpdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit InteractiveForm(syntax::DictionaryObjectPtr root);

	syntax::ArrayObjectPtr<FieldPtr> Fields(void) const;

private:
	static FieldPtr ConversionFunction(const syntax::DictionaryObjectPtr& obj);
};

} // semantics
} // gotchangpdf

#endif /* _INTERACTIVE_FORMS_H */
