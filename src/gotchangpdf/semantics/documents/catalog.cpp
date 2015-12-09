#include "precompiled.h"
#include "catalog.h"

#include "constants.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "exception.h"
#include "page_tree.h"
#include "semantic_exceptions.h"

#include <regex>

namespace gotchangpdf
{
	namespace semantics
	{
		Catalog::Catalog(syntax::DictionaryObjectPtr root) : HighLevelObject(root)
		{
			if (*root->FindAs<syntax::NameObjectPtr>(constant::Name::Type) != constant::Name::Catalog)
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, Catalog>(root);
		}

		Version Catalog::Version(void) const
		{
			if (!_obj->Contains(constant::Name::Version))
				throw OptionalEntryMissingException(_obj, constant::Name::Version);

			auto ver = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Version);
			auto ver_str = ver->Value()->ToString();

			std::smatch sm;
			std::regex header_regex("([0-9])\\.([0-9])");
			if (!std::regex_match(ver_str, sm, header_regex))
				throw NotSupportedException("Unsupported pdf version: " + ver_str);

			assert(sm.size() == 3);
			if (1 != stoi(sm[1]))
				throw NotSupportedException("Unsupported pdf version: " + ver_str);

			switch (stoi(sm[2])) {
			case 0: return Version::PDF10;
			case 1: return Version::PDF11;
			case 2: return Version::PDF12;
			case 3: return Version::PDF13;
			case 4: return Version::PDF14;
			case 5: return Version::PDF15;
			case 6: return Version::PDF16;
			case 7: return Version::PDF17;
			default: throw NotSupportedException("Unsupported pdf version: " + ver_str);
			}
		}

		PageTreePtr Catalog::Pages(void) const
		{
			auto pages = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Pages);
			return PageTreePtr(pages);
		}
	}
}
