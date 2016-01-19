#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "semantics_fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ResourceDictionary : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			//PageObject();
			explicit ResourceDictionary(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

			//ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			//ObjectReferenceWrapper<DictionaryObject> Resources(void) const { return _resources; }
		};
	}
}

#endif /* _RESOURCE_DICTIONARY_H */
