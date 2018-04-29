#ifndef _INTERACTIVE_FORMS_H
#define _INTERACTIVE_FORMS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"

namespace vanillapdf {
namespace semantics {

class InteractiveForm : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit InteractiveForm(syntax::DictionaryObjectPtr root);

	bool Fields(OuputFieldCollectionPtr& result) const;
	FieldCollectionPtr CreateFields();
};

} // semantics
} // vanillapdf

#endif /* _INTERACTIVE_FORMS_H */
