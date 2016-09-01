#ifndef _NAME_DICTIONARY_H
#define _NAME_DICTIONARY_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "destinations.h"
#include "tree.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class NameDictionary : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit NameDictionary(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

			bool Dests(OutputNameTreePtr<DestinationPtr>& result) const
			{
				if (!_obj->Contains(constant::Name::Dests)) {
					return false;
				}

				auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Dests);
				NameTreePtr<DestinationPtr> tree(
					dict, [this](const syntax::ContainableObjectPtr& obj) {
					return DestinationBase::Create(obj, GetDocument());
				});

				result = tree;
				return true;
			}

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
