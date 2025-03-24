#include "precompiled.h"

#include "semantics/objects/viewer_preferences.h"
#include "semantics/utils/semantic_exceptions.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

ViewerPreferences::ViewerPreferences(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
PageRange::SubRange::SubRange(syntax::IntegerObjectPtr first, syntax::IntegerObjectPtr last)
    : _first_page(first), _last_page(last) {
}

PageRange::PageRange(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> obj) : HighLevelObject(obj) {
    assert(obj->GetSize() % 2 == 0);
    if (obj->GetSize() % 2 != 0) {
        throw SemanticContextExceptionFactory::Construct<syntax::ArrayObject<syntax::IntegerObjectPtr>, PageRange>(obj);
    }
}

syntax::IntegerObjectPtr PageRange::SubRange::FirstPage(void) const {
    return _first_page;
}

syntax::IntegerObjectPtr PageRange::SubRange::LastPage(void) const {
    return _last_page;
}

types::size_type PageRange::GetSize(void) const {
    return _obj->GetSize() / 2;
}

PageRange::SubRangePtr PageRange::At(types::size_type at) const {
    return make_deferred<SubRange>(_obj->GetValue(at), _obj->GetValue(at + 1));
}

bool ViewerPreferences::HideToolbar(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::HideToolbar)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideToolbar);
    return true;
}

bool ViewerPreferences::HideMenubar(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::HideMenubar)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideMenubar);
    return true;
}

bool ViewerPreferences::HideWindowUI(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::HideWindowUI)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::HideWindowUI);
    return true;
}

bool ViewerPreferences::FitWindow(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::FitWindow)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::FitWindow);
    return true;
}

bool ViewerPreferences::CenterWindow(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::CenterWindow)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::CenterWindow);
    return true;
}

bool ViewerPreferences::DisplayDocTitle(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::DisplayDocTitle)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::DisplayDocTitle);
    return true;
}

bool ViewerPreferences::NonFullScreenPageMode(NonFullScreenPageModeType& result) const {
    if (!_obj->Contains(constant::Name::NonFullScreenPageMode)) {
        return false;
    }

    auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::NonFullScreenPageMode);
    if (name == constant::Name::UseNone) {
        result = NonFullScreenPageModeType::UseNone;
    } else if (name == constant::Name::UseOutlines) {
        result = NonFullScreenPageModeType::UseOutlines;
    } else if (name == constant::Name::UseThumbs) {
        result = NonFullScreenPageModeType::UseThumbs;
    } else if (name == constant::Name::UseOC) {
        result = NonFullScreenPageModeType::UseOC;
    } else {
        throw GeneralException("Unknown page mode type: " + name->ToString());
    }

    return true;
}

bool ViewerPreferences::Direction(ReadingOrderType& result) const {
    if (!_obj->Contains(constant::Name::Direction)) {
        return false;
    }

    auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Direction);
    if (name == constant::Name::L2R) {
        result = ReadingOrderType::LeftToRight;
    } else if (name == constant::Name::R2L) {
        result = ReadingOrderType::RightToLeft;
    } else {
        throw GeneralException("Unknown reading order: " + name->ToString());
    }

    return true;
}

bool ViewerPreferences::ViewArea(syntax::NameObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::ViewArea)) {
        return false;
    }

    result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::ViewArea);
    return true;
}

bool ViewerPreferences::ViewClip(syntax::NameObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::ViewClip)) {
        return false;
    }

    result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::ViewClip);
    return true;
}

bool ViewerPreferences::PrintArea(syntax::NameObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::PrintArea)) {
        return false;
    }

    result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintArea);
    return true;
}

bool ViewerPreferences::PrintClip(syntax::NameObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::PrintClip)) {
        return false;
    }

    result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintClip);
    return true;
}

bool ViewerPreferences::PrintScaling(PrintScalingType& result) const {
    if (!_obj->Contains(constant::Name::PrintScaling)) {
        return false;
    }

    auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::PrintScaling);
    if (name == constant::Name::AppDefault) {
        result = PrintScalingType::AppDefault;
    } else if (name == constant::Name::None) {
        result = PrintScalingType::None;
    } else {
        throw GeneralException("Unknown print scaling: " + name->ToString());
    }

    return true;
}

bool ViewerPreferences::Duplex(DuplexType& result) const {
    if (!_obj->Contains(constant::Name::Duplex)) {
        return false;
    }

    auto name = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Duplex);
    if (name == constant::Name::Simplex) {
        result = DuplexType::Simplex;
        return true;
    }

    if (name == constant::Name::DuplexFlipShortEdge) {
        result = DuplexType::DuplexFlipShortEdge;
        return true;
    }

    if (name == constant::Name::DuplexFlipLongEdge) {
        result = DuplexType::DuplexFlipLongEdge;
        return true;
    }

    throw GeneralException("Unknown duplex: " + name->ToString());
}

bool ViewerPreferences::PickTrayByPDFSize(syntax::BooleanObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::PickTrayByPDFSize)) {
        return false;
    }

    result = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::PickTrayByPDFSize);
    return true;
}

bool ViewerPreferences::PrintPageRange(OutputPageRangePtr& result) const {
    if (!_obj->Contains(constant::Name::PrintPageRange)) {
        return false;
    }

    auto range = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(constant::Name::PrintPageRange);
    result = make_deferred<PageRange>(range);
    return true;
}

bool ViewerPreferences::NumCopies(syntax::IntegerObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::NumCopies)) {
        return false;
    }

    result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::NumCopies);
    return true;
}

} // semantics
} // vanillapdf
