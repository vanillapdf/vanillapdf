#include "precompiled.h"
#include "catalog.h"

#include "constants.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "exception.h"
#include "page_tree.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"
#include "outline.h"
#include "destinations.h"
#include "name_dictionary.h"

namespace gotchangpdf
{
	namespace semantics
	{
		Catalog::Catalog(syntax::DictionaryObjectPtr root) : HighLevelObject(root)
		{
			if (root->Contains(constant::Name::Type) && root->FindAs<syntax::NameObjectPtr>(constant::Name::Type) != constant::Name::Catalog)
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, Catalog>(root);
		}

		bool Catalog::Version(gotchangpdf::Version& result) const
		{
			if (!_obj->Contains(constant::Name::Version))
				return false;

			auto ver = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Version);
			result = SemanticUtils::GetVersionFromName(ver);
			return true;
		}

		bool Catalog::Extensions(OutputDeveloperExtensionsPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Extensions))
				return false;

			auto extensions = DeveloperExtensionsPtr(_obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Extensions));
			extensions->SetDocument(GetDocument());

			result = extensions;
			return true;
		}

		PageTreePtr Catalog::Pages(void) const
		{
			if (!m_pages.empty()) {
				return m_pages;
			}

			auto pages_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Pages);
			auto pages = PageTreePtr(pages_obj);
			pages->SetDocument(GetDocument());

			m_pages = pages;
			return m_pages;
		}

		bool Catalog::PageLabels(OutputPageLabelsPtr& result) const
		{
			if (!_obj->Contains(constant::Name::PageLabels))
				return false;

			result = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::PageLabels);
			return true;
		}

		bool Catalog::PageLayout(Catalog::PageLayoutType& result) const
		{
			if (!_obj->Contains(constant::Name::PageLayout))
				return false;

			auto layout = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PageLayout);
			if (layout == constant::Name::SinglePage)
				result = PageLayoutType::SinglePage;
			else if (layout == constant::Name::OneColumn)
				result = PageLayoutType::OneColumn;
			else if (layout == constant::Name::TwoColumnLeft)
				result = PageLayoutType::TwoColumnLeft;
			else if (layout == constant::Name::TwoColumnRight)
				result = PageLayoutType::TwoColumnRight;
			else if (layout == constant::Name::TwoPageLeft)
				result = PageLayoutType::TwoPageLeft;
			else if (layout == constant::Name::TwoPageRight)
				result = PageLayoutType::TwoPageRight;
			else
				throw GeneralException("Unknown value in PageLayout entry: " + layout->ToString());

			return true;
		}

		bool Catalog::NeedsRendering(syntax::BooleanObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::NeedsRendering))
				return false;

			result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::NeedsRendering);
			return true;
		}

		bool Catalog::Names(OutputNameDictionaryPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Names))
				return false;

			auto names = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Names);
			NameDictionaryPtr dictionary(names);
			dictionary->SetDocument(GetDocument());

			result = dictionary;
			return true;
		}

		bool Catalog::Destinations(OutputNamedDestinationsPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Dests))
				return false;

			auto dests = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Dests);
			auto named_dests = NamedDestinationsPtr(dests);
			named_dests->SetDocument(GetDocument());

			result = named_dests;
			return true;
		}

		bool Catalog::ViewerPreferences(OutputViewerPreferencesPtr& result) const
		{
			if (!_obj->Contains(constant::Name::ViewerPreferences))
				return false;

			auto prefs = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::ViewerPreferences);
			auto viewer_prefs = ViewerPreferencesPtr(prefs);
			viewer_prefs->SetDocument(GetDocument());

			result = viewer_prefs;
			return true;
		}

		bool Catalog::PageMode(PageModeType& result) const
		{
			if (!_obj->Contains(constant::Name::PageMode))
				return false;

			auto page_mode = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PageMode);
			if (page_mode == constant::Name::UseNone)
				result = PageModeType::UseNone;
			else if (page_mode == constant::Name::UseOutlines)
				result = PageModeType::UseOutlines;
			else if (page_mode == constant::Name::UseThumbs)
				result = PageModeType::UseThumbs;
			else if (page_mode == constant::Name::FullScreen)
				result = PageModeType::FullScreen;
			else if (page_mode == constant::Name::UseOC)
				result = PageModeType::UseOC;
			else if (page_mode == constant::Name::UseAttachments)
				result = PageModeType::UseAttachments;
			else
				throw GeneralException("Unknown page mode type: " + page_mode->ToString());

			return true;
		}

		bool Catalog::Outlines(OutputOutlinePtr& result) const
		{
			if (!_obj->Contains(constant::Name::Outlines))
				return false;

			auto outline_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Outlines);
			auto outlines = OutlinePtr(outline_obj);
			outlines->SetDocument(GetDocument());

			result = outlines;
			return true;
		}
	}
}
