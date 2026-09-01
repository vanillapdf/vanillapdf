#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/rectangle.h"
#include "semantics/utils/semantic_exceptions.h"
#include "semantics/objects/document.h"
#include "semantics/objects/annotations.h"
#include "semantics/objects/transparency_group.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/integer_object.h"

#include "syntax/utils/name_constants.h"

#include <cmath>

namespace vanillapdf {
namespace semantics {

using namespace constant;
using namespace syntax;

PageObject::PageObject(DictionaryObjectPtr obj) : PageNodeBase(obj) {
    if (_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page) {
        throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageObject>(obj);
    }
}

bool PageObject::GetResources(OutputResourceDictionaryPtr& result) const {
    if (!_obj->Contains(constant::Name::Resources)) {
        return false;
    }

    auto resources_obj = _obj->FindAs<DictionaryObjectPtr>(constant::Name::Resources);
    auto resources = make_deferred<ResourceDictionary>(resources_obj);
    result = resources;
    return true;
}

void PageObject::SetResources(ResourceDictionaryPtr resources) {
    _obj->Remove(Name::Resources);

    _obj->Insert(Name::Resources, resources->GetObject());
}

bool PageObject::GetRectangle(const NameObject& name, OutputRectanglePtr& result) const {
    if (!_obj->Contains(name)) {
        return false;
    }

    auto box_obj = _obj->FindAs<ArrayObjectPtr<RealObjectPtr>>(name);
    result = make_deferred<Rectangle>(box_obj);
    return true;
}

void PageObject::SetRectangle(const NameObject& name, RectanglePtr value) {
    _obj->Insert(name, value->GetObject(), true);
}

bool PageObject::GetMediaBox(OutputRectanglePtr& result) const {
    return GetRectangle(Name::MediaBox, result);
}

void PageObject::SetMediaBox(RectanglePtr media_box) {
    SetRectangle(Name::MediaBox, media_box);
}

bool PageObject::GetCropBox(OutputRectanglePtr& result) const {
    return GetRectangle(Name::CropBox, result);
}

void PageObject::SetCropBox(RectanglePtr value) {
    SetRectangle(Name::CropBox, value);
}

bool PageObject::GetBleedBox(OutputRectanglePtr& result) const {
    return GetRectangle(Name::BleedBox, result);
}

void PageObject::SetBleedBox(RectanglePtr value) {
    SetRectangle(Name::BleedBox, value);
}

bool PageObject::GetTrimBox(OutputRectanglePtr& result) const {
    return GetRectangle(Name::TrimBox, result);
}

void PageObject::SetTrimBox(RectanglePtr value) {
    SetRectangle(Name::TrimBox, value);
}

bool PageObject::GetArtBox(OutputRectanglePtr& result) const {
    return GetRectangle(Name::ArtBox, result);
}

void PageObject::SetArtBox(RectanglePtr value) {
    SetRectangle(Name::ArtBox, value);
}

bool PageObject::GetRotate(types::big_int& result) const {
    if (!_obj->Contains(Name::Rotate)) {
        return false;
    }

    auto rotate = _obj->FindAs<IntegerObjectPtr>(Name::Rotate);
    result = rotate->GetIntegerValue();
    return true;
}

void PageObject::SetRotate(types::big_int value) {
    if (value % 90 != 0) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Page rotation has to be a multiple of 90: {}", value);
    }

    IntegerObjectPtr rotate = make_deferred<IntegerObject>(value);
    _obj->Insert(Name::Rotate, rotate, true);
}

bool PageObject::GetUserUnit(types::real& result) const {
    if (!_obj->Contains(Name::UserUnit)) {
        return false;
    }

    auto user_unit = _obj->FindAs<RealObjectPtr>(Name::UserUnit);
    result = user_unit->GetValue();
    return true;
}

void PageObject::SetUserUnit(types::real value) {
    // NaN compares false against everything and infinity has no PDF
    // representation, so both have to be rejected separately
    if (!std::isfinite(value) || value <= 0) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Page user unit has to be positive: {}", value);
    }

    RealObjectPtr user_unit = make_deferred<RealObject>(value);
    _obj->Insert(Name::UserUnit, user_unit, true);
}

bool PageObject::GetGroup(OutputTransparencyGroupPtr& result) const {
    if (!_obj->Contains(Name::Group)) {
        return false;
    }

    auto group_obj = _obj->FindAs<DictionaryObjectPtr>(Name::Group);
    result = make_deferred<TransparencyGroup>(group_obj);
    return true;
}

void PageObject::SetGroup(TransparencyGroupPtr value) {
    _obj->Insert(Name::Group, value->GetObject(), true);
}

bool PageObject::GetAnnotations(OutputPageAnnotationsPtr& result) const {
    if (!_obj->Contains(constant::Name::Annots)) {
        return false;
    }

    auto annots_obj = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Annots);
    auto annots = make_deferred<PageAnnotations>(annots_obj);
    result = annots;
    return true;
}

void PageObject::SetAnnotations(PageAnnotationsPtr annots) {
    _obj->Remove(Name::Annots);

    _obj->Insert(Name::Annots, annots->GetObject());
}


void PageObject::SetContents(PageContentsPtr contents) {
    _obj->Remove(Name::Contents);

    IndirectReferenceObjectPtr contents_ref = make_deferred<IndirectReferenceObject>(contents->GetObject());
    _obj->Insert(Name::Contents, contents_ref);
}

bool PageObject::GetContents(OutputPageContentsPtr& result) const {
    if (!m_contents.empty()) {
        result = m_contents;
        return true;
    }

    OutputContainableObjectPtr content;
    auto found = _obj->TryFind(Name::Contents, content);

    // Missing entry, return empty collection
    if (!found) {
        return false;
    }

    bool is_null = ObjectUtils::IsType<NullObjectPtr>(*content);
    bool is_ref = ObjectUtils::IsType<StreamObjectPtr>(*content);
    bool is_array = ObjectUtils::IsType<ArrayObjectPtr<StreamObjectPtr>>(*content);

    // Missing entry, return empty collection
    if (is_null) {
        return false;
    }

    // Content shall be stream or array of references to stream
    assert(is_ref ^ is_array);
    if (!(is_ref ^ is_array)) {

        // This is programming safe check
        assert(!(is_ref && is_array) && "Error in object utils, object is stream and array at the same time");

        auto base_type_str = Object::TypeName(content->GetObjectType());
        LOG_ERROR_AND_THROW(syntax::ParseException, "Invalid contents type: {}", base_type_str);
    }

    if (is_ref) {
        auto data = ObjectUtils::ConvertTo<StreamObjectPtr>(*content);
        PageContentsPtr contents = make_deferred<PageContents>(data);
        m_contents = contents;
        result = contents;
        return true;
    }

    if (is_array) {
        auto data = ObjectUtils::ConvertTo<ArrayObjectPtr<IndirectReferenceObjectPtr>>(*content);
        PageContentsPtr contents = make_deferred<PageContents>(data);
        m_contents = contents;
        result = contents;
        return true;
    }

    LOG_ERROR_AND_THROW(InvalidParameterException, "Unreachable code");
}

PageObjectPtr PageObject::Create(DocumentPtr document) {
    auto file = document->GetFile();

    syntax::DictionaryObjectPtr obj;
    obj->Insert(Name::Type, Name::Page.Clone());

    XrefUsedEntryBasePtr new_entry = file->AllocateNewEntry();
    new_entry->SetReference(obj);
    new_entry->SetFile(file);
    new_entry->SetInitialized();

    obj->SetFile(file);
    obj->SetInitialized();

    return make_deferred<PageObject>(obj);
}

PageObjectPtr PageObject::Create(syntax::DictionaryObjectPtr obj) {
    return make_deferred<PageObject>(obj);
}

bool PageObjectExtensions::GetInheritableResources(const PageObject& obj, OutputResourceDictionaryPtr& result) {
    auto page_dictionary = obj.GetObject();
    if (!page_dictionary->Contains(constant::Name::Resources)) {
        if (!page_dictionary->Contains(constant::Name::Parent)) {
            return false;
        }

        auto parent_dictionary_obj = page_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Parent);
        auto parent_obj = PageObject::Create(parent_dictionary_obj);
        return GetInheritableResources(*parent_obj, result);
    }

    auto resources_obj = page_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Resources);
    auto resources = make_deferred<ResourceDictionary>(resources_obj);
    result = resources;
    return true;
}

} // semantics
} // vanillapdf
