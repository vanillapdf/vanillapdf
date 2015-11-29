#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ResourceDictionary : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			//PageObject();
			explicit ResourceDictionary(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

			//inline ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			//inline ObjectReferenceWrapper<DictionaryObject> Resources(void) const { return _resources; }

			virtual inline Type GetType() const override { return HighLevelObject::Type::ResourceDictionary; }
		};
	}
}

#endif /* _RESOURCE_DICTIONARY_H */
