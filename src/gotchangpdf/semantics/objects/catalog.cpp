#include "precompiled.h"

#include "semantics/objects/catalog.h"
#include "semantics/objects/page_tree.h"
#include "semantics/objects/outline.h"
#include "semantics/objects/name_dictionary.h"

#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/boolean_object.h"

#include "semantics/utils/semantic_exceptions.h"
#include "semantics/utils/semantic_utils.h"

namespace gotchangpdf {
namespace semantics {

Catalog::Catalog(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {
	if (root->Contains(constant::Name::Type)) {
		auto type = root->FindAs<syntax::NameObjectPtr>(constant::Name::Type);
		if (type != constant::Name::Catalog) {
			throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, Catalog>(root);
		}
	}
}

bool Catalog::Version(gotchangpdf::Version& result) const {
	if (!_obj->Contains(constant::Name::Version)) {
		return false;
	}

	auto ver = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Version);
	result = SemanticUtils::GetVersionFromName(ver);
	return true;
}

bool Catalog::Extensions(OutputDeveloperExtensionsPtr& result) const {
	if (!_obj->Contains(constant::Name::Extensions)) {
		return false;
	}

	auto extensions = DeveloperExtensionsPtr(_obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Extensions));
	result = extensions;
	return true;
}

PageTreePtr Catalog::Pages(void) const {
	if (!m_pages.empty()) {
		return m_pages;
	}

	auto pages_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Pages);
	auto pages = PageTreePtr(pages_obj);
	m_pages = pages;
	return m_pages;
}

bool Catalog::PageLabels(OutputPageLabelsPtr& result) const {
	if (!_obj->Contains(constant::Name::PageLabels)) {
		return false;
	}

	result = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::PageLabels);
	return true;
}

bool Catalog::PageLayout(Catalog::PageLayoutType& result) const {
	if (!_obj->Contains(constant::Name::PageLayout)) {
		return false;
	}

	auto layout = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PageLayout);
	if (layout == constant::Name::SinglePage) {
		result = PageLayoutType::SinglePage;
	} else if (layout == constant::Name::OneColumn) {
		result = PageLayoutType::OneColumn;
	} else if (layout == constant::Name::TwoColumnLeft) {
		result = PageLayoutType::TwoColumnLeft;
	} else if (layout == constant::Name::TwoColumnRight) {
		result = PageLayoutType::TwoColumnRight;
	} else if (layout == constant::Name::TwoPageLeft) {
		result = PageLayoutType::TwoPageLeft;
	} else if (layout == constant::Name::TwoPageRight) {
		result = PageLayoutType::TwoPageRight;
	} else {
		throw GeneralException("Unknown value in PageLayout entry: " + layout->ToString());
	}

	return true;
}

bool Catalog::NeedsRendering(syntax::BooleanObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::NeedsRendering)) {
		return false;
	}

	result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::NeedsRendering);
	return true;
}

bool Catalog::Names(OutputNameDictionaryPtr& result) const {
	if (!_obj->Contains(constant::Name::Names)) {
		return false;
	}

	auto names = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Names);
	NameDictionaryPtr dictionary(names);
	result = dictionary;
	return true;
}

bool Catalog::Destinations(OutputNamedDestinationsPtr& result) const {
	if (!_obj->Contains(constant::Name::Dests)) {
		return false;
	}

	auto dests = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Dests);
	auto named_dests = NamedDestinationsPtr(dests);
	result = named_dests;
	return true;
}

bool Catalog::ViewerPreferences(OutputViewerPreferencesPtr& result) const {
	if (!_obj->Contains(constant::Name::ViewerPreferences)) {
		return false;
	}

	auto prefs = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::ViewerPreferences);
	auto viewer_prefs = ViewerPreferencesPtr(prefs);
	result = viewer_prefs;
	return true;
}

bool Catalog::PageMode(PageModeType& result) const {
	if (!_obj->Contains(constant::Name::PageMode)) {
		return false;
	}

	auto page_mode = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PageMode);
	if (page_mode == constant::Name::UseNone) {
		result = PageModeType::UseNone;
	} else if (page_mode == constant::Name::UseOutlines) {
		result = PageModeType::UseOutlines;
	} else if (page_mode == constant::Name::UseThumbs) {
		result = PageModeType::UseThumbs;
	} else if (page_mode == constant::Name::FullScreen) {
		result = PageModeType::FullScreen;
	} else if (page_mode == constant::Name::UseOC) {
		result = PageModeType::UseOC;
	} else if (page_mode == constant::Name::UseAttachments) {
		result = PageModeType::UseAttachments;
	} else {
		throw GeneralException("Unknown page mode type: " + page_mode->ToString());
	}

	return true;
}

bool Catalog::Outlines(OutputOutlinePtr& result) const {
	if (!_obj->Contains(constant::Name::Outlines)) {
		return false;
	}

	auto outline_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Outlines);
	auto outlines = OutlinePtr(outline_obj);
	result = outlines;
	return true;
}

bool Catalog::AcroForm(OuputInteractiveFormPtr& result) const {
	if (!_obj->Contains(constant::Name::AcroForm)) {
		return false;
	}

	auto form_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::AcroForm);
	auto interactive_form = InteractiveFormPtr(form_obj);
	result = interactive_form;
	return true;
}

} // semantics
} // gotchangpdf
