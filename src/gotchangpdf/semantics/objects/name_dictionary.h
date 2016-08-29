#ifndef _NAME_DICTIONARY_H
#define _NAME_DICTIONARY_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "tree.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class NameDictionary : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit NameDictionary(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
			//NamedDestinationsPtr Dests(void) const;
			//NameTreePtr AP(void) const;
			//NameTreePtr JavaScript(void) const;
			//NameTreePtr Pages(void) const;
			//NameTreePtr Templates(void) const;
			//NameTreePtr IDS(void) const;
			//NameTreePtr URLS(void) const;
			//NameTreePtr EmbeddedFiles(void) const;
			//NameTreePtr Renditions(void) const;
		};
	}
}

#endif /* _NAME_DICTIONARY_H */
