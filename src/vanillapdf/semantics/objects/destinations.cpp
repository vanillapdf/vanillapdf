#include "precompiled.h"

#include "semantics/objects/destinations.h"
#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/catalog.h"
#include "semantics/utils/semantic_exceptions.h"
#include "semantics/utils/semantic_utils.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

DestinationBase::DestinationBase(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}
XYZDestination::XYZDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitDestination::FitDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitHorizontalDestination::FitHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitVerticalDestination::FitVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitRectangleDestination::FitRectangleDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxDestination::FitBoundingBoxDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxHorizontalDestination::FitBoundingBoxHorizontalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxVerticalDestination::FitBoundingBoxVerticalDestination(syntax::MixedArrayObjectPtr root) : DestinationBase(root) {}

DestinationBase::DestinationBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
XYZDestination::XYZDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitDestination::FitDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitHorizontalDestination::FitHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitVerticalDestination::FitVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitRectangleDestination::FitRectangleDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxDestination::FitBoundingBoxDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxHorizontalDestination::FitBoundingBoxHorizontalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}
FitBoundingBoxVerticalDestination::FitBoundingBoxVerticalDestination(syntax::DictionaryObjectPtr root) : DestinationBase(root) {}

DestinationBase::Type XYZDestination::GetType() const noexcept { return DestinationBase::Type::XYZ; }

bool XYZDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(1);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool XYZDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool XYZDestination::GetZoom(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 4) return false;

    auto obj = arr->GetValue(3);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

DestinationBase::Type FitDestination::GetType() const noexcept { return DestinationBase::Type::Fit; }

bool FitHorizontalDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}
DestinationBase::Type FitHorizontalDestination::GetType() const noexcept { return DestinationBase::Type::FitHorizontal; }

bool FitVerticalDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}
DestinationBase::Type FitVerticalDestination::GetType() const noexcept { return DestinationBase::Type::FitVertical; }

bool FitRectangleDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetBottom(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 4) return false;

    auto obj = arr->GetValue(3);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetRight(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 5) return false;

    auto obj = arr->GetValue(4);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 6) return false;

    auto obj = arr->GetValue(5);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}
DestinationBase::Type FitRectangleDestination::GetType() const noexcept { return DestinationBase::Type::FitRectangle; }
DestinationBase::Type FitBoundingBoxDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBox; }

bool FitBoundingBoxHorizontalDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}
DestinationBase::Type FitBoundingBoxHorizontalDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBoxHorizontal; }

bool FitBoundingBoxVerticalDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}
DestinationBase::Type FitBoundingBoxVerticalDestination::GetType() const noexcept { return DestinationBase::Type::FitBoundingBoxVertical; }

NamedDestinations::NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

DestinationPtr DestinationBase::Create(syntax::ObjectPtr root) {
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(root)) {
        auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(root);
        return Create(arr);
    }

    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(root)) {
        auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(root);
        return Create(dict);
    }

    LOG_ERROR_AND_THROW_GENERAL("Invalid object type: {}", static_cast<int32_t>(root->GetObjectType()));
}

DestinationPtr DestinationBase::Create(syntax::MixedArrayObjectPtr root) {
    if (root->GetSize() < 2) {
        throw GeneralException("Invalid destination array");
    }

    syntax::ObjectPtr page_number_obj = root->GetValue(0);
    syntax::ObjectPtr type_obj = root->GetValue(1);

    // page_number_obj shall be indirect reference to page object
    // for remote go to actions it can be integer value of destination page

    // Update 27.9.2025:
    // The page number can be null, where no action is taken by Foxit.
    // Example file is excerpts.pdf /D[null/XYZ 2.04765 614.058 null]
    if (!syntax::ObjectUtils::IsType<syntax::IntegerObjectPtr>(page_number_obj)
        && !syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(page_number_obj)
        && !syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(page_number_obj)) {
        LOG_ERROR_AND_THROW_GENERAL("Invalid page object type in destination: {}",
            static_cast<int32_t>(page_number_obj->GetObjectType()));
    }

    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
        LOG_ERROR_AND_THROW_GENERAL("Destination type is not name object: {}",
            static_cast<int32_t>(type_obj->GetObjectType()));
    }

    syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

    if (type == constant::Name::XYZ) {
        return make_deferred<XYZDestination>(root);
    }

    if (type == constant::Name::Fit) {
        return make_deferred<FitDestination>(root);
    }

    if (type == constant::Name::FitH) {
        return make_deferred<FitHorizontalDestination>(root);
    }

    if (type == constant::Name::FitV) {
        return make_deferred<FitVerticalDestination>(root);
    }

    if (type == constant::Name::FitR) {
        return make_deferred<FitRectangleDestination>(root);
    }

    if (type == constant::Name::FitB) {
        return make_deferred<FitBoundingBoxDestination>(root);
    }

    if (type == constant::Name::FitBH) {
        return make_deferred<FitBoundingBoxHorizontalDestination>(root);
    }

    if (type == constant::Name::FitBV) {
        return make_deferred<FitBoundingBoxVerticalDestination>(root);
    }

    LOG_ERROR_AND_THROW_GENERAL("Unknown destination type: {}", type->ToString());
}

DestinationPtr DestinationBase::ResolveDestination(syntax::ObjectPtr dest_obj) {
    // Handle direct array destination: [page /Type ...]
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(dest_obj)) {
        auto array_obj = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(dest_obj);
        return Create(array_obj);
    }

    // Handle direct dictionary destination: {/D [page /Type ...]}
    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(dest_obj)) {
        auto dict_obj = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(dest_obj);
        return Create(dict_obj);
    }

    // Handle string name reference to named destinations (via Names dictionary)
    if (syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(dest_obj)) {
        auto weak_file = dest_obj->GetFile();
        auto document_ref = SemanticUtils::GetMappedDocument(weak_file);

        assert(!document_ref.IsEmpty() && "Document reference was not set");
        if (!document_ref.IsActive()) {
            throw GeneralException("Document reference is not active");
        }

        DocumentPtr document = document_ref.GetReference();

        OutputCatalogPtr catalog_ptr;
        bool has_catalog = document->GetDocumentCatalog(catalog_ptr);
        if (!has_catalog) {
            throw GeneralException("Document does not have a catalog");
        }

        OutputNameDictionaryPtr name_dictionary;
        bool has_dictionary = catalog_ptr->Names(name_dictionary);
        if (!has_dictionary) {
            throw GeneralException("Document does not have a name dictionary");
        }

        OutputNameTreePtr<DestinationPtr> destinations;
        bool contains = name_dictionary->Dests(destinations);
        if (!contains) {
            throw GeneralException("Document does not have destinations in name dictionary");
        }

        auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(dest_obj);

        assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
        if (!destinations->Contains(destination_name)) {
            throw GeneralException("Referenced destination does not exist");
        }

        auto found_dest = destinations->Find(destination_name);
        return found_dest;
    }

    // Handle name object reference to named destinations (via Destinations dictionary)
    if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(dest_obj)) {
        auto weak_file = dest_obj->GetFile();
        auto document_ref = SemanticUtils::GetMappedDocument(weak_file);

        assert(!document_ref.IsEmpty() && "Document reference was not set");
        if (!document_ref.IsActive()) {
            throw GeneralException("Document reference is not active");
        }

        DocumentPtr document = document_ref.GetReference();

        OutputCatalogPtr catalog;
        bool has_catalog = document->GetDocumentCatalog(catalog);
        if (!has_catalog) {
            throw GeneralException("Document does not have a catalog");
        }

        OutputNamedDestinationsPtr destinations;
        bool has_destinations = catalog->Destinations(destinations);
        if (!has_destinations) {
            throw GeneralException("Document does not have named destinations");
        }

        auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(dest_obj);

        assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
        if (!destinations->Contains(destination_name)) {
            throw GeneralException("Referenced destination does not exist");
        }

        return destinations->Find(destination_name);
    }

    LOG_ERROR_AND_THROW_GENERAL("Unknown destination reference type: {}", static_cast<int32_t>(dest_obj->GetObjectType()));
}

DestinationPtr DestinationBase::Create(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::D)) {
        throw GeneralException("Invalid destination dictionary");
    }

    auto destination = root->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);
    return Create(destination);
}

syntax::ObjectPtr DestinationBase::GetPage() const {
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(_obj)) {
        auto destination = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
        return destination->GetValue(0);
    }

    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
        auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
        auto destination = dict->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);
        return destination->GetValue(0);
    }

    assert(false && "Destination was created but object is neither array nor dictionary");
    throw GeneralException("Destination was created but object is neither array nor dictionary");
}

bool NamedDestinations::Contains(const syntax::NameObject& name) const {
    return _obj->Contains(name);
}

DestinationPtr NamedDestinations::Find(const syntax::NameObject& name) const {
    auto found_obj = _obj->Find(name);
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(found_obj)) {
        auto found_array = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(found_obj);
        return DestinationBase::Create(found_array);
    }

    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(found_obj)) {
        auto found_dictionary = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(found_obj);
        return DestinationBase::Create(found_dictionary);
    }

    throw GeneralException("Unable to find entry");
}

void NamedDestinations::Insert(const syntax::NameObject& name, DestinationPtr value) {
    if (_obj->Contains(name)) {
        bool removed = _obj->Remove(name);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }

    auto raw_obj = value->GetObject();
    if (raw_obj->IsIndirect()) {
        syntax::IndirectReferenceObjectPtr reference = make_deferred<syntax::IndirectReferenceObject>(raw_obj);
        _obj->Insert(name, reference);
    } else {
        auto containable = syntax::ObjectUtils::ConvertTo<syntax::ContainableObjectPtr>(raw_obj);
        _obj->Insert(name, containable);
    }
}

} // semantics
} // vanillapdf
