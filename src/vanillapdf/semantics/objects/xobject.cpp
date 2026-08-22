#include "precompiled.h"

#include "syntax/files/file.h"

#include "semantics/objects/xobject.h"
#include "semantics/objects/document.h"
#include "semantics/objects/rectangle.h"
#include "semantics/objects/resource_dictionary.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

using namespace syntax;

XObjectBase::XObjectBase(StreamObjectPtr obj) : HighLevelObject(obj) {}
FormXObject::FormXObject(StreamObjectPtr obj) : XObjectBase(obj) {}
ImageXObject::ImageXObject(StreamObjectPtr obj) : XObjectBase(obj) {}
PostScriptXObject::PostScriptXObject(StreamObjectPtr obj) : XObjectBase(obj) {}

XObjectPtr XObjectBase::Create(StreamObjectPtr obj) {
    auto header = obj->GetHeader();

    // Verify type, if it is included
    if (header->Contains(constant::Name::Type)) {
        auto type = header->FindAs<NameObjectPtr>(constant::Name::Type);
        if (type != constant::Name::XObject) {
            LOG_ERROR_AND_THROW(ParseException, "Invalid XObject type: {}", type->ToString());
        }
    }

    if (!header->Contains(constant::Name::Subtype)) {
        LOG_ERROR_AND_THROW(ObjectMissingException, "XObject stream does not contain subtype");
    }

    auto subtype = header->FindAs<NameObjectPtr>(constant::Name::Subtype);

    if (subtype == constant::Name::Form) {
        return make_deferred<FormXObject>(obj);
    }

    if (subtype == constant::Name::Image) {
        return make_deferred<ImageXObject>(obj);
    }

    if (subtype == constant::Name::PS) {
        return make_deferred<PostScriptXObject>(obj);
    }

    LOG_ERROR_AND_THROW(ParseException, "Unknown XObject subtype: {}", subtype->ToString());
}

FormXObjectPtr FormXObject::Create(DocumentPtr document) {
    auto file = document->GetFile();

    StreamObjectPtr form_stream;

    XrefUsedEntryBasePtr new_entry = file->AllocateNewEntry();
    new_entry->SetReference(form_stream);
    new_entry->SetFile(file);
    new_entry->SetInitialized();

    form_stream->SetFile(file);
    form_stream->SetInitialized();

    IntegerObjectPtr form_type = make_deferred<IntegerObject>(1);
    RectanglePtr bounding_box;

    auto header = form_stream->GetHeader();
    header->Insert(constant::Name::Type, constant::Name::XObject.Clone());
    header->Insert(constant::Name::Subtype, constant::Name::Form.Clone());
    header->Insert(constant::Name::FormType, form_type);
    header->Insert(constant::Name::BBox, bounding_box->GetObject());

    return make_deferred<FormXObject>(form_stream);
}

bool FormXObject::GetBoundingBox(OutputRectanglePtr& result) const {
    auto header = _obj->GetHeader();
    if (!header->Contains(constant::Name::BBox)) {
        return false;
    }

    auto bounding_box_obj = header->FindAs<ArrayObjectPtr<RealObjectPtr>>(constant::Name::BBox);
    auto bounding_box = make_deferred<Rectangle>(bounding_box_obj);
    result = bounding_box;
    return true;
}

void FormXObject::SetBoundingBox(RectanglePtr value) {
    auto header = _obj->GetHeader();
    header->Insert(constant::Name::BBox, value->GetObject(), true);
}

bool FormXObject::GetMatrix(MixedArrayObjectPtr& result) const {
    auto header = _obj->GetHeader();
    if (!header->Contains(constant::Name::Matrix)) {
        return false;
    }

    result = header->FindAs<MixedArrayObjectPtr>(constant::Name::Matrix);
    return true;
}

void FormXObject::SetMatrix(MixedArrayObjectPtr value) {
    auto header = _obj->GetHeader();
    header->Insert(constant::Name::Matrix, value, true);
}

bool FormXObject::GetResources(OutputResourceDictionaryPtr& result) const {
    auto header = _obj->GetHeader();
    if (!header->Contains(constant::Name::Resources)) {
        return false;
    }

    auto resources_obj = header->FindAs<DictionaryObjectPtr>(constant::Name::Resources);
    result = make_deferred<ResourceDictionary>(resources_obj);
    return true;
}

void FormXObject::SetResources(DictionaryObjectPtr value) {
    auto header = _obj->GetHeader();
    header->Insert(constant::Name::Resources, value, true);
}

} // semantics
} // vanillapdf
