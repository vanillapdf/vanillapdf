#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "font.h"

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
