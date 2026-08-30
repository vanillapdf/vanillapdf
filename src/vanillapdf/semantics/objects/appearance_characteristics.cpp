#include "precompiled.h"

#include "semantics/objects/appearance_characteristics.h"
#include "semantics/objects/color.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

bool AppearanceCharacteristics::GetRotation(types::big_int& result) const {
    if (!_obj->Contains(constant::Name::R)) {
        return false;
    }

    auto rotation = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::R);
    result = rotation->GetIntegerValue();
    return true;
}

void AppearanceCharacteristics::SetRotation(types::big_int value) {
    auto rotation = make_deferred<syntax::IntegerObject>(value);
    _obj->Insert(constant::Name::R, rotation, true);
}

bool AppearanceCharacteristics::GetBorderColor(OutputColorPtr& result) const {
    if (!_obj->Contains(constant::Name::BC)) {
        return false;
    }

    auto color_obj = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::BC);
    result = make_deferred<Color>(color_obj);
    return true;
}

void AppearanceCharacteristics::SetBorderColor(ColorPtr value) {
    _obj->Insert(constant::Name::BC, value->GetObject(), true);
}

bool AppearanceCharacteristics::GetBackgroundColor(OutputColorPtr& result) const {
    if (!_obj->Contains(constant::Name::BG)) {
        return false;
    }

    auto color_obj = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::BG);
    result = make_deferred<Color>(color_obj);
    return true;
}

void AppearanceCharacteristics::SetBackgroundColor(ColorPtr value) {
    _obj->Insert(constant::Name::BG, value->GetObject(), true);
}

bool AppearanceCharacteristics::GetNormalCaption(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::CA)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CA);
    return true;
}

void AppearanceCharacteristics::SetNormalCaption(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::CA, value, true);
}

bool AppearanceCharacteristics::GetRolloverCaption(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::RC)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::RC);
    return true;
}

void AppearanceCharacteristics::SetRolloverCaption(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::RC, value, true);
}

bool AppearanceCharacteristics::GetDownCaption(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::AC)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::AC);
    return true;
}

void AppearanceCharacteristics::SetDownCaption(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::AC, value, true);
}

} // semantics
} // vanillapdf
