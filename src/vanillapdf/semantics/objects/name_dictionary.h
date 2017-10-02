#ifndef _NAME_DICTIONARY_H
#define _NAME_DICTIONARY_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/tree.h"

namespace vanillapdf {
namespace semantics {

class NameDictionary : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit NameDictionary(syntax::DictionaryObjectPtr root);

	bool Dests(OutputNameTreePtr<DestinationPtr>& result) const;

	//NameTreePtr AP(void) const;
	//NameTreePtr JavaScript(void) const;
	//NameTreePtr Pages(void) const;
	//NameTreePtr Templates(void) const;
	//NameTreePtr IDS(void) const;
	//NameTreePtr URLS(void) const;
	//NameTreePtr EmbeddedFiles(void) const;
	//NameTreePtr Renditions(void) const;

private:
	static DestinationPtr DestinationConversionFunction(const syntax::ContainableObjectPtr& obj);
};

} // semantics
} // vanillapdf

#endif /* _NAME_DICTIONARY_H */
