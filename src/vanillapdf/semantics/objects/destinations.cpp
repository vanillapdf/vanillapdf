#include "precompiled.h"

#include "semantics/objects/destinations.h"
#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/catalog.h"
#include "semantics/utils/semantic_exceptions.h"
#include "semantics/utils/semantic_utils.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

bool XYZDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool XYZDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 4) return false;

    auto obj = arr->GetValue(3);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool XYZDestination::GetZoom(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 5) return false;

    auto obj = arr->GetValue(4);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}


bool FitHorizontalDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitVerticalDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetBottom(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 4) return false;

    auto obj = arr->GetValue(3);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetRight(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 5) return false;

    auto obj = arr->GetValue(4);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitRectangleDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 6) return false;

    auto obj = arr->GetValue(5);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitBoundingBoxHorizontalDestination::GetTop(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

bool FitBoundingBoxVerticalDestination::GetLeft(syntax::ObjectPtr& result) const {
    auto arr = GetDestinationArray();
    if (arr->GetSize() < 3) return false;

    auto obj = arr->GetValue(2);
    if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(obj)) {
        return false; // null means no change
    }

    result = obj;
    return true;
}

NamedDestinations::NamedDestinations(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}


syntax::NameObjectPtr DestinationBase::ValidateAndGetDestinationType(syntax::MixedArrayObjectPtr array) {
    if (array->GetSize() < 2) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Invalid destination array");
    }

    syntax::ObjectPtr page_number_obj = array->GetValue(0);
    syntax::ObjectPtr type_obj = array->GetValue(1);

    // page_number_obj shall be indirect reference to page object
    // for remote go to actions it can be integer value of destination page

    // Update 27.9.2025:
    // The page number can be null, where no action is taken by Foxit.
    // Example file is excerpts.pdf /D[null/XYZ 2.04765 614.058 null]
    if (!syntax::ObjectUtils::IsType<syntax::IntegerObjectPtr>(page_number_obj)
        && !syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(page_number_obj)
        && !syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(page_number_obj)) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Invalid page object type in destination: {}",
            static_cast<int32_t>(page_number_obj->GetObjectType()));
    }

    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Destination type is not name object: {}",
            static_cast<int32_t>(type_obj->GetObjectType()));
    }

    return syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);
}

DestinationBase::Type DestinationBase::GetDestinationClassType(syntax::NameObjectPtr type) {
    if (type == constant::Name::XYZ) {
        return DestinationBase::Type::XYZ;
    }

    if (type == constant::Name::Fit) {
        return DestinationBase::Type::Fit;
    }

    if (type == constant::Name::FitH) {
        return DestinationBase::Type::FitHorizontal;
    }

    if (type == constant::Name::FitV) {
        return DestinationBase::Type::FitVertical;
    }

    if (type == constant::Name::FitR) {
        return DestinationBase::Type::FitRectangle;
    }

    if (type == constant::Name::FitB) {
        return DestinationBase::Type::FitBoundingBox;
    }

    if (type == constant::Name::FitBH) {
        return DestinationBase::Type::FitBoundingBoxHorizontal;
    }

    if (type == constant::Name::FitBV) {
        return DestinationBase::Type::FitBoundingBoxVertical;
    }

    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Unknown destination type: {}", type->ToString());
}

DestinationPtr DestinationBase::CreateFromArray(syntax::MixedArrayObjectPtr root) {
    auto type_name = ValidateAndGetDestinationType(root);
    auto type = GetDestinationClassType(type_name);

    switch (type) {
        case DestinationBase::Type::XYZ:
            return make_deferred<XYZDestination>(root);
        case DestinationBase::Type::Fit:
            return make_deferred<FitDestination>(root);
        case DestinationBase::Type::FitHorizontal:
            return make_deferred<FitHorizontalDestination>(root);
        case DestinationBase::Type::FitVertical:
            return make_deferred<FitVerticalDestination>(root);
        case DestinationBase::Type::FitRectangle:
            return make_deferred<FitRectangleDestination>(root);
        case DestinationBase::Type::FitBoundingBox:
            return make_deferred<FitBoundingBoxDestination>(root);
        case DestinationBase::Type::FitBoundingBoxHorizontal:
            return make_deferred<FitBoundingBoxHorizontalDestination>(root);
        case DestinationBase::Type::FitBoundingBoxVertical:
            return make_deferred<FitBoundingBoxVerticalDestination>(root);
        default:
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Unknown destination type: {}", static_cast<int>(type));
    }
}

DestinationPtr DestinationBase::ResolveDestination(syntax::ObjectPtr dest_obj) {
    // Handle direct array destination: [page /Type ...]
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(dest_obj)) {
        auto array_obj = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(dest_obj);
        return CreateFromArray(array_obj);
    }

    // Handle direct dictionary destination: {/D [page /Type ...]}
    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(dest_obj)) {
        auto dict_obj = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(dest_obj);
        return CreateFromDictionary(dict_obj);
    }

    // Handle string name reference to named destinations (via Names dictionary)
    if (syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(dest_obj)) {
        auto weak_file = dest_obj->GetFile();

        OutputDocumentPtr document;
        if (!SemanticUtils::TryGetMappedDocument(weak_file, document)) {
            auto file = weak_file.GetReference();
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document for file {} is not mapped or is no longer active", file->GetFilenameString());
        }

        OutputCatalogPtr catalog_ptr;
        bool has_catalog = document->GetDocumentCatalog(catalog_ptr);
        if (!has_catalog) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document does not have a catalog");
        }

        OutputNameDictionaryPtr name_dictionary;
        bool has_dictionary = catalog_ptr->Names(name_dictionary);
        if (!has_dictionary) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document does not have a name dictionary");
        }

        OutputNameTreePtr<DestinationPtr> destinations;
        bool contains = name_dictionary->Dests(destinations);
        if (!contains) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document does not have destinations in name dictionary");
        }

        auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(dest_obj);

        assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
        if (!destinations->Contains(destination_name)) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Referenced destination does not exist");
        }

        auto found_dest = destinations->Find(destination_name);
        return found_dest;
    }

    // Handle name object reference to named destinations (via Destinations dictionary)
    if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(dest_obj)) {
        auto weak_file = dest_obj->GetFile();

        OutputDocumentPtr document;
        if (!SemanticUtils::TryGetMappedDocument(weak_file, document)) {
            auto file = weak_file.GetReference();
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document for file {} is not mapped or is no longer active", file->GetFilenameString());
        }

        OutputCatalogPtr catalog;
        bool has_catalog = document->GetDocumentCatalog(catalog);
        if (!has_catalog) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document does not have a catalog");
        }

        OutputNamedDestinationsPtr destinations;
        bool has_destinations = catalog->Destinations(destinations);
        if (!has_destinations) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Document does not have named destinations");
        }

        auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(dest_obj);

        assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
        if (!destinations->Contains(destination_name)) {
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Referenced destination does not exist");
        }

        return destinations->Find(destination_name);
    }

    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Unknown destination reference type: {}", static_cast<int32_t>(dest_obj->GetObjectType()));
}

DestinationPtr DestinationBase::CreateFromDictionary(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::D)) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Invalid destination dictionary");
    }

    auto destination_array = root->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);
    auto type_name = ValidateAndGetDestinationType(destination_array);
    auto type = GetDestinationClassType(type_name);

    switch (type) {
        case DestinationBase::Type::XYZ:
            return make_deferred<XYZDestination>(root);
        case DestinationBase::Type::Fit:
            return make_deferred<FitDestination>(root);
        case DestinationBase::Type::FitHorizontal:
            return make_deferred<FitHorizontalDestination>(root);
        case DestinationBase::Type::FitVertical:
            return make_deferred<FitVerticalDestination>(root);
        case DestinationBase::Type::FitRectangle:
            return make_deferred<FitRectangleDestination>(root);
        case DestinationBase::Type::FitBoundingBox:
            return make_deferred<FitBoundingBoxDestination>(root);
        case DestinationBase::Type::FitBoundingBoxHorizontal:
            return make_deferred<FitBoundingBoxHorizontalDestination>(root);
        case DestinationBase::Type::FitBoundingBoxVertical:
            return make_deferred<FitBoundingBoxVerticalDestination>(root);
        default:
            LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Unknown destination type: {}", static_cast<int>(type));
    }
}

DestinationPtr DestinationBase::CreateFromObject(syntax::ObjectPtr root) {
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(root)) {
        auto arr = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(root);
        return CreateFromArray(arr);
    }

    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(root)) {
        auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(root);
        return CreateFromDictionary(dict);
    }

    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Invalid object type: {}", static_cast<int32_t>(root->GetObjectType()));
}

syntax::MixedArrayObjectPtr DestinationBase::GetDestinationArray() const {
    if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(_obj)) {
        return syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(_obj);
    }

    if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(_obj)) {
        auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(_obj);
        return dict->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::D);
    }

    assert(false && "Destination was created but object is neither array nor dictionary");
    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Destination was created but object is neither array nor dictionary");
}

syntax::ObjectPtr DestinationBase::GetPage() const {
    auto destination = GetDestinationArray();
    return destination->GetValue(0);
}

bool NamedDestinations::Contains(const syntax::NameObject& name) const {
    return _obj->Contains(name);
}

DestinationPtr NamedDestinations::Find(const syntax::NameObject& name) const {
    auto found_obj = _obj->Find(name);
    return DestinationBase::CreateFromObject(found_obj);
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
