#include "precompiled.h"
#include "resource_dictionary.h"

namespace gotchangpdf
{
	namespace semantics
	{
		ResourceDictionary::ResourceDictionary(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

		bool ResourceDictionary::Font(OutputFontMapPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Font)) {
				return false;
			}

			auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Font);
			result = FontMapPtr(dict);
			return true;
		}
	}
}
