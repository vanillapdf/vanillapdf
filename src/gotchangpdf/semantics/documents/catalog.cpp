#include "precompiled.h"
#include "catalog.h"

#include "constants.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "exception.h"
#include "page_tree.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

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
			return SemanticUtils::GetVersionFromName(ver);
		}

		DeveloperExtensionsPtr Catalog::Extensions(void) const
		{
			if (!_obj->Contains(constant::Name::Extensions))
				throw OptionalEntryMissingException(_obj, constant::Name::Extensions);

			return DeveloperExtensionsPtr(_obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Extensions));
		}

		PageTreePtr Catalog::Pages(void) const
		{
			auto pages = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Pages);
			return PageTreePtr(pages);
		}

		PageLabelsPtr Catalog::PageLabels(void) const
		{
			if (!_obj->Contains(constant::Name::PageLabels))
				throw OptionalEntryMissingException(_obj, constant::Name::PageLabels);

			auto labels = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::PageLabels);
			return PageLabelsPtr(labels);
		}

		PageLayout Catalog::PageLayout(void) const
		{
			if (!_obj->Contains(constant::Name::PageLayout))
				throw OptionalEntryMissingException(_obj, constant::Name::PageLayout);

			auto layout = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PageLayout);
			if (layout == constant::Name::SinglePage)
				return PageLayout::SinglePage;
			else if (layout == constant::Name::OneColumn)
				return PageLayout::OneColumn;
			else if (layout == constant::Name::TwoColumnLeft)
				return PageLayout::TwoColumnLeft;
			else if (layout == constant::Name::TwoColumnRight)
				return PageLayout::TwoColumnRight;
			else if (layout == constant::Name::TwoPageLeft)
				return PageLayout::TwoPageLeft;
			else if (layout == constant::Name::TwoPageRight)
				return PageLayout::TwoPageRight;
			else
				throw GeneralException("Unknown value in PageLayout entry: " + layout->ToString());
		}
		bool Catalog::ViewerPreferences(ViewerPreferencesPtr& result) const
		{
			if (!_obj->Contains(constant::Name::ViewerPreferences))
				return false;

			auto prefs = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::ViewerPreferences);
			result = ViewerPreferencesPtr(prefs);
			return true;
		}
	}
}
