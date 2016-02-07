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

		class Destination : public HighLevelObject<syntax::MixedArrayObjectPtr>
		{
		public:
			explicit Destination(syntax::MixedArrayObjectPtr obj) : HighLevelObject(obj)
			{
			}

		private:
			PageObjectPtr _page;
			syntax::RealObjectPtr _x;
			syntax::RealObjectPtr _y;
			syntax::RealObjectPtr _z;
			syntax::ObjectPtr _zoom;
		};

		class NamedDestinations : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

		private:
			//NameTreePtr<Destination> _tree;
		};
	}
}

#endif /* _NAME_DICTIONARY_H */
