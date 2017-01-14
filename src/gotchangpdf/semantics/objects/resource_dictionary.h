#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/font.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class ResourceDictionary : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit ResourceDictionary(syntax::DictionaryObjectPtr obj);

			// Optional
			bool Font(OutputFontMapPtr& result) const;
		};
	}
}

#endif /* _RESOURCE_DICTIONARY_H */
